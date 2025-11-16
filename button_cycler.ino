#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define BUTTON_PIN 2   // Pin connected to the button
#define PIXEL_PIN 7    // Pin connected to the NeoPixels
#define PIXEL_COUNT 20 // Number of NeoPixels

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

boolean oldState = HIGH;
int mode = 0; // Current animation mode (0-8)

// Timers for non-blocking operations
elapsedMillis debounceTimer;
elapsedMillis animationTimer;

const int debounceDelay = 20;      // Debounce delay in milliseconds
const int animationDelay = 50;     // Delay between animation steps

// Variables for animations
int colorWipeIndex = 0;
int theaterChaseIndex = 0;
long rainbowHue = 0;

void lightSetup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin(); // Initialize NeoPixel strip object
  strip.show();  // Turn all pixels off
}

void lightLoop() {
  // Get the current button state
  boolean newState = digitalRead(BUTTON_PIN);

  // Check if state changed from HIGH to LOW (button press)
  if ((newState == LOW) && (oldState == HIGH) && debounceTimer > debounceDelay) {
    debounceTimer = 0;          // Reset debounce timer
    mode = (mode + 1) % 9;      // Cycle through modes 0-8
    resetAnimationState();      // Reset animation variables
  }

  oldState = newState; // Update the old state

  // Execute the current animation based on the mode
  if (animationTimer >= animationDelay) {
    animationTimer = 0; // Reset animation timer
    switch (mode) {
      case 0:
        colorWipe(strip.Color(255, 255, 255), false); // Black/off
        break;
      case 1:
        colorWipe(strip.Color(255, 0, 0), false); // Red
        break;
      case 2:
        colorWipe(strip.Color(0, 255, 0), false); // Green
        break;
      case 3:
        colorWipe(strip.Color(0, 0, 255), false); // Blue
        break;
      case 4:
        theaterChase(strip.Color(127, 127, 127)); // White
        break;
      case 5:
        theaterChase(strip.Color(127, 0, 0)); // Red
        break;
      case 6:
        theaterChase(strip.Color(0, 0, 127)); // Blue
        break;
      case 7:
        rainbow(); // Rainbow effect
        break;
      case 8:
        theaterChaseRainbow(); // Theater chase rainbow
        break;
    }
  }
}

void resetAnimationState() {
  // Reset variables for iterative animations
  colorWipeIndex = 0;
  theaterChaseIndex = 0;
  rainbowHue = 0;
}

// Non-blocking color wipe
void colorWipe(uint32_t color, boolean reset) {
  if (reset) colorWipeIndex = 0; // Optionally reset animation
  if (colorWipeIndex < strip.numPixels()) {
    strip.setPixelColor(colorWipeIndex, color); // Set pixel color
    strip.show();
    colorWipeIndex++; // Move to the next pixel
  }
}

// Non-blocking theater chase
void theaterChase(uint32_t color) {
  strip.clear();
  for (int i = theaterChaseIndex; i < strip.numPixels(); i += 3) {
    strip.setPixelColor(i, color); // Set every third pixel
  }
  strip.show();
  theaterChaseIndex = (theaterChaseIndex + 1) % 3; // Cycle through phases
}

// Non-blocking rainbow
void rainbow() {
  for (int i = 0; i < strip.numPixels(); i++) {
    int pixelHue = rainbowHue + (i * 65536L / strip.numPixels());
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  strip.show();
  rainbowHue = (rainbowHue + 256) % 65536; // Increment hue
}

// Non-blocking theater chase rainbow
void theaterChaseRainbow() {
  strip.clear();
  for (int i = theaterChaseIndex; i < strip.numPixels(); i += 3) {
    int pixelHue = rainbowHue + (i * 65536L / strip.numPixels());
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
  }
  strip.show();
  theaterChaseIndex = (theaterChaseIndex + 1) % 3; // Cycle through phases
  rainbowHue = (rainbowHue + 256) % 65536;         // Increment hue
}
