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

/* Piezo buzzer notes */
// Definitions: https://www.arduino.cc/en/Tutorial/toneMelody
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
// Define a special note, 'R', to represent a rest
#define NOTE_R     0

int speakerOut = A0;
// MELODY and TIMING
//  melody[] is an array of notes, accompanied by beats[],
//  which sets each note's relative length (higher #, longer note)
// Theme from Cheers
// Transcribed from: http://www.musicnotes.com/sheetmusic/mtd.asp?ppn=MN0018004
int melodyCheersTheme[] = {
    // Measure 1
    NOTE_C5,
    NOTE_E4,
    NOTE_G4,
    NOTE_C5,
    NOTE_E5,
    // Measure 2
    NOTE_D5,
    NOTE_R,
    NOTE_D5,
    NOTE_R
};
int melodyCheersThemeLength = 9;
int beatsCheersTheme[]  = {
    // Measure 1
    32,
    8,
    8,
    8,
    8,
    // Measure 2
    14,
    2,
    32,
    16
};
// ms per beat. 118bpm. Whole note is 16 beats in our setup.
long tempoCheersTheme = 31;  // ms per "beat"
// Final countdown
// Transcribed from: http://www.musicnotes.com/sheetmusic/mtd.asp?ppn=MN0129095
int melodyFinalCountdown[] = {
    // Measure 1
    NOTE_E4,
    NOTE_D4,
    NOTE_E4,
    NOTE_A3,
    // Measure 2
    NOTE_R,
    NOTE_F4,
    NOTE_E4,
    NOTE_F4,
    NOTE_E4,
    NOTE_D4,
    // Measure 3
    NOTE_R,
    NOTE_F4,
    NOTE_E4,
    NOTE_F4,
    NOTE_A3,
    // Measure 4
    NOTE_R,
    NOTE_D4,
    NOTE_C4,
    NOTE_D4,
    NOTE_C4,
    NOTE_B3,
    NOTE_D4,
    // Measure 5
    NOTE_C4,
    NOTE_R
};
int melodyFinalCountdownLength = 24;
int beatsFinalCountdown[]  = {
    // Measure 1
    4,
    4,
    16,
    16,
    // Measure 2
    24,
    4,
    4,
    8,
    8,
    16,
    // Measure 3
    24,
    4,
    4,
    16,
    16,
    // Measure 4
    24,
    4,
    4,
    8,
    8,
    8,
    8,
    // End
    16,
    16
};
// ms per beat. 118bpm. Whole note is 16 beats in our setup.
long tempoFinalCountdown = 31;  // ms per "beat"

// Windows Shutdown Song
// G#4 D#4 G#3 A#3
int melodyXPShutdown[] = {
  // measure 1
  NOTE_GS5,
  NOTE_DS5,
  NOTE_GS4,
  NOTE_AS4,
  // end
  NOTE_R
};
int melodyXPShutdownLength = 5;
int beatsXPShutdown[]  = {
  // measure 1
  8,
  8,
  8,
  24,
  // end
  16
};
long tempoXPShutdown = 40;

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
        playMelody(melodyXPShutdown, beatsXPShutdown, melodyXPShutdownLength, tempoXPShutdown);
        // playMelody(melodyFinalCountdown, beatsFinalCountdown, melodyFinalCountdownLength, tempoFinalCountdown);
    } else {
        Particle.publish("room_occupied", "OCCUPIED", 60, PRIVATE);
        digitalWrite(roomStatusLED, HIGH);
        roomOccupied = 1;
        playMelody(melodyCheersTheme, beatsCheersTheme, melodyCheersThemeLength, tempoCheersTheme);
    }

}

void playMelody(int melody[], int beats[], int melodyLength, int tempo) {
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
