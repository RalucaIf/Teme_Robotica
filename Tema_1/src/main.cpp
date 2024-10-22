#include <Arduino.h>

#define PIN_ROSU 6
#define PIN_VERDE 5

#define LED_ALBASTRU_1 7
#define LED_ALBASTRU_4 10

#define START_BUTON 3  
#define STOP_BUTON 2  

// variabile pentru debouncing 
int startButtonState = HIGH, lastStartButtonState = HIGH;
int stopButtonState = HIGH, lastStopButtonState = HIGH;
unsigned long lastDebounceTimeStart = 0, lastDebounceTimeStop = 0;
const unsigned long debounceDelayStart = 50, debounceDelayStop = 1000;

// flags
bool isCharging = false;
bool stopChargingFlag = false;
bool chargingStopped = false;

// timer pentru butonul apasat
unsigned long stopButtonPressTime = 0;

// starile butoanelor si debouncing
void handleButtonPresses() {
  // buton pt start
  int readingStart = digitalRead(START_BUTON);
  if (readingStart != lastStartButtonState) {
    lastDebounceTimeStart = millis();
  }
  if ((millis() - lastDebounceTimeStart) > debounceDelayStart) {
    if (readingStart != startButtonState) {
      startButtonState = readingStart;
      if (startButtonState == LOW && !isCharging) {  // incepe incarcarea daca nu functioneaza deja
        isCharging = true;  // setare flag
      }
    }
  }
  lastStartButtonState = readingStart;

  // buton de stop
  int readingStop = digitalRead(STOP_BUTON);
  if (readingStop != lastStopButtonState) {
    lastDebounceTimeStop = millis();
    if (readingStop == LOW) {
      stopButtonPressTime = millis(); // durata apasarii butonului
    }
  }
  if ((millis() - lastDebounceTimeStop) > debounceDelayStop) {
    if (readingStop == LOW && (millis() - stopButtonPressTime > 1000)) {
      stopChargingFlag = true; // flag pentru oprirea incarcarii
    }
  }
  lastStopButtonState = readingStop;
}

// setarea led-ului RGB
void setRGBState(bool isCharging) {
  if (isCharging) {
    digitalWrite(PIN_ROSU, HIGH);   // rosu daca se incarca
    digitalWrite(PIN_VERDE, LOW);   
  } else {
    digitalWrite(PIN_ROSU, LOW);    // verde daca statia este libera
    digitalWrite(PIN_VERDE, HIGH);  
  }
}

// setarea led-urilor pe modul OFF
void resetChargingLEDs() {
  for (int led = LED_ALBASTRU_1; led <= LED_ALBASTRU_4; led++) {
    digitalWrite(led, LOW);
  }
}

// aprinderea led-urilor odata cu terminarea incarcarii 
void blinkChargingCompletion() {
  for (int i = 0; i < 3; i++) {
    resetChargingLEDs();
    delay(500);
    for (int led = LED_ALBASTRU_1; led <= LED_ALBASTRU_4; led++) {
      digitalWrite(led, HIGH);
    }
    delay(500);
  }
  resetChargingLEDs(); // stingerea LED-urilor dupa blinking 
}

// functie pentru incarcare
void chargingProcess() {
  chargingStopped = false;
  stopChargingFlag = false; // resetare flag inaintea incarcarii 

  for (int led = LED_ALBASTRU_1; led <= LED_ALBASTRU_4 && !chargingStopped; led++) {
    // LED-ul se aprinde de 3 ori 
    for (int blink = 0; blink < 3 && !chargingStopped; blink++) {
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
      handleButtonPresses(); // verificare buton de stop in timpul blinking-ului
      if (stopChargingFlag) {
        chargingStopped = true; // daca butonul de stop este apasat, se opreste incarcarea
      }
    }
    digitalWrite(led, HIGH); // LED-ul ramane aprins dupa blinking
  }
}

// functie pentru inceperea incarcarii
void startCharging() {
  setRGBState(true); // se incarca - RGB este setat pe rosu
  chargingProcess(); // se executa incarcarea
  
  if (!chargingStopped) {
    blinkChargingCompletion(); // LED-urile fac blinking daca incarcarea s-a terminat
  } 
  
  resetChargingLEDs(); // resetarea LED-urilor dupa terminarea incarcarii
  setRGBState(false);  // led-ul RGB setat pe free - verde 
  isCharging = false;  // flag-ul este resetat pe false dupa terminarea incarcarii
}

// functie pentru initializarea pinilor si a sistemului 
void setup() {
  Serial.begin(9600);
  
  // pinii setati pentru output
  for (int pin = 4; pin <= 10; pin++) {
    pinMode(pin, OUTPUT);
  }

  pinMode(START_BUTON, INPUT_PULLUP);
  pinMode(STOP_BUTON, INPUT_PULLUP);

  // starea initiala este free - verde
  setRGBState(false);
  resetChargingLEDs();
}

// incarcarea si apasarea butoanelor
void loop() {
  handleButtonPresses();  // gestionarea apasarii butoanelor

  if (isCharging) {
    startCharging(); // incepe incarcarea
  }
}

