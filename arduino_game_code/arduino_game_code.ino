#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Declare variables for the rope motor pins
const int ropePin1 = 10;    // Orange   - 28BYJ48 pin 1
const int ropePin2 = 11;    // Yellow   - 28BYJ48 pin 2
const int ropePin3 = 12;    // Green - 28BYJ48 pin 3
const int ropePin4 = 13;    // Blue - 28BYJ48 pin 4

const int pusherPin1 = 2;    // Orange   - 28BYJ48 pin 1
const int pusherPin2 = 3;    // Yellow   - 28BYJ48 pin 2
const int pusherPin3 = 4;    // Green - 28BYJ48 pin 3
const int pusherPin4 = 5;    // Blue - 28BYJ48 pin 4

const int transistorPin = 7; // Purple - transistor pin

const int motorSpeed = 700;  // Variable to set stepper speed
const int countsPerColumn = 303; // Number of steps to travel over one column
const int stepperCases[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001}; // Stepper Cases...1=HIGH, 0=LOW

// Trig Pin and Echo Pin locations. Indeces correspond to sensor numbers
const int trigPins[7] = {53, 51, 49, 47, 45, 43, 41};
const int echoPins[7] = {52, 50, 48, 46, 44, 42, 40};

// Duration Cutoffs for different sensors. Indeces correspond to sensor numbers
int lower_cutoffs[7] = {310, 270, 290, 290, 300, 300, 285};  
int upper_cutoffs[7] = {370, 400, 1000, 400, 400, 400, 400};

// Duration Values and indeces for the sensors
int durations[7];
int averageDurations[7] = {369, 369, 369, 369, 369, 369, 369};
int totalDuration;
const int microSecondSensorDelay = 700;
const int testsBeforeCheck = 100;

const int beginButton = 22;

boolean humanTurn = true;
boolean difficultySelected = false;

LiquidCrystal_I2C lcd(0x27,16,2);

const char * thinkingPhrases[15] = {"I need to think a little", "Oh you think you're clever", "I wonder what I'll do next", 
  "Some moves can't be rushed", "Where will I go?", "Nice try, buster", "All of these possibilities...", 
  "I love all the possibilities", "Wow this is a big board", "Hmmmm...", "What will happen next?", 
  "Let me reach into my noggin", "Hahahah you're clever", "Interesting...", "What a play!"};

const char * chosePhrases[14] = {"This column looks good!", "This is a tasty move", ":) How about this choice", 
  "I'll try my luck here", "This move is very calculated", "This one will confuse you", "I think I've chosen", 
  "I choose to go here", "How about I move here", "What about this one?", "Hahahaha this choice is great", "Wow, I'm so smart", 
  "Here's a good move", "How about I go here"};


const int maxThinkingPhraseNum = sizeof(thinkingPhrases)/sizeof(thinkingPhrases[0]);
const int maxChosePhraseNum = sizeof(chosePhrases)/sizeof(chosePhrases[0]);
boolean computerPrintYet;

void setup() {
  // Declare the motor and transistor pins as outputs
  pinMode(ropePin1, OUTPUT);
  pinMode(ropePin2, OUTPUT);
  pinMode(ropePin3, OUTPUT);
  pinMode(ropePin4, OUTPUT);
  
  pinMode(pusherPin1, OUTPUT);
  pinMode(pusherPin2, OUTPUT);
  pinMode(pusherPin3, OUTPUT);
  pinMode(pusherPin4, OUTPUT);

  pinMode(transistorPin, OUTPUT);
  
  // Initialize the pushbutton pin as an input
  pinMode(beginButton, INPUT);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  randomSeed(analogRead(7));

  //Set the trig and echo pins
  for (int i = 0; i < 7; i++){
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
}

void loop() {
  // If the beginButton is pressed, then beginState will be set to HIGH and
  // a game will begin shortly
  int beginState = digitalRead(beginButton);

  if (difficultySelected){
    // Check for a human move if it is the human's turn
    if (humanTurn == true) {

      int countTested = 0;

      // Calibrate all of the sensors in case anything moved and for good measure
      calibrate();
      
      while(1){
        //Delay a little between tests
        delayMicroseconds(microSecondSensorDelay);
        
        // computerPrintYet is checked so that the LCD is not constantly being reset:
        // The message is only printed once after the sensors have started and
        // gone off more than the testsBeforeCheck
        if (computerPrintYet == false && countTested > testsBeforeCheck){
          lcdPrint("Now you go!");
          computerPrintYet = true;
        }
        int humanCol = checkColumns(countTested);

        countTested++;
        if (humanCol != -1){
          // Form a printout message to show where the player went . . . mostly for debugging purposes,
          // can change to a quirky phrase later
          char printOut[16] = "Player Moved: ";
          itoa(humanCol, &printOut[14], 10);
          lcdPrint(printOut);

          // Send the human move to the computer
          Serial.print(humanCol);

          // Let the arduino know that the next move will be from the computer and reset the computer
          // lcd print boolean
          humanTurn = false;
          computerPrintYet = false;
          break;
        }
      }
      
    }
    else{
      // If it is the computer's turn, the move will execute through a serial event,
      // but a printout will occur with the computer's "thinking" message
      if(computerPrintYet == false){
        lcdPrint(thinkingPhrases[random(maxThinkingPhraseNum)]);
        computerPrintYet = true;
      }
    }
  }
  else{
    int buttonDelay = 0;
    
    while (beginState == HIGH){
      delay(100);
      buttonDelay++;
      beginState = digitalRead(beginButton);
    }
    if (buttonDelay > 10){
      Serial.print("i");
      difficultySelected = true;
    }
    else if (buttonDelay > 0){
      Serial.print("h");
      difficultySelected = true;
    }
  }
}

void serialEvent(){
  /*
   * This function fires off when there is a Serial Event
   */
  if (Serial.available()){
    char * data = readSerial();
    if (strcmp(data, "loss") == 0){
      difficultySelected = false;
      lcdPrint("I'll get you next time"); 
    }
    else if (strcmp(data, "win") == 0){
      difficultySelected = false;
      lcdPrint("Better Luck Next Time");
    }
    else if (strcmp(data, "tie") == 0){
      difficultySelected = false;
      lcdPrint("Guess We're Even"); 
    }
    else{
      
      if (humanTurn == false){

        // Get the desired computer column through the serial data
        int col = data[0] - '0';
        lcdPrint(chosePhrases[random(maxChosePhraseNum)]);

//        // Form and print a message for the computer move       //DEBUG ******************
//        char printOut[16] = "Computer Move: ";
//        itoa(col, &printOut[14], 10);
//        lcdPrint(printOut);
        
        // Make the move with the motors from the received column
        coverColumns(col);

        // Let the arduino know that the next move will be from the human
        // and reset the computer printout boolean
        humanTurn = true;
        computerPrintYet = false;   
      }
    }
  }
}

void lcdPrint(const char * phrase){
  /*
   * Prints out a message on a 16 x 2 i2c LCD
   * This function best chooses how to display a valid phrase
   * by allocating different words to either row 0 or 1
   */
  char firsthalf[33] = "";
  char secondhalf[33] = "";

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

char * readSerial(){
  /*
   * Returns the Serial Data sent from the computer 
   */
  char * receivedData = (char*) malloc(10 * sizeof(char));
  int rindex = 0;
  while (Serial.available()){
    delay(3);
    char received = Serial.read();
    if (received == '*'){
      receivedData[rindex] = '\0';
      break;
    }
    receivedData[rindex] = received;
    rindex++;
  }
  return receivedData;
}

int checkColumns(int countTested){
  /*
   * Checks if a column was played in, and if so returns the column number.
   * If no column was played, return -1
   * input countTested is necessary so that the initial values are ignored
   * since the sensors often give incorrect initial values
   */
  // Get the duration values for each sensor
  getDurations();

  for (int i = 0; i < 7; i++){
    // Check if any of the durations meet the cutoff after testsBeforeCheck tests have taken place
    if (durations[i] < lower_cutoffs[i] || durations[i] > upper_cutoffs[i] && countTested > testsBeforeCheck){
      // If a duration meets the cutoff, then focus on that sensor and find the average of the
      // trigger value and the next 3 durations of that sensor
      totalDuration = durations[i];
      for(int n = 0; n < 3; n++){
        totalDuration += getDuration(i);
        delayMicroseconds(microSecondSensorDelay);
//        Serial.print("Total Duration for "); DEBUG********************
//        Serial.print(i);
//        Serial.print(" is: ");
//        Serial.println(totalDuration);
      }
      averageDurations[i] = totalDuration / 4;
    }
  }
  for (int i = 0; i < 7; i++){
    // If the average duration meets the cutoff, then return the respective sensor index
    if((averageDurations[i] < lower_cutoffs[i] || averageDurations[i] > upper_cutoffs[i])){
//      Serial.print(i);                        //DEBUG********************
//      Serial.print(", Avg Duration: "); 
//      Serial.println(averageDurations[i]);
//      Serial.print(i);
//      Serial.println(": Checker! :)");
      averageDurations[i] = 369;
      return 6 - i;
    }
  }
  
  return -1;
}

// Motor code based off of example found at http://www.4tronix.co.uk/arduino/Stepper-Motors.php
//////////////////////////////////////////////////////////////////////////////
// Set pins to ULN2003 high in sequence from 1 to 4
// Delay "motorSpeed" between each pin setting (to determine speed)
void motorAnticlockwise(int pin1, int pin2, int pin3, int pin4)
{
  for(int i = 0; i < 8; i++)
  {
    setMotorOutput(i, pin1, pin2, pin3, pin4);
    delayMicroseconds(motorSpeed);
  }
}

void motorClockwise(int pin1, int pin2, int pin3, int pin4)
{
  for(int i = 7; i >= 0; i--)
  {
    setMotorOutput(i, pin1, pin2, pin3, pin4);
    delayMicroseconds(motorSpeed);
  }
}

void setMotorOutput(int out, int pin1, int pin2, int pin3, int pin4)
{
  digitalWrite(pin1, bitRead(stepperCases[out], 0));
  digitalWrite(pin2, bitRead(stepperCases[out], 1));
  digitalWrite(pin3, bitRead(stepperCases[out], 2));
  digitalWrite(pin4, bitRead(stepperCases[out], 3));
}
//////////////////////////////////////////////////////////////////////////////

void coverColumns(int col){
  /*
   * Covers the appropriate columns with the rope motor given an input col
   * for the computer's next move
   */

  // Haven't set up the transistor yet, but if heat problems occur with the motos
  // then I will in the future
  // digitalWrite(transistorPin, HIGH);
  int stepCount = 0;
  while (stepCount < countsPerColumn * col){
    motorAnticlockwise(ropePin1, ropePin2, ropePin3, ropePin4);
    stepCount++;
  }
  
  // Reset the step count
  stepCount = 0;
  // Delay 3 seconds for checker to fall
  pushChecker();
  delay(3000);

  // Check to see if there was a win/loss/tie message
  if (serialEventRun) serialEventRun();
  
  while (stepCount < countsPerColumn * col){
    motorClockwise(ropePin1, ropePin2, ropePin3, ropePin4);
    stepCount++;
  }
  // digitalWrite(transistorPin, LOW);
}

void pushChecker(){
  /*
   * Pushes a checker out from the dispenser
   */
  int stepCount = 0;
  // 110 is the amount of steps to clear the PVC. This first movement brings the pusher
  // Away from the PVC and causes a checker to fall
  while (stepCount < 110){
    motorAnticlockwise(pusherPin1, pusherPin2, pusherPin3, pusherPin4);
    stepCount++;
  }
  
  // Reset the step count
  stepCount = 0;

  delay(5);
  
  // Pushes the fallen checker into the dispenser, which eventually falls into the board
  while (stepCount < 110){
    motorClockwise(pusherPin1, pusherPin2, pusherPin3, pusherPin4);
    stepCount++;
  }
  
}

void getDurations(){
  /*
   * Returns the durations in microseconds when given pointers of valid ultrasound sensor
   * trig and echo pins
   */
  for(int i = 0; i < 7; i++){
    // Clear trigPins[i]
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(5);
    
    // Set trigPins[i] on HIGH state for 10 micro seconds
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);
    
    // Read echoPins[i] and set the duration to durations[i]
    durations[i] = pulseIn(echoPins[i], HIGH);
//    Serial.print(i);                          //DEBUG********************
//    Serial.print(", Duration: ");
//    Serial.println(durations[i]);
    delayMicroseconds(microSecondSensorDelay);
  }
}

int getDuration(int index){
  /*
   * Returns the duration in microseconds of a single pin when given the
   * trig and echo pins
   */
  // Clears the trigPin
  digitalWrite(trigPins[index], LOW);
  delayMicroseconds(5);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPins[index], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins[index], LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  return pulseIn(echoPins[index], HIGH);
}

void calibrate(){
  /*
   * Calibrates all of the sensors by setting the appropriate cutoffs
   */
  // Fire off the sensors 30 times to get a solid average
  int timesFired = 30;
  
  int totalValues[7] = {0,0,0,0,0,0,0};
  for (int i = 0; i < timesFired; i++){
    getDurations();
    for (int n = 0; n < 7; n++){
      totalValues[n] += durations[n];
    }
    delayMicroseconds(microSecondSensorDelay);
  }

  //Set the lower cutoffs to the average of all the runs - 20
  for (int i = 0; i < 7; i++){
    lower_cutoffs[i] = totalValues[i] / timesFired - 20;

    // Sensor 4 isn't very good and I don't want to replace it, so this code
    // just makes it more sensitive
    if (i == 4){
      lower_cutoffs[i] += 10;
    }
  }
  
//  for (int i = 0; i < 7; i++){    // DEBUG ************************
//    Serial.print(i);
//    Serial.print(" Lower cutoff is ");
//    Serial.println(lower_cutoffs[i]);
//  }
}

