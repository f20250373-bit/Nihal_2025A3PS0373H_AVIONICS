
#include <LiquidCrystal_I2C.h>

// LCD

LiquidCrystal_I2C lcd(0x27, 16, 2);

// STATES

const int OPEN_SEA = 0;
const int ANCHOR_DROPPED = 1;
const int STORM = 2;
const int CHARYBDIS = 3;
const int WRECKED = 4;

int currentState = ANCHOR_DROPPED;

// PIN DEFINITIONS
const int buzzerPin = 3;

const int trigPin = 8;
const int echoPin = 7;

const int buttonPin = 4;

const int ledPin = 2;

const int lightPin = A0;

// TIMING
unsigned long dangerStartTime = 0;
unsigned long lastBlinkTime = 0;

const unsigned long WRECK_TIME = 5000;
const unsigned long BLINK_INTERVAL = 500;

bool ledState = false;

// BUTTON

bool lastButtonState = HIGH;

// SETUP

void setup() {

  pinMode(buzzerPin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buttonPin, INPUT_PULLUP);//puts the default input state to HIGH

  pinMode(ledPin, OUTPUT);

  pinMode(lightPin, INPUT);

  digitalWrite(ledPin, LOW);

  noTone(buzzerPin);

  // I2C LCD

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME!");
  lcd.setCursor(0, 1);
  lcd.print("ATHENA'S INTERN");

  delay(1500);

  //the function is coded below
}



// MAIN LOOP


void loop() {

  int lightValue = analogRead(lightPin);

  float distance = getDistance();

  // Check button
  handleButton();


 
  // WRECKED
  
  if (currentState == WRECKED) {

    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);

    displayState();

    return;
  }


  // ANCHOR DROPPED
  
  if (currentState == ANCHOR_DROPPED) {

    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);

    displayState();

    return;
  }

  // OPEN SEA

  if (currentState == OPEN_SEA) {

    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);

    // Storm

    if (lightValue < 512) {

      enterState(STORM);
    }

    // Charybdis
    else if (distance < 100) {

      enterState(CHARYBDIS);
    }
  }

  // STORM
  
  else if (currentState == STORM) {

    noTone(buzzerPin);


    // Blink LED every 500 ms

    if (millis() - lastBlinkTime >= BLINK_INTERVAL) {

      lastBlinkTime = millis();

      ledState = !ledState;

      digitalWrite(ledPin, ledState);
    }


    // Storm ended

    if (lightValue >= 512) {

      digitalWrite(ledPin, LOW);

      enterState(OPEN_SEA);
    }


    // Storm lasted 5 seconds

    else if (millis() - dangerStartTime >= WRECK_TIME) {

      digitalWrite(ledPin, LOW);

      enterState(WRECKED);
    }
  }


  
  // CHARYBDIS
  

  else if (currentState == CHARYBDIS) {

    digitalWrite(ledPin, LOW);


    // Sound buzzer

    tone(buzzerPin, 1000);


    // Ship escaped

    if (distance >= 100) {

      noTone(buzzerPin);

      enterState(OPEN_SEA);
    }


    // Danger lasted 5 seconds

    else if (millis() - dangerStartTime >= WRECK_TIME) {

      noTone(buzzerPin);

      enterState(WRECKED);
    }
  }

  displayState();
}

// STATE TRANSITION

void enterState(int newState) {
  currentState = newState;
  if (newState == STORM ||newState == CHARYBDIS) {
    dangerStartTime = millis();
  }
  if (newState == OPEN_SEA || newState == ANCHOR_DROPPED) {  // Reset timer when entering OPEN SEA
      dangerStartTime = 0;                                   // or ANCHOR DROPPEDdangerStartTime = 0;
  }
  // Reset LED timer
  lastBlinkTime = millis();
  ledState = false;
}

// BUTTON

void handleButton() {
  bool buttonState = digitalRead(buttonPin);


  // Detect new button press

  if (lastButtonState == HIGH && buttonState == LOW) {
    if (currentState == ANCHOR_DROPPED) {

      enterState(OPEN_SEA);
    }
    else if (currentState != WRECKED) {     // Otherwise drop the anchor.This also works during STORM AND CHARYBDIS
      enterState(ANCHOR_DROPPED);      
    }
  }
  lastButtonState = buttonState;

  delay(30);
}



// ULTRASONIC SENSOR

float getDistance() {

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);


  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);


  long duration = pulseIn(
    echoPin,
    HIGH,
    30000
  );

  // No echo

  if (duration == 0) {

    return 999;
  }

  // Calculate distance in cm

  float distance =
    duration * 0.0343 / 2;//formula in google


  return distance;
}

void displayState() {

  static int lastDisplayedState = -1;

  // Don't refresh LCD unnecessarily

  if (currentState == lastDisplayedState) {

    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STATE:");
  lcd.setCursor(0, 1);

  if (currentState == OPEN_SEA) {

    lcd.print("OPEN SEA");
  }

  else if (currentState == ANCHOR_DROPPED) {

    lcd.print("ANCHOR DROPPED");
  }

  else if (currentState == STORM) {

    lcd.print("STORM");
  }

  else if (currentState == CHARYBDIS) {

    lcd.print("CHARYBDIS");
  }

  else if (currentState == WRECKED) {

    lcd.print("WRECKED");
  }

  lastDisplayedState = currentState;//update
  delay(500);
}