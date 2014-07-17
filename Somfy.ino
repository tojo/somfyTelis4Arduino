/*
 * Modul to controll a Somfy Telis 4 RTS 
 * remote control via an Arduino and serial 
 * communication to send commands.
 */
#include <SerialCommand.h>

// Serial protocol command object
SerialCommand SCmd;

// Telis 4 remote control buttons 
int myBtn = 3;
int chSelBtn = 4;
int upBtn = 5;
int downBtn = 6;

// Telis 4 remote control leds
int chOneLed = 12;
int chFourLed = 13;

int activeCh;

// Timings [milli seconds]
unsigned long lastchSwitchTime;
int btnPressureTime = 100;
unsigned long minTimeBetweenchSwitching = 5000;

/**
 * Initialize variables, pin modes, etc.
 */
void setup() {
  Serial.begin(9600);
  
  // input
  pinMode(chOneLed, INPUT);
  pinMode(chFourLed, INPUT);
  
  // output
  pinMode(myBtn, OUTPUT);
  pinMode(upBtn, OUTPUT);
  pinMode(downBtn, OUTPUT);
  pinMode(chSelBtn, OUTPUT);
  
  // timings
  lastchSwitchTime = millis();

  initActiveCh();

  // Setup callbacks for SerialCommand commands 
  SCmd.addCommand("ACTIVE",printActiveCh);
  SCmd.addCommand("UP",cmdUp);
  SCmd.addCommand("MY",cmdMy);
  SCmd.addCommand("DOWN",cmdDown);
  SCmd.addCommand("SWITCH",cmdSwitchch);
  SCmd.setDefaultHandler(cmdUnrecognized);
  Serial.println("Ready"); 
}

/**
 * Initialize activeCh variable and switch the 
 * somfy remote to the corresponding selection.
 */
void initActiveCh() {
  Serial.println("initActiveCh");
  while (!isChOneSelected()) {
    switchch(1);
  }
  activeCh = 1;
}

/**
 * Main loop
 */
void loop() {
  SCmd.readSerial(); // process serial commands
}

/**
 * TODO: add documentation
 */
void cmdSwitchch() {
  int chNumber;
  char *ch = SCmd.next(); 
  if (ch != NULL) {
    chNumber=atoi(ch);
    switchch(chNumber);
  }
}

/**
 * TODO: add documentation
 */
void cmdUp() {
  cmdSwitchch();
  pressButton(upBtn);
}

/**
 * TODO: add documentation
 */
void cmdMy() {
  cmdSwitchch();
  pressButton(myBtn);
}

/**
 * TODO: add documentation
 */
void cmdDown() {
  cmdSwitchch();
  pressButton(downBtn);
}

/** 
 * This gets set as the default handler, 
 * and gets called when no other command matches. 
 */
void cmdUnrecognized(const char *command) {
  Serial.print("Valid commands: 'ACTIVE', 'UP $ch'"); 
  Serial.println(", 'DOWN $ch', 'MY $ch', 'SWITCH $ch'"); 
}

/**
 * Checks the selected ch of the somfy remote and
 * returns true if the ch one is active.
 */
boolean isChOneSelected() {
  pressButton(chSelBtn); // let blink the leds
  boolean chOneInput = digitalRead(chOneLed);
  boolean chFourInput = digitalRead(chFourLed);
  return chOneInput && !chFourInput;
}

/**
 * Switch remote control to the given channel.
 *
 * The remote supports the following channel
 * - ch 1
 * - ch 2
 * - ch 3
 * - ch 4
 * - ch 5 -> all channel together
 *  
 * First pressure on the button shows the actual
 * active channel, seconde one switch to the 
 * next channel.
 */
void switchch(int ch) {
  if (lastchSwitchTime + minTimeBetweenchSwitching < millis()) {
    int times;
    if (ch > activeCh) {
      times = ch - activeCh;
    } else {
      // example: activeCh = 4, ch = 2 -> times must be 3
      times = 5 - activeCh + ch;
    }
    lastchSwitchTime = millis();
    int var = 0; 
    while(var < times + 1){
      pressButton(chSelBtn);    
      var++;
    }
    activeCh = ch;
    printActiveCh();
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

/**
 * Print the active ch to Serial output for
 * debugging purposes. 
 */
void printActiveCh() {
  Serial.println(activeCh);
}
