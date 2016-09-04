// SensorFade - read a photosensor and control several LEDs at different brightnesses
//
// Copyright (c) 2016, Garth Zeglin.  All rights reserved. Licensed under the
// terms of the BSD 3-clause license as included in LICENSE.
//
// This program assumes that:
//
//  1. A sensor is attached to analog input A0.  The exercise uses a photocell
//     with a 5.6K pull-down resistor in a voltage divider.
//
//  2. Pins 5 and 6 are attached via dropping resistors to LEDs which connect to
//     ground.  The output pin is used to source current; the logic will be
//     positive (HIGH = ON).

// ================================================================================
// Define constant values.  
#define LED1PIN 5
#define LED2PIN 6
#define SENSORPIN 0     // 0 is A0

#define BLINKDELAY 500  // in milliseconds

// ================================================================================
// Configure the hardware once after booting up.  This runs once after pressing
// reset or powering up the board.

void setup()
{
  // Initialize the hardware digital pin 13 as an output.  The 'OUTPUT' symbol
  // is pre-defined by the Arduino system.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize the external LEDs as outputs.
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
}

// ================================================================================
// Run one iteration of the main event loop.  The Arduino system will call this
// function over and over forever.
void loop()
{
  // First just flash a fixed pattern for a few cycles.  This uses binary
  // digital outputs so the LEDS will be either full brightness or off.
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1PIN, HIGH);   // turn LED1 on
    delay(BLINKDELAY);
    
    digitalWrite(LED1PIN, LOW);    // turn LED1 off
    digitalWrite(LED2PIN, HIGH);   // turn LED2 on
    delay(BLINKDELAY);
    
    digitalWrite(LED2PIN, LOW);    // turn LED2 off
    delay(BLINKDELAY);
  }

  // Then continuosly map changes in the input level into pulse-width-modulated
  // (PWM) output.

  // Begin an infinite loop: an empty condition is assumed to be 'true', so the body will run forever.
  for (int cycle = 0; ; cycle++) {

    // Read the voltage on the sensor input.  This function returns a value
    // between 0 and 1023 representing a voltage between 0 and 5V.
    int sensor = analogRead(SENSORPIN);
    
    // Compute proportional signals to drive the LEDS by mapping an input range
    // to the output range.
    //
    //   The PWM output is scaled from 0 to 255.
    //
    //   The input range for a typical photocell with a 5.6K bias resistor is
    //   centered around 4 volts.  This can be verified using a voltmeter to
    //   check the range of inputs on A0 and adjust the following values.  Note
    //   the use of the scaling factor to express voltage in terms of the input
    //   range:
    
#define VLOW  (3.5 * (1024.0/5.0))
#define VHIGH (4.2 * (1024.0/5.0))
    
    int led1_value = map(sensor, VLOW, VHIGH,   0, 255);
    int led2_value = map(sensor, VLOW, VHIGH, 255,   0);  // symmetric scaling
    
    // Emit PWM signals with a proportional average power; the LEDs will have
    // variable brightness.  The constrain function will ensure the values stay
    // within the correct limits.
    analogWrite(LED1PIN, constrain(led1_value, 0, 255));
    analogWrite(LED2PIN, constrain(led2_value, 0, 255));

    // Blink the onboard LED.  The logic expression is left as a challenge to the reader.
    digitalWrite(LED_BUILTIN, cycle & 32);
    
    // Delay for a short interval to create a periodic sampling rate.
    delay(20);
  }

  // This point will not be reached.
}
