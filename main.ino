// main.ino

// Declare setup and loop functions for motor and light control
void motorSetup();
void motorLoop();
void lightSetup();
void lightLoop();
void musicSetup();
void musicLoop();

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:

  motorSetup();  // Initialize motor control
  lightSetup();  // Initialize light control
  musicSetup();
}

void loop() {
  motorLoop();  // Rotate motor
  lightLoop();  // Control LEDs
  musicLoop();
}
