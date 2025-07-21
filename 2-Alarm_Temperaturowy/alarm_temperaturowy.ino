// --- Definicja pinów ---
const int sensorPin   = A0;  // pin A0: tu podłączamy środkową nóżkę sensora LM35 (wartość analogowa)
const int greenLedPin = 8;   // pin 8: zielona dioda LED
const int redLedPin   = 9;   // pin 9: czerwona dioda LED
const int buzzerPin   = 10;  // pin 10: buzzer (głośniczek)

void setup() {
  // Ustawiamy wyjścia cyfrowe
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // 1) Odczyt surowej wartości temperatury (0–1023)
  int rawValue = analogRead(sensorPin);
  
  // 2) Przeliczanie:
  //    5.0 V odpowiada wartości 1023 → dzielimy przez 1023 i mnożymy przez 5.0
  float voltage = rawValue * (5.0 / 1023.0);
  
  // 3) Przeliczanie napięcia na temperaturę [°C]:
  //    LM35 daje 10 mV na każdy stopień → 0.01 V/°C → dzielimy przez 0.01
  float temperatureC = voltage / 0.01;
  
  // 4) Porównanie z progiem 26 °C
  if (temperatureC < 26.0) {
    // Temperatura poniżej 26 °C:
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin,   LOW);
    // buzzer wyciszony
    digitalWrite(buzzerPin,   LOW);
  } else {
    // Temperatura 26 °C lub wyżej:
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin,   HIGH);
    // buzzer: dwa razy beep-beep
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      delay(200);
    }
  }
  
  // 5) Krótka pauza przed kolejnym odczytem:
  delay(1000);  // 1000 milisekund = 1 sekunda
}
