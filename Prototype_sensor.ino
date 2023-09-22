const int MOTION_SENSOR_PIN = 14;   // Arduino-pin som er koblet til MOTION_SENSOR_PIN
int motionStateCurrent      = LOW; // nåværende tilstand til motion-sensor-pinnen
int motionStatePrevious     = LOW; // forrige tilstand til motion-sensor-pinnen

bool showX = false;

//instansvariabler for LED
// 2-dimensjonal array av rad-pinnumre:
const int row[8] = {
  2, 7, 19, 5, 13, 18, 12, 16
};

// 2-dimennsjonal array av kolonno-pinnumre:
const int col[8] = {
  6, 11, 10, 3, 17, 4, 8, 9
};

// 2-dimensional array av piksler
int pixels[8][8];

typedef bool charMapType[8][8];
// LED Kryss kode, 1 for LED på, 0 for LED av
const charMapType signal = {
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1}
};

void setup() {
  Serial.begin(9600);                // Initialiser serial
  pinMode(MOTION_SENSOR_PIN, INPUT); // setter arduino pin til input

// Setter LED pin i rad og kol til å være output
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);
   
  }
  setupMatrix();
}



void loop() {
  // hvis showX er true så vises X
  if (showX){
    displayLedPattern();
  }

  motionStatePrevious = motionStateCurrent;             // Lagre forrige tilstand
  motionStateCurrent  = digitalRead(MOTION_SENSOR_PIN); // les ny tilstand

  if (motionStatePrevious == LOW && motionStateCurrent == HIGH) { // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    showX = true;
  }
  else if (motionStatePrevious == HIGH && motionStateCurrent == LOW) { // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    showX = false;
  }
}

void displayLedPattern(){
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    digitalWrite(row[thisRow], HIGH);
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      // hent tilstand til gjeldende piksel
      int thisPixel = pixels[thisRow][thisCol];
      // når row er HIGH og col er LOW,
      // Slås LED på
      digitalWrite(col[thisCol], thisPixel);
      // Slå av pikselen
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], HIGH);
      }
    }
    // Sett rad-pin til LOW får å slå av hele raden
    digitalWrite(row[thisRow], LOW);
  }
}

// går gjennom hver LED på 8x8 matrix og sjekker boolean value
void setupMatrix(){
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      bool v = signal[x][y];
      if(v){
        pixels[x][y] = LOW;
      }else{
        pixels[x][y] = HIGH;
      }
    }
  }
}








