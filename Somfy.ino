/*
 * Modul to controll a somfy Telis 4 RTS remote control via an Arduino.
 */

// Remote control button 
int myBtnPin = 3;
int channelSelBtnPin = 4;
int upBtnPin = 5;
int downBtnPin = 6;

// Home automation input commands
int activePin = 8;

boolean lastCmdActive = false;

// Timings [milli seconds]
unsigned long lastChannelSwitchTime;
int btnPressureTime = 100;
unsigned long minTimeBetweenChannelSwitching = 5000;

/**
 * Initialize variables, pin modes, etc.
 */
void setup() {
  Serial.begin(9600);
  
  // input
  pinMode(activePin, INPUT);
  
  // output
  pinMode(myBtnPin, OUTPUT);
  pinMode(upBtnPin, OUTPUT);
  pinMode(downBtnPin, OUTPUT);
  pinMode(channelSelBtnPin, OUTPUT);
  
  // timings
  lastChannelSwitchTime = millis();
}

/**
 * Main loop
 */
void loop() {
  if (digitalRead(activePin) == HIGH &! lastCmdActive) {
    lastCmdActive = true;
    //switchChannel(1);
    pressButton(downBtnPin);
  } else if (digitalRead(activePin) == LOW) {
    lastCmdActive = false;
  }
}

/**
 * Switch remote control channel in the following order:
 * - channel 1
 * - channel 2
 * - channel 3
 * - channel 4
 * - all channels
 * 
 * First pressure on the button shows the actual active channel, 
 * seconde one switch to the next channel.
 *
 * int times: how many channel switching events should be emitted
 */
void switchChannel(int times) {
  if (lastChannelSwitchTime + minTimeBetweenChannelSwitching < millis()) {
    lastChannelSwitchTime = millis();
    int var = 0; 
    while(var < times + 1){
      pressButton(channelSelBtnPin);    
      var++;
    }
  }
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
