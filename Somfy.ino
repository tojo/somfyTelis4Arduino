/*
 * Modul to controll a somfy Telis 4 RTS remote control via an Arduino.
 */

// Remote control button 
int channelSelBtnPin = 4;

// Timings
int btnPressureTime = 100; // [milli seconds]

/**
 * Initialize variables, pin modes, etc.
 */
void setup() {
  Serial.begin(9600);
  pinMode(channelSelBtnPin, OUTPUT);
}

/**
 * Main loop
 */
void loop() {
  pressButton(channelSelBtnPin);
}

/**
 * Simulate pressing a remote control button
 */
 void pressButton(int btn) {
   digitalWrite(btn, HIGH);
   delay(btnPressureTime);
   digitalWrite(btn, LOW);
   delay(btnPressureTime);
 }
