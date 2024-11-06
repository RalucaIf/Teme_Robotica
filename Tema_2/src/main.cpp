#include <Arduino.h>

// constants
#define BAUD_RATE 115200
#define BTN_START_STOP 3      
#define BTN_SELECT_DIFF 2       
#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 4

const unsigned long debounceDelay = 50;
const uint8_t secondsPerDifficulty[3] = {7, 6, 5}; 

// difficulties and LED colors
enum GameDifficulty { UNKNOWN = -1, EASY = 0, MEDIUM, HARD };
enum LedColor { COLOR_RED, COLOR_GREEN, COLOR_WHITE };

// dictionary
const char* wordList[] = {
  "advice", "horizon", "message", "reality", "dramatic", "strong",
  "progress", "treatment", "nature", "adventure", "technology", "tricky",
  "grief", "difficult", "challenge"
};
const uint8_t wordCount = sizeof(wordList) / sizeof(wordList[0]);

// state variables
volatile bool isRoundActive = false;
volatile bool isCountdownActive = false;
volatile bool shouldChangeWord = true;
volatile bool wordTypedCorrectly = false;

GameDifficulty gameDifficulty = UNKNOWN;
uint8_t correctWords = 0;
const char* currentWord = nullptr;
String playerInput = "";

// timers and counters
unsigned long lastDebounceStartStopTime = 0;
unsigned long lastDebounceDiffTime = 0;
unsigned long countdownStartTime = 0;
unsigned long lastCountdownToggleTime = 0;
unsigned long roundStartTime = 0;
unsigned long wordStartMillis = 0;
int countdownRemaining = 3; 

// function declarations
void setRGBLed(LedColor color);
void handleStartStop();
void handleCycleDifficulty();
void manageCountdown();
void startRound();
void stopRound();
void manageRound();
void clearSerialBuffer();
const char* generateWord();
void resetGameState();

// set RGB LED to a specified color
void setRGBLed(LedColor color) {
  // turn off all LEDs initially
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  
  // set LED based on the specified color
  switch (color) {
    case COLOR_RED:
      digitalWrite(LED_RED, HIGH);
      break;
    case COLOR_GREEN:
      digitalWrite(LED_GREEN, HIGH);
      break;
    case COLOR_WHITE:
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, HIGH);
      break;
  }
}

// start/stop button handler 
void handleStartStop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastDebounceStartStopTime > debounceDelay) { // debouncing
    lastDebounceStartStopTime = currentMillis;
    // check if the round is active
    if (isRoundActive) {
      stopRound();  // stop the round if it is currently active
    } else if (!isRoundActive && !isCountdownActive && gameDifficulty != UNKNOWN) {
      // start countdown if no round is active and difficulty is set
      isCountdownActive = true;
      countdownStartTime = millis();
      lastCountdownToggleTime = countdownStartTime;  // initialize toggle timer
      countdownRemaining = 3;
      Serial.println("Countdown...");
    }
  }
}

// difficulty button handler
void handleCycleDifficulty() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastDebounceDiffTime > debounceDelay && !isRoundActive && !isCountdownActive) {
    lastDebounceDiffTime = currentMillis;
    gameDifficulty = static_cast<GameDifficulty>((gameDifficulty + 1) % 3);
    Serial.println(gameDifficulty == EASY ? "Easy mode on!" : gameDifficulty == MEDIUM ? "Medium mode on!" : "Hard mode on!");
  }
}

// countdown with LED blinking 
void manageCountdown() {
  unsigned long currentMillis = millis();
  
  // toggle the white LED every 500 ms to create a blinking effect
  if ((currentMillis - lastCountdownToggleTime) >= 500) {
    lastCountdownToggleTime = currentMillis;
    // toggle the white LED state
    bool ledState = digitalRead(LED_RED) && digitalRead(LED_GREEN) && digitalRead(LED_BLUE);  // check if white is on
    if (ledState) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
    } else {
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_BLUE, HIGH);
    }
  }

  // countdown on terminal
  if ((currentMillis - countdownStartTime) >= (3000 - countdownRemaining * 1000)) {
    Serial.println(countdownRemaining);
    countdownRemaining--;
    if (countdownRemaining == 0) {
      startRound();
    }
  }
}

// start round function
void startRound() {
  setRGBLed(COLOR_GREEN);  // set LED to green for round start
  isCountdownActive = false;
  isRoundActive = true;
  shouldChangeWord = true;
  roundStartTime = millis();
  playerInput = "";
  correctWords = 0;
  clearSerialBuffer();
  Serial.println("Round started! Type: ");
}

// generating words with random() function
const char* generateWord() {
  return wordList[random(wordCount)];
}

// round flow
void manageRound() {
  if ((millis() - roundStartTime) >= 30000) { // 30s for one round
    stopRound();
    return;
  }

  if (shouldChangeWord) {
    currentWord = generateWord();
    Serial.println(String("\nNew Word: ") + currentWord);
    wordStartMillis = millis();
    shouldChangeWord = false;
  }
  
  if ((millis() - wordStartMillis) >= secondsPerDifficulty[gameDifficulty] * 1000 || wordTypedCorrectly) {
    wordTypedCorrectly = false;
    shouldChangeWord = true;
    playerInput = ""; // reset input for new word
  } else {
    if (Serial.available()) {
      char inputChar = Serial.read();
      
      if (inputChar == '\b' && playerInput.length() > 0) {  // handle backspace
        playerInput.remove(playerInput.length() - 1);  // remove last character from input
        Serial.print("\r");                  // move cursor to start of line
        Serial.print("                          ");  // print spaces to clear
        Serial.print("\r");  // return to start of line
        Serial.print(playerInput);  // re-print current input
      } else { 
        playerInput += inputChar;   // append character to input
      }
      // if the full word is matched, mark word as correct and increase count
      if (strncmp(playerInput.c_str(), currentWord, playerInput.length()) == 0) {
        setRGBLed(COLOR_GREEN);  // green - correct 
      } else {
        setRGBLed(COLOR_RED);  // red - incorrect
      }

      if (playerInput.equals(currentWord)) {
        wordTypedCorrectly = true;
        correctWords++; // increment correct words
      }
    }
  }
}

// stop round
void stopRound() {
  isRoundActive = false;
  Serial.print("\nRound over! Correct words: ");
  Serial.println(correctWords);
  setRGBLed(COLOR_WHITE);  // return LED to white
}

// 
void clearSerialBuffer() {
  while (Serial.available()) Serial.read();
}

// reset game state
void resetGameState() {
  isRoundActive = false;
  isCountdownActive = false;
  correctWords = 0;
  setRGBLed(COLOR_WHITE);  // white LED again
}

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BTN_START_STOP, INPUT_PULLUP);
  pinMode(BTN_SELECT_DIFF, INPUT_PULLUP);
  setRGBLed(COLOR_WHITE);  // start with white LED

  // interrupts to buttons
  attachInterrupt(digitalPinToInterrupt(BTN_START_STOP), handleStartStop, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_SELECT_DIFF), handleCycleDifficulty, FALLING);

  randomSeed(analogRead(0)); 
  resetGameState();
}

void loop() {
  if (isCountdownActive) manageCountdown();
  else if (isRoundActive) manageRound();
}
