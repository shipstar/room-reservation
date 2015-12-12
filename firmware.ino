/* Pins configuration */
int inPin = D3;  // button
int outPin = D7; // led
int outPin2 = D6; // led

/* Button and led logics */
int state = HIGH;     // current state of the output pin
int reading;          // current reading from the input pin
int previous = LOW;   // previous reading from the input pin
long lastToggleTime = 0;           // the last time the output pin was toggled
long debounce = 200;  // the debounce time, increase if the output flickers
long ledDuration = 2000;  // duration LED should stay lit after press

void setup() {
    pinMode(inPin, INPUT);    // button pin setup
    pinMode(outPin, OUTPUT);  // led pin setup
    pinMode(outPin2, OUTPUT);  // led pin setup
}

void loop() {
    reading = digitalRead(inPin);  // read the button state

    if (state == HIGH && millis() - lastToggleTime > ledDuration) {
        state = LOW;
        digitalWrite(outPin, LOW);
        digitalWrite(outPin2, LOW);
    }
    if (millis() - lastToggleTime > debounce) {
        // We have waited long enough to rule out this being random power fluctuation
        if (reading == HIGH && previous == LOW) {
            // If the input just went from LOW and HIGH and we've waited long enough to ignore
            // any noise on the circuit, toggle the output pin and remember the time
            Spark.publish("button_state", "pressed", 60, PRIVATE);
            digitalWrite(outPin, HIGH);
            digitalWrite(outPin2, HIGH);
            state = HIGH;

            lastToggleTime = millis();
        }
    }

    previous = reading;
}
