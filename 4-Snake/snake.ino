// Pins for shift‑register driving the 8×8 LED matrix
const int dataPin  = 2;
const int shiftPin = 4;
const int storePin = 3;

const int BTN_UP    = 5;
const int BTN_DOWN  = 6;
const int BTN_LEFT  = 7;
const int BTN_RIGHT = 8;

int lastEdgeDir = -1;        // 0=UP,1=RIGHT,2=DOWN,3=LEFT, -1=brak
int prevUp, prevRight, prevDown, prevLeft;

byte rowMask[8] = {
  0b01111111,
  0b10111111,
  0b11011111,
  0b11101111,
  0b11110111,
  0b11111011,
  0b11111101,
  0b11111110
};

int snakeX[64], snakeY[64];
int snakeLen = 3;
int dirCurrent = 2, dirNew = 2; // 2 = DOWN
int foodX, foodY;

unsigned long lastMove = 0;
const unsigned long moveDelay = 600;

byte frameBuffer[8];

void setup() {
  // inicjalizacja matrycy i przycisków (jak poprzednio)…
  pinMode(dataPin,  OUTPUT);
  pinMode(shiftPin, OUTPUT);
  pinMode(storePin, OUTPUT);

  pinMode(BTN_UP,    INPUT_PULLUP);
  pinMode(BTN_DOWN,  INPUT_PULLUP);
  pinMode(BTN_LEFT,  INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  
  prevUp    = digitalRead(BTN_UP);
  prevRight = digitalRead(BTN_RIGHT);
  prevDown  = digitalRead(BTN_DOWN);
  prevLeft  = digitalRead(BTN_LEFT);

  // rozpocznij Serial
  Serial.begin(9600);
  Serial.println("=== Snake Debug Start ===");

  // ustawienia początkowe węża i jedzenia
  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 3; snakeY[1] = 3;
  snakeX[2] = 3; snakeY[2] = 2;
  generateFood();
}

void loop() {
  unsigned long deadline = lastMove + moveDelay;

  // 1) między ruchami zbieramy input i odświeżamy matrycę
  while (millis() < deadline) {
    sampleButtons();
    refreshDisplay();
  }

  // 2) po collectingu — przejmij ostatnią krawędź:
  if (lastEdgeDir >= 0) {
    int candidate = lastEdgeDir;
    lastEdgeDir = -1;

    // blokada cofania jak wcześniej
    if (candidate != dirCurrent) {
      dirNew = candidate;

      // jedyny moment, gdy drukujemy
      Serial.print(">> Next dir = ");
      switch (dirNew) {
        case 0: Serial.println("UP");    break;
        case 1: Serial.println("RIGHT"); break;
        case 2: Serial.println("DOWN");  break;
        case 3: Serial.println("LEFT");  break;
      }
    }
  }

  // 3) wykonaj ruch
  lastMove = deadline;
  updateGame();
}



void sampleButtons() {
  int u = digitalRead(BTN_UP);
  int r = digitalRead(BTN_RIGHT);
  int d = digitalRead(BTN_DOWN);
  int l = digitalRead(BTN_LEFT);

  // wykrycie LOW→HIGH i zapamiętanie kierunku
  if (u == HIGH && prevUp    == LOW && dirCurrent != 2) lastEdgeDir = 0;
  if (r == HIGH && prevRight == LOW && dirCurrent != 3) lastEdgeDir = 1;
  if (d == HIGH && prevDown  == LOW && dirCurrent != 0) lastEdgeDir = 2;
  if (l == HIGH && prevLeft  == LOW && dirCurrent != 1) lastEdgeDir = 3;

  prevUp    = u;
  prevRight = r;
  prevDown  = d;
  prevLeft  = l;
}


void updateGame() {
  dirCurrent = dirNew;

  // przesuwanie węża
  for (int i = snakeLen - 1; i > 0; i--) {
    snakeX[i] = snakeX[i-1];
    snakeY[i] = snakeY[i-1];
  }
  // ruch głowy
  switch (dirCurrent) {
    case 0: snakeY[0]--; break;
    case 1: snakeX[0]--; break;
    case 2: snakeY[0]++; break;
    case 3: snakeX[0]++; break;
  }

  // debug: pozycja głowy
  Serial.print("Head at (");
  Serial.print(snakeX[0]);
  Serial.print(", ");
  Serial.print(snakeY[0]);
  Serial.println(")");

  // kolizja ze ścianą
  if (snakeX[0] < 0 || snakeX[0] >= 8 ||
      snakeY[0] < 0 || snakeY[0] >= 8) {
    Serial.println(">> Collision: wall");
    resetGame();
    return;
  }
  // kolizja z ciałem
  for (int i = 1; i < snakeLen; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      Serial.print(">> Collision: self at segment ");
      Serial.println(i);
      resetGame();
      return;
    }
  }
  // jedzenie?
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLen++;
    Serial.print(">> Ate food. New length = ");
    Serial.println(snakeLen);
    generateFood();
  }
}

void generateFood() {
  bool ok;
  do {
    ok = true;
    foodX = random(0,8);
    foodY = random(0,8);
    for (int i = 0; i < snakeLen; i++) {
      if (foodX == snakeX[i] && foodY == snakeY[i]) {
        ok = false;
        break;
      }
    }
  } while (!ok);
}

void resetGame() {
  snakeLen = 3;
  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 3; snakeY[1] = 3;
  snakeX[2] = 3; snakeY[2] = 2;
  dirCurrent = dirNew = 2;
  generateFood();
}

void refreshDisplay() {
  // wyczyść bufor
  for (int i = 0; i < 8; i++) frameBuffer[i] = 0;

  // narysuj węża
  for (int i = 0; i < snakeLen; i++) {
    frameBuffer[ snakeY[i] ] |= (1 << snakeX[i]);
  }
  // narysuj jedzenie
  frameBuffer[ foodY ] |= (1 << foodX);

  // wyślij przez shift‑registro do matrycy
  for (int row = 0; row < 8; row++) {
    digitalWrite(storePin, LOW);
    shiftOut(dataPin, shiftPin, LSBFIRST, frameBuffer[row]);
    shiftOut(dataPin, shiftPin, LSBFIRST, rowMask[row]);
    digitalWrite(storePin, HIGH);
    delayMicroseconds(300);
  }
}
