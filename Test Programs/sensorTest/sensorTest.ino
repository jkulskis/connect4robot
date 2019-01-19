const int IN_A0 = A0; // analog input
int initial_val;
void setup() {
  pinMode (IN_A0, INPUT);
  Serial.begin(9600);
  initial_val = analogRead(IN_A0);
  Serial.print("Initial Value: ");
  Serial.println(initial_val);
}

int value_A0;
boolean no_checker = true;

void loop() {
  // put your main code here, to run repeatedly:
  if (no_checker){
    value_A0 = analogRead(IN_A0);
    if (initial_val - value_A0 > 100){
      Serial.println("Checker Detected!");
      Serial.println(value_A0);
      no_checker = false;
    }
    else{
      Serial.println("No Checker!");
      Serial.println(value_A0);
    }
  }
  delay(50);
}
