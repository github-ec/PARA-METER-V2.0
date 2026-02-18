Public functions of "WordStorage"

**int numberOfCategories() const { return _numberOfCategories; };**

Returns the number of word categories (char arrays) that are available in WordList.h
   
**int katID() const              { return _katID; };**

Returns the index of the actual category (char array) that was used to retrieve a word
It is useful in combination with the function "wordFromData()" to get a feedback about
the category that was randomly chosen 
    
**int totalWords() const         { return _totalWords; };**

Returns the maximum no of words of the actual category (char array) that was used to retrieve a word
It is useful in combination with the function "wordFromData()" to get the maximum number of words
available in the  category that was randomly chosen 

    
**int wordIndex() const          { return _wordIndex; };**

Returns the index of the last word retrieved from the actual category (char array) 
It is useful in combination with the function "wordFromData()" 
    
**const char\* word() const       { return _wordBuffer; };**

Returns the last word retrieved from the actual category (char array) 

**int totalWords(int katID);**

Returns the maximum no of words of the category katID (char array).
    
**bool wordFromData(int data);**

Returns true if a word has been chosen randomly using the value of data and an internal function 
of WordStorage that creates a random value for the category and the word index.
After this function has been called successfully the word can be retrieved by calling .word(), the category 
chosen by  .katID() and the word index by calling .wordIndex().
    
**bool wordFromCatEntry(int katID, int entry);**

Returns true if category katdID and index entry allow to successfully retrieve a word from PROGMEM.
After this function has been called successfully the word can be retrieved by calling .word(), the category 
chosen by  .katID() and the word index by calling .wordIndex().

  
```

#include "WordStorage.h";

WordStorage wordStore;

void setup() {
  Serial.begin(115200);
  printWordFromCatEntry();
  printWordFromData();
}

void loop() {
  // put your main code here, to run repeatedly:
}


void printWordFromData() {
  for (int i = 0; i < 10; i++) {
    int data = analogRead(A0);
    if (wordStore.wordFromData(data)) {
      Serial.print(data);
      Serial.print('\t');
      Serial.print(wordStore.word());
    }
    Serial.println();
  }
}

void printWordFromCatEntry() {
  int i = 0;
  while (wordStore.wordFromCatEntry(i, 0)) {
    Serial.println(wordStore.word());
    i++;
  }
  Serial.println(wordStore.numberOfCategories());
};

 

```
