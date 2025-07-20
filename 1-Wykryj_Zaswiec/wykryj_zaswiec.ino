const int GREEN_LED = 2;
const int RED_LED   = 3;
const int BLUE_LED = 4;

void setup() {
  pinMode(RED_LED,   OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    int c = Serial.read();
    if (c == '1') {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
    } else if (c == '2') {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
    } else if (c == '3') {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
    } else if (c == '4') {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
    }
  }
}
