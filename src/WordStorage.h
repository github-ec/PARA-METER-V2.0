#pragma once

/*
  WordStorage V2.0  dated 2026/02/18
  ec2021
  - Added buffer overflow protection in _getNthWord
  - Cleaned up naming conventions and constants
*/

#include "WordLists.h"

class WordStorage {
  private:
    int _katID = 0;
    int _totalWords = 0;
    int _wordIndex = 0;
    int _numberOfCategories = NumberOfCategories;
    const char* _chosenList = nullptr;
    char _wordBuffer[40]; // Fest definierte Größe für Puffer-Sicherheit

    int _countWords(const char* flashStr);
    // n-tes Wort sicher extrahieren (mit Größenlimit)
    void _getNthWord(const char* flashStr, int n, char* buffer, size_t bufferSize);
    int _calculateIndex(int data);
    void _getListData(int katID);

  public:
    WordStorage();

    int numberOfCategories() const { return _numberOfCategories; };
    int katID() const              { return _katID; };
    int totalWords() const         { return _totalWords; };
    int wordIndex() const          { return _wordIndex; };
    const char* word() const       { return _wordBuffer; };

    int totalWords(int katID);
    bool wordFromData(int data);
    bool wordFromCatEntry(int katID, int entry);
};

WordStorage::WordStorage() {
  _numberOfCategories = NumberOfCategories;
}

int WordStorage::_countWords(const char* flashStr) {
  if (!flashStr) return 0;
  int count = 0;
  int i = 0;
  char c;
  while ((c = pgm_read_byte(flashStr + i++)) != '\0') {
    if (c == '/') count++;
  }
  return count + 1;
}

void WordStorage::_getNthWord(const char* flashStr, int n, char* buffer, size_t bufferSize) {
  if (bufferSize == 0) return;
  
  int currentWord = 0;
  int i = 0;
  int j = 0;
  char c;

  // Zum n-ten Wort springen
  while (currentWord < n) {
    c = pgm_read_byte(flashStr + i++);
    if (c == '/') currentWord++;
    if (c == '\0') {
      buffer[0] = '\0';
      return;
    }
  }

  // Wort kopieren, bis Trenner, Ende oder Puffer voll (Platz für \0 lassen)
  while (j < (int)(bufferSize - 1)) {
    c = pgm_read_byte(flashStr + i++);
    if (c == '/' || c == '\0') break;
    buffer[j++] = c;
  }
  buffer[j] = '\0'; // Sicherer Nullterminator
}

int WordStorage::_calculateIndex(int data) {
  unsigned long timestamp = millis();
  int index = (data ^ (int)(timestamp & 0xFF)) % _totalWords;
  return (index < 0) ? -index : index;
}

int WordStorage::totalWords(int katID) {
  katID = constrain(katID, 0, _numberOfCategories - 1);
  _getListData(katID);
  return _totalWords;
}

void WordStorage::_getListData(int katID) {
  if (katID < _numberOfCategories) {
    _katID = katID;
    _chosenList = (const char*)pgm_read_word(&(cat_table[katID]));
  } else {
    _chosenList = nullptr;
  }
  _totalWords = _countWords(_chosenList);
}

bool WordStorage::wordFromData(int data) {
  int katID = data % _numberOfCategories;
  return wordFromCatEntry(katID, -data);
}

bool WordStorage::wordFromCatEntry(int katID, int entry) {
  katID = constrain(katID, 0, _numberOfCategories - 1);
  _getListData(katID);

  if (!_chosenList) return false;

  if (entry < 0) {
    entry = _calculateIndex(-entry);
  }

  if (entry >= 0 && entry < _totalWords) {
    _wordIndex = entry;
    _getNthWord(_chosenList, _wordIndex, _wordBuffer, sizeof(_wordBuffer));
    return true;
  }
  return false;
}
