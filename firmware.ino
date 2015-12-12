/* Pins configuration */
int inPin = D3;  // button
int buttonPressLED = D7; // led
int roomStatusLED = D6; // led

/* Button and led logics */
bool roomOccupied = FALSE;
int buttonPressLEDState = HIGH;     // current state of the output pin
int currentButtonReading;          // current reading from the input pin
int previousButtonReading = LOW;   // previous reading from the input pin
long lastToggleTime = 0;           // the last time the output pin was toggled
long debounce = 200;  // the debounce time, increase if the output flickers
long ledDuration = 2000;  // duration LED should stay lit after press

void setup() {
    pinMode(inPin, INPUT);    // button pin setup
    pinMode(buttonPressLED, OUTPUT);  // led pin setup
    pinMode(roomStatusLED, OUTPUT);  // led pin setup
}

void loop() {
    currentButtonReading = digitalRead(inPin);  // read the button state

    if (buttonPressLEDState == HIGH && millis() - lastToggleTime > ledDuration) {
        buttonPressLEDState = LOW;
        digitalWrite(buttonPressLED, LOW);
    }

    if (millis() - lastToggleTime > debounce) {
        // We have waited long enough to rule out this being random power fluctuation
        if (currentButtonReading == HIGH && previousButtonReading == LOW) {
            handleButtonPress();
        }
    }

    previousButtonReading = currentButtonReading;
}

void handleButtonPress() {
    if (roomOccupied) {
        Spark.publish("room_occupied", "NOT_OCCUPIED", 60, PRIVATE);
        digitalWrite(roomStatusLED, LOW);
        roomOccupied = FALSE;
    } else {
        Spark.publish("room_occupied", "OCCUPIED", 60, PRIVATE);
        digitalWrite(roomStatusLED, HIGH);
        roomOccupied = TRUE;
    }

    digitalWrite(buttonPressLED, HIGH);
    buttonPressLEDState = HIGH;
    lastToggleTime = millis();
}
