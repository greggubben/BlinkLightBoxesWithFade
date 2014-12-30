/*
 * This Arduino code will turn on/off a set of LEDs.
 *
 * The LED Light fade is fully configurable with ranges.
 * A random number is generated between the min and max values specified.
 * If you do not want random intervales then set the min and max to the same value.
 * The following describes the options and process:
 *
 * Assuming we are staring in an off state...
 *
 * The Lights will fade in at a random interval between fade_in_min and fade_in_max.
 *
 * Then the Lights will stay on for a random interval between on_min and on_max.
 *
 * At which point the Lights will start to fade out for a random intervale
 * between fade_out_min and fade_out_max.
 *
 * The Lights will stay off for a random interval between off_min and off_max.
 *
 * Each set of Lights also has an intial state which indicates where the Lights
 * start in the sequence described above.  If the initial state is BOX_OFF then
 * the Lights will stay off for the off random interval. If the initial state
 * is BOX_ON then the Lights will stay on for the on random interval.

 *
 * Created by: Gregg Ubben
 * Created on: 22-Oct-2013
 */

// Our own constants for defining if the Box is on or off
const unsigned int BOX_ON  = HIGH;    // Same as true
const unsigned int BOX_OFF = LOW;     // Same as false

/*********************
 * Structure to hold the Blinking Box Data
 *********************/
typedef struct BlinkBox {
  unsigned int ledPin;                // Pin the LED is attached. Must be a PWM pin 3, 5, 6, 9, 10, and 11
  
  // The durations need to include the fading.
  //     The Fade In is included in the On Duration.
  //     The Fade Out is included in the Off Duration.
  unsigned long on_duration_min;       // Minimum amount of time(ms) the light could be on
  unsigned long on_duration_max;       // Maximum amount of time(ms) the light could be on
  unsigned long off_duration_min;      // Minimum amount of time(ms) the light could be off
  unsigned long off_duration_max;      // Maximum amount of time(ms) the light could be off

  // The fade is included in the Duration defined above
  // Use 0 for no fade.
  unsigned long fade_in_min;  // Minimum amount of time(ms) the light fades to on
  unsigned long fade_in_max;  // Maximum amount of time(ms) the light fades to on
  unsigned long fade_out_min; // Minimum amount of time(ms) the light fades to off
  unsigned long fade_out_max; // Maximum amount of time(ms) the light fades to off
  
  boolean ledStatus;         // Current status of the LED true=on; false=off
  
  // Current Status of the Box
  unsigned long start;       // Start time(ms) of the current duration
  unsigned long until;       // End time(ms) of the current duration
  unsigned long fade_until;  // End Time(ms) of fading
  unsigned long fade_dur;    // Duration in Time(ms) of fading

} BlinkBox;

// The blinking boxes
// Definition: {pin, on_min, on_max, off_min, off_max, fade_in_min, fade_in_max, fade_out_min, fade_out_max, initial_state, 0, 0, 0, 0}
BlinkBox box1 = {3, 4000, 6000, 1000, 3000,  500, 1500,    0, 1000, BOX_OFF, 0, 0, 0, 0};
BlinkBox box2 = {5,  500,  500,  500,  500,    0,    0,    0,    0, BOX_ON,  0, 0, 0, 0};
BlinkBox box3 = {6, 6000, 9000, 5000, 8000,  500, 3000, 1000, 4000, BOX_OFF, 0, 0, 0, 0};
BlinkBox box4 = {9, 4000, 6000, 1000, 3000, 1000, 1500,  200,  200, BOX_OFF, 0, 0, 0, 0};

// Working Variables
unsigned long currTime;     // Current time in milliseconds


/*********************
 * Set how long to wait for the State Change
 *********************/
void setWaitUntil(struct BlinkBox *blinkBox) {
  unsigned long duration_min;
  unsigned long duration_max;
  unsigned long fade_min;
  unsigned long fade_max;

  if (blinkBox->ledStatus) {
    // Setting LED on duration
    duration_min = blinkBox->on_duration_min;
    duration_max = blinkBox->on_duration_max;
    fade_min = blinkBox->fade_in_min;
    fade_max = blinkBox->fade_in_max;
  }
  else {
    // Setting LED off duration
    duration_min = blinkBox->off_duration_min;
    duration_max = blinkBox->off_duration_max;
    fade_min = blinkBox->fade_out_min;
    fade_max = blinkBox->fade_out_max;
  }

  blinkBox->start = currTime;         // Remember when we started this state

  // Set how long the LED should hold state (on/off)
  unsigned long duration = random(duration_min, duration_max);
  blinkBox->until = currTime + duration;

  // Set how long to fade in/out the LED
  blinkBox->fade_dur = random(fade_min, fade_max);
  blinkBox->fade_until = currTime + blinkBox->fade_dur;
}


/*********************
 * Initalize the Blinking Box
 *********************/
void setupBlinkBox(struct BlinkBox *blinkBox) {
  pinMode(blinkBox->ledPin, OUTPUT);      // sets the pin as output

  digitalWrite(blinkBox->ledPin, blinkBox->ledStatus);  // Set the initial State

  setWaitUntil(blinkBox);

  blinkBox->fade_until = currTime;        // No fading on setup
  blinkBox->fade_dur   = 0;               // No fading on setup
}


/*********************
 * Process the Blinking Box
 *********************/
void loopBlinkBox(struct BlinkBox *blinkBox) {
  unsigned long elapsed = 0;
  unsigned long fade = 0;

  if (currTime > blinkBox->until) {
    // Time is up.  Change the LED status.
    blinkBox->ledStatus = !blinkBox->ledStatus;
    setWaitUntil(blinkBox);
  }
  
  if (currTime < blinkBox->fade_until) {
    // Still fading
    elapsed = currTime - blinkBox->start;
    fade = 128+127*cos(PI/blinkBox->fade_dur*elapsed)*(blinkBox->ledStatus? -1: 1);

    analogWrite(blinkBox->ledPin, fade);
  }
  else {
    // Done fading, make sure the LED is in the proper state
    digitalWrite(blinkBox->ledPin, blinkBox->ledStatus);
  }
}


void setup() {

  currTime = millis();
  
  setupBlinkBox(&box1);
  setupBlinkBox(&box2);
  setupBlinkBox(&box3);
  setupBlinkBox(&box4);

}

void loop() {
  currTime = millis();
  
  loopBlinkBox(&box1);
  loopBlinkBox(&box2);
  loopBlinkBox(&box3);
  loopBlinkBox(&box4);
  
}

