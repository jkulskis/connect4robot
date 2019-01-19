/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/
// defines pins numbers
const int trigPin = 45;
const int echoPin = 44;
// defines variables
long duration;
int distance;
int previousDistance;
boolean checker;
int countTested;
int countSeen;
int totalDuration;
int averageDuration;
 
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  
  // Prints the distance on the Serial Monitor
//  if(!checker && 0){
//    if(previousDistance != 0 && distance < 4 && countTested > 5){
//      Serial.print("Distance: ");
//      Serial.println(distance);
//      Serial.println("Checker! :)))))))))))))))))))))))))))))))))");
//      checker = true;
//    }
//    else{
//      delay(5);
//      Serial.println("No Checker :(");
//      Serial.print("Distance: ");
//      Serial.println(distance);
//      previousDistance = distance;
//    }
//  }
  if(!checker){
    countTested++;
    averageDuration = 369;
    duration = getDuration(trigPin, echoPin);
    
    if (duration < 285 || duration > 400){
      totalDuration = 0;
      for(int i = 0; i < 3; i++){
        totalDuration += getDuration(trigPin, echoPin);
        delay(2);
      }
      averageDuration = totalDuration / 3;
    }
    
    if((averageDuration < 0 || averageDuration > 400) && countTested > 10){
      Serial.print("Avg Duration: ");
      Serial.println(averageDuration);
      Serial.println("Checker! :)");
      checker = true;
    }
    else{
      //Serial.println("No Checker :(");
      Serial.print("Duration: ");
      Serial.println(duration);
    }
    delay(2);
    }
}

int getDuration(int trigPin, int echoPin){
      // Clears the trigPin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(5);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      return pulseIn(echoPin, HIGH);
}

