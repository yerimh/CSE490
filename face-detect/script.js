const video = document.getElementById('video')

const serial = new Serial();
const inputText = document.querySelector('input');
const outputText = document.getElementById('output-text');
const rcvdText = document.getElementById('received-text');

serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);

let arduinoFace;
let leftScore = 0;
let rightScore = 0;
let time = 30;
let buttonVal = 1;
let lastButtonVal = 1;
let gameOn = false;

window.onload = function(){
  document.getElementById('left-score').innerText = leftScore;
  document.getElementById('right-score').innerText = rightScore;
  document.getElementById('time').innerText = "PRESS BUTTON TO START";
};

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
  document.getElementById('expression').append(canvas)
  const displaySize = { width: video.width, height: video.height }
  faceapi.matchDimensions(canvas, displaySize)
  let before0, after0, before1, after1;
  let exp;
  setInterval(async() => {
    // FACE API
    const detections = await faceapi.detectAllFaces(video, new faceapi.TinyFaceDetectorOptions()).withFaceLandmarks().withFaceExpressions()
    const resizedDetections = faceapi.resizeResults(detections, displaySize)
    canvas.getContext('2d').clearRect(0, 0, canvas.width, canvas.height)
    faceapi.draw.drawDetections(canvas, resizedDetections)
    faceapi.draw.drawFaceLandmarks(canvas, resizedDetections)
    faceapi.draw.drawFaceExpressions(canvas, resizedDetections)

    // FACE MATCH
    if (detections) {
      if (detections.length === 1) { // SINGLE PLAYER
        exp = detections[0].expressions;
        after0 = Object.keys(exp).reduce((a, b) => exp[a] > exp[b] ? a : b);
        if (after0 != before0 && arduinoFace == after0) {
          serialWriteTextData("correct");
        } else {
          serialWriteTextData("incorrect");
        }
        before0 = after0;
      } else if (detections.length > 1) { // TWO PLAYERS
        let leftFace, rightFace;
        if (detections[0].alignedRect.box.left < detections[1].alignedRect.box.left) {
          leftFace = detections[0].expressions;
          rightFace = detections[1].expressions;
        } else {
          leftFace = detections[1].expressions;
          rightFace = detections[0].expressions;
        }
        after0 = Object.keys(leftFace).reduce((a, b) => leftFace[a] > leftFace[b] ? a : b);
        after1 = Object.keys(rightFace).reduce((a, b) => rightFace[a] > rightFace[b] ? a : b);

        // WHO WON
        if (after0 != before0 && arduinoFace == after0) {
          serialWriteTextData("left");
          if (gameOn) leftScore++;
          document.getElementById('left-score').innerText = leftScore;
        } else if (after1 != before1 && arduinoFace == after1) {
          serialWriteTextData("right");
          if (gameOn) rightScore++;
          document.getElementById('right-score').innerText = rightScore;
        } else {
          serialWriteTextData("incorrect");
        }
        before0 = after0;
        before1 = after1;
        console.log(after0, after1);
      }
    }
  }, 100)
})

/**
 * Starts the game and counts down the timer
 */
function startGame() {
  time = 30;
  leftScore = rightScore = 0;
  gameOn = true;
  document.getElementById('left-score').innerText = leftScore;
  document.getElementById('right-score').innerText = rightScore;

  let x = setInterval(function() {
    document.getElementById('time').innerText = time;
    time--;

    if (time < 1) {
      clearInterval(x);
      document.getElementById('time').innerText = "GAME OVER";
      gameOn = false;
    }
  }, 1000);
}

async function onButtonConnectToSerialDevice() {
  console.log("onButtonConnectToSerialDevice");
  if (!serial.isOpen()) {
    await serial.connectAndOpen();
  }
}

async function serialWriteTextData(textData) {
  if (serial.isOpen()) {
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
  let startIndex = 0;
  let endIndex = newData.indexOf(',');
  if (endIndex != -1) {
    arduinoFace = newData.substring(startIndex, endIndex);
    buttonVal = newData.substring(endIndex + 1);
    console.log(buttonVal);

    if (buttonVal != lastButtonVal && buttonVal == 0) {
      startGame();
    }
    lastButtonVal = buttonVal;
  }
}

async function onConnectButtonClick() {
  console.log("Connect button clicked!");
}