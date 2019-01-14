/*
 * Copyright (c) 2019 University of Edinburgh. All rights reserved.
 * 
 * Redistribution and use in source and binary forms are permitted 
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of Edinburgh. The name of the University of 
 * Edinburgh may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * Paragliding simlulator control unit.
 * 
 * Uses an IR controller to flash two LEDs in pre-defined sequences.
 * 
 * Author: Chris Sangwin
 * Verion: 1.0
 */

#include <IRremote.h>

// Digital pin to which the IR receiver is attached.
int RECV_PIN = 4;
// Digitial pins to which the LEDs are attached.
int R_LED_PIN = 8;
int L_LED_PIN = 7;

// The following integers record the buttons on the control.
// To see these use the serial monitor with the device plugged in by USB.
//
// These need to be copied below by hand!
int B_0 = 0xFF4AB5;
int B_1 = 0xFF6897;
int B_2 = 0xFF9867;
int B_3 = 0xFFB04F;
int B_4 = 0xFF30CF;
int B_5 = 0xFF18E7;
int B_6 = 0xFF7A85;
int B_7 = 0xFF10EF;
int B_8 = 0xFF38C7;
int B_9 = 0xFF5AA5;

// States to use when flashing.
int state_I = 0;
int state_R = 0;
int state_L = 0;

// Light sequences.
int led_Seq_1[] = {1,0,2,0,0,1,2,2,0,1,2,1,0,2,1,2,2,1,1,0,2,2,0,2,2,2,1,1,0,1,0,0,0,1,2,2,2,2,2,0,0,1,1,1,0,1,2,2,1,1}; 
int timeSeq_1[] = {2,1,2,2,1,2,1,2,1,1,1,2,1,2,1,2,1,1,2,2,1,1,2,2,1,2,2,1,2,1,2,1,1,2,2,1,1,2,1,1,2,1,1,1,1,1,1,2,1,2}; 

int led_Seq_2[] = {1,1,2,0,2,2,0,1,2,0,0,2,1,0,2,2,0,2,1,2,2,0,0,0,0,0,1,2,0,2,0,1,1,1,1,1,1,0,2,1,2,1,2,0,1,1,2,2,2,2}; 
int timeSeq_2[] = {2,1,1,2,1,1,2,1,2,1,2,1,2,2,2,2,2,2,1,1,1,2,1,2,1,2,2,1,1,2,2,1,1,2,2,2,2,1,2,1,2,2,2,2,2,1,1,1,1,1}; 

int led_Seq_3[] = {1,2,0,0,2,0,2,1,1,0,1,0,2,0,0,2,1,0,0,2}; 
int timeSeq_3[] = {1,1,2,1,1,2,1,1,2,2,2,1,1,1,2,2,1,1,1,1}; 


IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(L_LED_PIN, OUTPUT);
  pinMode(R_LED_PIN, OUTPUT);

  // Turn on the LEDs to indicate they are actually working, and the thing is alive.
  digitalWrite(R_LED_PIN, HIGH);  
  digitalWrite(L_LED_PIN, HIGH);  
  digitalWrite(LED_BUILTIN, HIGH);  

  
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");

  delay(2000);                      
  // Flash everything to indicate we are ready.
  flash_both();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);

    // Value of B0
    if (results.value == 0xFF4AB5) {
      flash_both();
      state_L = 0;
      state_R = 0;
    }

    // Value of B1
    if (results.value == 0xFF6897) {
      flash_sequence(led_Seq_1, timeSeq_1, sizeof(led_Seq_1));
    }

    // Value of B2
    if (results.value == 0xFF9867) {
      flash_sequence(led_Seq_2, timeSeq_2, sizeof(led_Seq_2));
    }

    // Value of B3
    if (results.value == 0xFFB04F) {
      flash_sequence(led_Seq_3, timeSeq_3, sizeof(led_Seq_3));
    }

    // Value of B7
    if (results.value == 0xFF10EF) {
      state_L = (state_L + 1) %2;
    }
    // Value of B9
    if (results.value == 0xFF5AA5) {
      state_R = (state_R + 1) %2;
    }

    irrecv.resume(); // Receive the next value
  }

  // Blink the LEDs.
  if (state_R == 0) {
    digitalWrite(R_LED_PIN, LOW);  
  } else {
    digitalWrite(R_LED_PIN, HIGH);  
  }
  if (state_L == 0) {
    digitalWrite(L_LED_PIN, LOW);  
  } else {
    digitalWrite(L_LED_PIN, HIGH);  
  }
  delay(100);                      
  digitalWrite(R_LED_PIN, LOW);  
  digitalWrite(L_LED_PIN, LOW);  


  // Blink the internal LED.
  state_I = (state_I + 1) %20;
  if (state_I == 0) {
    digitalWrite(LED_BUILTIN, HIGH);  
  } else {
    digitalWrite(LED_BUILTIN, LOW);  
  }

  delay(100);                      
}

// Flash a sequnce of LEDs.
void flash_sequence(int LEDs[], int LEDtimes[], int arraySize) {
  for (int i=0; i < arraySize; i++) {
    switch (LEDs[i]) {
      case 1: flash_led(L_LED_PIN, 1000*LEDtimes[i]); break;
      case 2: flash_led(R_LED_PIN, 1000*LEDtimes[i]); break;
      default: delay(1000*LEDtimes[i]); break;
    }
  }
  // Indicate the end of the process by flashing the red internal LED.
  for (int i=0; i <= 10; i++){
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(100);                      
    digitalWrite(LED_BUILTIN, LOW);  
    delay(100);
  }
}

// Function to flah LED l for a time t.
void flash_led(int l, int t) {
  int c;
  c = t/200;
  for (int i=0; i <= c; i++){
    digitalWrite(l, HIGH);
    delay(100);                      
    digitalWrite(l, LOW);  
    delay(100);
   }
}

void flash_both() {
  for (int i=0; i <= 5; i++){
    digitalWrite(R_LED_PIN, HIGH);  
    digitalWrite(L_LED_PIN, HIGH);  
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(100);                      
    digitalWrite(R_LED_PIN, LOW);  
    digitalWrite(L_LED_PIN, LOW);  
    digitalWrite(LED_BUILTIN, LOW);  
    delay(100);
  }
}
