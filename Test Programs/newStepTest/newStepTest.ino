//declare variables for the motor pins
int motorPin1 = 2;    // Orange   - 28BYJ48 pin 1
int motorPin2 = 3;    // Yellow   - 28BYJ48 pin 2
int motorPin3 = 4;    // Green - 28BYJ48 pin 3
int motorPin4 = 5;    // Blue - 28BYJ48 pin 4

int transistorPin = 7; // Purple - transistor pin

int motorSpeed = 900;  //variable to set stepper speed
int count = 0;          // count of steps made
int countsperrev; // number of steps per full revolution
int stepperCases[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001}; //Stepper Cases...1=HIGH, 0=LOW
//////////////////////////////////////////////////////////////////////////////
void setup() {
  //declare the motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  //Transistor Pin as output
  pinMode(transistorPin, OUTPUT);
  
  Serial.begin(9600);
  randomSeed(analogRead(7));
  countsperrev = 110;
}

//////////////////////////////////////////////////////////////////////////////
void loop(){
  digitalWrite(transistorPin, HIGH);
  if (count < countsperrev ){
    anticlockwise();
  }
  else if (count == countsperrev * 2){
    //digitalWrite(transistorPin, LOW);
    delay(5000);
    //digitalWrite(transistorPin, LOW);
    countsperrev = 110;
    count = 0;
  }
  else{
    if (count == countsperrev){
      //digitalWrite(transistorPin, LOW);
      delay(5000);
      //digitalWrite(transistorPin, LOW);
    }
    clockwise();
  }
  count++;
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
void anticlockwise()
{
  for(int i = 0; i < 8; i++)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise()
{
  for(int i = 7; i >= 0; i--)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput(int out)
{
  digitalWrite(motorPin1, bitRead(stepperCases[out], 0));
  digitalWrite(motorPin2, bitRead(stepperCases[out], 1));
  digitalWrite(motorPin3, bitRead(stepperCases[out], 2));
  digitalWrite(motorPin4, bitRead(stepperCases[out], 3));
}
