#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const char * phrases[2] = {"1234567891234567 234567891234567", "Wow that was cool"};
const int maxPhraseNum = sizeof(phrases)/sizeof(phrases[0]);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(7));
  lcd.begin();
  lcd.backlight();
  char phrase[33] = "Phrase 1";
  lcdPrint("Phrase 4");
}

void loop() {
  
}

void lcdPrint(const char * phrase){
  
  char firsthalf[33] = "";
  char secondhalf[33] = "";

  //lcd.setCursor(0,0);
  lcd.clear();
  if (strlen(phrase) <= 16){
    lcd.print(phrase);
  }
  else{
    for (int i = 16; i > -1; i--){
      if (phrase[i] == ' '){
        strncpy(firsthalf, phrase, i);
        firsthalf[i] = '\0';
        strncpy(secondhalf, phrase + i + 1, strlen(phrase));
        secondhalf[strlen(phrase) - i - 1] = '\0';
        break;
      }
    }
    lcd.print(firsthalf);
    lcd.setCursor(0,1);
    lcd.print(secondhalf);
  }
}
