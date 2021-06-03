const video = document.getElementById('video')

const serial = new Serial();
const inputText = document.querySelector('input');
const outputText = document.getElementById('output-text');
const rcvdText = document.getElementById('received-text');

serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);


Promise.all([
  faceapi.nets.tinyFaceDetector.loadFromUri('/models'),
  faceapi.nets.faceLandmark68Net.loadFromUri('/models'),
  faceapi.nets.faceRecognitionNet.loadFromUri('/models'),
  faceapi.nets.faceExpressionNet.loadFromUri('/models')
]).then(startVideo)

function startVideo() {
  navigator.getUserMedia({ video: {} },
  stream => video.srcObject = stream,
  err => console.error(err)
  )
}

video.addEventListener('play', () => {
  const canvas = faceapi.createCanvasFromMedia(video)
  document.body.append(canvas)
  const displaySize = { width: video.width, height: video.height }
  faceapi.matchDimensions(canvas, displaySize)
  let before, after;
  setInterval(async() => {
    const detections = await faceapi.detectAllFaces(video, new faceapi.TinyFaceDetectorOptions()).withFaceLandmarks().withFaceExpressions()
    const resizedDetections = faceapi.resizeResults(detections, displaySize)
    canvas.getContext('2d').clearRect(0, 0, canvas.width, canvas.height)
    faceapi.draw.drawDetections(canvas, resizedDetections)
    faceapi.draw.drawFaceLandmarks(canvas, resizedDetections)
    faceapi.draw.drawFaceExpressions(canvas, resizedDetections)

    let exps = detections[0].expressions;
    after = Object.keys(exps).reduce((a, b) => exps[a] > exps[b] ? a : b);
    console.log(after);
    if (after != before) {
      serialWriteTextData(after);
    }
    before = after;
  }, 100)
})

async function onButtonConnectToSerialDevice() {
  console.log("onButtonConnectToSerialDevice");
  if (!serial.isOpen()) {
    await serial.connectAndOpen();
  }
}

async function serialWriteTextData(textData) {
  if (serial.isOpen()) {
    console.log("Writing to serial: ", textData);
    serial.writeLine(textData);
  }
}

function onSerialErrorOccurred(eventSender, error) {
  console.log("onSerialErrorOccurred", error);
}

function onSerialConnectionOpened(eventSender) {
  console.log("onSerialConnectionOpened", eventSender);
}

function onSerialConnectionClosed(eventSender) {
  console.log("onSerialConnectionClosed", eventSender);
}

function onSerialDataReceived(eventSender, newData) {
  rcvdText.textContent = newData;
}

async function onConnectButtonClick() {
  console.log("Connect button clicked!");
}