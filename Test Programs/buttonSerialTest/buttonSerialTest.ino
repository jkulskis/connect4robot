const int buttonPin = 9;

int buttonState = 0;
int index = 0;
boolean userTurn = true;

void setup() {
  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  buttonState = digitalRead(buttonPin);
  //Button state = high fires when a checker is detected
  //With the actual program, instead of button state, each
  //sensor will check if a checker moved, and the appropriate
  //column will be assigned
  if (userTurn == true) {
    
    if (buttonState == HIGH){
      int col = random(0, 6);
      Serial.print(col);
      
      //Serial.print("Player moved to: ");
      //Serial.println(col);
      
      userTurn = false;
    }
  }
}

void serialEvent(){
  while (Serial.available()){
    data = Serial.read()
    if (data == "lost"){
      //Print I'll get you next time
    }
    if (data == "won"){
      //Print Better Luck Next Time
    }
    if (data == "tied"){
      //Print Guess We're Even
    }
    int col = Serial.parseInt();
    if (userTurn == false){
      //Make the move with the motors from the received column
      
      //Serial.print("Computer moved to: ");
      //Serial.print(col);
      
      userTurn = true;
    }
  }
}

