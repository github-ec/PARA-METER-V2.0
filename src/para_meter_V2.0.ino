#include <avr/pgmspace.h>

/* 
 * DIY "PARA"-METER SYSTEM (PROGMEM EDITION)
 * Version 2.0 mit NOIASCA LCD Treiber 
 * 2026/02/18
 *
 * ec2021
 *
 * 
 */

// --- LCD VORBEREITUNG ----
#include "GermanLCD.h"

// --- WORTLISTEN HANDLING (FLASH-SPEICHER -> PROGMEM) ---
#include "WordStorage.h"
WordStorage wordStore;

// --- DFROBOT PLAYER MINI HANDLING ---

#include "WordPlayer.h"

constexpr byte RXPin{ 10 };
constexpr byte TXPin{ 11 };
wordPlayerClass wordPlayer(RXPin, TXPin);

// --- HAUPTPROGRAMM ---
unsigned long lastTime = 0;
bool verbose = false;


void setup() {
  // Pin A1 offen lassen für echten Zufalls-Seed im Setup
  randomSeed(analogRead(A1));
  Serial.begin(115200);
  
  InitLCDGerman();
  lcd.setCursor(0, 0);
  lcd.print("PARA-METER 2.0");
  lcdZeile2("Initialisieren");

  wordPlayer.init();
  wordPlayer.messagesOff();
  //wordPlayer.messagesOn();
  Serial.println(F("--- PARA-METER DIY GESTARTET ---"));
  Serial.println(F("Warte auf EMF-Ausschlag an Pin A0..."));
  lcd.setCursor(0, 1);
  lcd.print("                  ");
}

void loop() {
  int currentRead = analogRead(A0);
  if (millis() - lastTime > 3000) {
    lastTime = millis();
    if (currentRead > 250) {
      if (!wordStore.wordFromData(currentRead)) return;
      int mp3Index = wordStore.katID() * 100 + wordStore.wordIndex();
      serielleAusgabe(mp3Index, currentRead);
      lcdWortAusgabe();
      wordPlayer.playTitle(mp3Index);
    }
  }
  wordPlayer.handlePlayerStatus();
  delay(20);
}

void lcdWortAusgabe() {
  char buf[20];
  strncpy(buf, wordStore.word(), sizeof(buf));
  lcdZeile2(buf);
}

void lcdZeile2(char * buf){
  lcd.setCursor(0, 1);
  lcd.print("                  ");
  lcd.setCursor(0, 1);
  lcd.print(buf);
}


void serielleAusgabe(int Idx, int data) {
  if (verbose) {
    Serial.print(F("[EVENT] Energie: "));
    Serial.print(data);
    Serial.print(F(" | Kat: "));
    Serial.print(wordStore.katID());
    Serial.print(F(" | Index: "));
    Serial.print(wordStore.wordIndex());
  }
  Serial.print(F(" | Wort: "));
  Serial.print(wordStore.word());
  Serial.print(" | Play ");
  Serial.println(Idx);
}
