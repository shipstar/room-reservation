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

/* Peizo buzzer notes */
#define  NOTE_c     261
#define  NOTE_d     294
#define  NOTE_e     329
#define  NOTE_f     349
#define  NOTE_g     392
#define  NOTE_a     440
#define  NOTE_b     493
#define  NOTE_C     523
// Define a special note, 'R', to represent a rest
#define  NOTE_R     0

int speakerOut = A0;
// MELODY and TIMING
//  melody[] is an array of notes, accompanied by beats[],
//  which sets each note's relative length (higher #, longer note)
int melody[] = {
    NOTE_C,
    NOTE_b,
    NOTE_g,
    NOTE_C,
    NOTE_b,
    NOTE_e,
    NOTE_R,
    NOTE_C,
    NOTE_c,
    NOTE_g,
    NOTE_a,
    NOTE_C
};
int melodyLength = 12;
int beats[]  = { 16, 16, 16, 8, 8, 16, 32, 16, 16, 16, 8, 8 };
long tempo = 10;  // ms per "beat"
// Final countdown
int melodyFinalCountdown[] = {
    NOTE_C,
    NOTE_b,
    NOTE_g,
    NOTE_C,
    NOTE_b,
    NOTE_e,
    NOTE_R,
    NOTE_C,
    NOTE_c,
    NOTE_g,
    NOTE_a,
    NOTE_C
};
int melodyFinalCountdownLength = 12;
int beatsFinalCountdown[]  = { 16, 16, 16, 8, 8, 16, 32, 16, 16, 16, 8, 8 };
long tempoFinalCountdown = 10;  // ms per "beat"
// Initialize core variables
int currentNote = 0;
int currentNoteBeatsTarget = 0;
long currentNoteDurationTarget  = 0;

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

    playMelody();
}

void playMelody() {
    for (int i=0; i < melodyLength - 1; i++) {
        currentNote = melody[i];
        currentNoteBeatsTarget = beats[i];

        currentNoteDurationTarget = currentNoteBeatsTarget * tempo; // Set up timing

        if (currentNote > 0) { // if this isn't a Rest beat, while the tone has
            tone(speakerOut, currentNote, currentNoteDurationTarget);
        }
        delay(currentNoteDurationTarget);

        // A pause between notes...
        delay(tempo/10);
    }
}
