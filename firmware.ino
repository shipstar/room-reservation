/* Pins configuration */
int buttonInput = D3;  // button
int buttonPressLED = D7; // led
int roomStatusLED = D6; // led

/* Button and led logics */
int roomOccupied = 0;
int buttonPressLEDState = HIGH;     // current state of the output pin
int currentButtonReading;          // current reading from the input pin
int previousButtonReading = LOW;   // previous reading from the input pin
long lastToggleTime = 0;           // the last time the output pin was toggled
long debounce = 200;  // the debounce time, increase if the output flickers
long ledDuration = 2000;  // duration LED should stay lit after press

int speakerOut = A0;
void setup() {
    pinMode(buttonInput, INPUT);
    pinMode(buttonPressLED, OUTPUT);
    pinMode(roomStatusLED, OUTPUT);
    pinMode(speakerOut, OUTPUT);
}

void loop() {
    currentButtonReading = digitalRead(buttonInput);  // read the button state

    maybeTurnOffButtonPressLED();

    if (millis() - lastToggleTime > debounce) {
        // We have waited long enough to rule out this being random power fluctuation
        if (currentButtonReading == HIGH && previousButtonReading == LOW) {
            handleButtonPress();
        }
    }

    previousButtonReading = currentButtonReading;
}

void maybeTurnOffButtonPressLED() {
    if (buttonPressLEDState == HIGH && millis() - lastToggleTime > ledDuration) {
        buttonPressLEDState = LOW;
        digitalWrite(buttonPressLED, LOW);
    }
}

void handleButtonPress() {
    digitalWrite(buttonPressLED, HIGH);
    buttonPressLEDState = HIGH;
    lastToggleTime = millis();

    if (roomOccupied == 1) {
        Particle.publish("room_occupied", "NOT_OCCUPIED", 60, PRIVATE);
        digitalWrite(roomStatusLED, LOW);
        roomOccupied = 0;
    } else {
        Particle.publish("room_occupied", "OCCUPIED", 60, PRIVATE);
        digitalWrite(roomStatusLED, HIGH);
        roomOccupied = 1;
    }

}
