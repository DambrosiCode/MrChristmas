#include "mp3tf16p.h"
#include <elapsedMillis.h>

#define BUTTON_PIN 2

boolean s = true;

int track_number = 1;
int max_tracks = 3;

const int next_btn = 2;
int btn_state = 0;
int last_btn_state = 0;

MP3Player mp3(10, 11);

// Timing variables
elapsedMillis btnDebounceTimer;
elapsedMillis playbackCheckTimer;
const unsigned long debounceInterval = 50; // debounce time in ms
const unsigned long playbackCheckInterval = 100; // time to check playback status

void musicSetup() {
  // Setup mp3
  Serial.begin(9600);
  mp3.initialize();
  max_tracks = mp3.getTotalTrackCount(); // Read total track count from SD card
  max_tracks = mp3.getTotalTrackCount(); // Why do I need to do this twice? 

  // Initialize the pushbutton pin as an input:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void play_next() {
  Serial.println("Playing Next Song");
  track_number++;
  Serial.println(track_number);
  if (track_number > max_tracks) {
    track_number = 1;
  }

  // Play the next track, non-blocking
  mp3.playTrackNumber(track_number, 20, false);
}


void musicLoop() {
  // Button press check for skipping tracks
  byte btn_state = digitalRead(BUTTON_PIN);
  Serial.println(btn_state);
  if (btn_state == LOW || s==true) {
    play_next();
    s=false;
  }
  // Check if the current track has finished
  if (mp3.playCompleted()) {
    play_next();
  }
}
