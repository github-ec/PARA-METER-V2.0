#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

class wordPlayerClass {
private:
  SoftwareSerial mySoftwareSerial;
  DFRobotDFPlayerMini myDFPlayer;
  bool _ready = false;
  bool _verbose = false;
  bool _doesPlay = false;
  bool _initFailed = false;
  void printDetail(uint8_t type, int value);
public:
  wordPlayerClass(byte rxPin, byte txPin);
  void init();
  void handlePlayerStatus();
  void playTitle(int value);
  bool playingTitle(int value);
  bool isReady();
  void setReady(bool value);
  bool initFailed();
  bool isPlaying();
  void messagesOn();
  void messagesOff();
  void setVolume(byte value);
  int getMaxFiles();
};

wordPlayerClass::wordPlayerClass(byte rxPin, byte txPin)
  : mySoftwareSerial(rxPin, txPin) {}


void wordPlayerClass::init() {
  _ready = false;
  _initFailed = false;
  mySoftwareSerial.begin(9600);
  if (!Serial) Serial.begin(115200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Initializing DFPlayer ..."));
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("DFPlayer init failed!"));
    Serial.println(F("Check wiring and SD card."));
    _initFailed = true;
    return;
  }
  delay(800);
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(30);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  Serial.println(F("DFPlayer Mini online."));
  _ready = true;
}

bool wordPlayerClass::initFailed() {
  return _initFailed;
}

void wordPlayerClass::setReady(bool value) {
  _ready = value;
}

bool wordPlayerClass::isReady() {
  return _ready && !_initFailed;
}

bool wordPlayerClass::isPlaying() {
  return _doesPlay && !_ready;
}

void wordPlayerClass::messagesOn() {
  _verbose = true;
}
void wordPlayerClass::messagesOff() {
  _verbose = false;
}
void wordPlayerClass::setVolume(byte value) {
  myDFPlayer.volume(value);
}

bool wordPlayerClass::playingTitle(int value) {
  playTitle(value);
  return _doesPlay;
}

int wordPlayerClass::getMaxFiles(){
  static bool firstCall = true;
  if (_initFailed) return -1;
  if (firstCall){
    // The first call does always return -1 
    firstCall = false;
    int dummy = myDFPlayer.readFileCounts();
  }
  return myDFPlayer.readFileCounts();
}


void wordPlayerClass::playTitle(int value) {
  if (_initFailed) return;
  _doesPlay = false;
  if (value > 0) {
    myDFPlayer.playMp3Folder(value);
    _doesPlay = true;
    _ready = false;
  }
}

void wordPlayerClass::handlePlayerStatus() {
  if (_initFailed) return;
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
    _ready = true;
  }
}

void wordPlayerClass::printDetail(uint8_t type, int value) {
  static bool firstTimeOut = true;
  if (!_verbose) return;
  switch (type) {
    case TimeOut:
      if (!firstTimeOut) {
        Serial.println(F("Time Out!"));
      } else {
        firstTimeOut = false;
      }
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println(F("USB Inserted!"));
      break;
    case DFPlayerUSBRemoved:
      Serial.println(F("USB Removed!"));
      break;
    case DFPlayerPlayFinished:
      Serial.println(F("Play Finished!"));
      _ready = true;
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Checksum Error"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
