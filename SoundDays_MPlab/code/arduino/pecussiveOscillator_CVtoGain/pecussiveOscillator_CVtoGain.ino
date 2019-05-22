// CFO EUROPROTO 1 as 8bit oscillator with FM synthesisish


// sound processing based on the arduino pocket piano code and circuit, by critter and guittari
// more info: http://www.critterandguitari.com/home/store/arduino-piano.php

#include <stdio.h>                      // lets us do some C string manipulations for the LCD display
#include <avr/pgmspace.h>               // lets us use progmem for the giant wavetables
#include <EEPROM.h>                     // lets us read from and write to the EEPROM

#define numberOfWaveforms 16

// sound generation
int frequency = 0;
byte gain = 0xff;                       // gain of oscillator
byte waveForm = 0;

// FM synthesis
byte modulatorWaveForm = 0;
uint16_t harmonicity = 0;              // harmonicity (first 8 bits are fractional) (0 - 65536)
uint8_t modulatorDepth = 0;            // modulation depth (0 - 255)

// define inputs and outputs, NB: DAC is connected to pins 10, 11 and 13
int pot1 = A5, pot2 = A4, pot3 = A3, cv = A2;

int lowest = 346, highest = 732;

void setup()
{
  Serial.begin(115200);

  // setup interrupt for sound engine
  // DAC SCK is digital 13, DAC MOSI is digital 11, DAC CS is digital 10
  cli();                                          // clear interrupts. this has been added, to get the pocket piano code working
  TIMSK2 = 1 << OCIE2A;                           // interrupt enable audio timer
  OCR2A = 127;
  TCCR2A = 2;                                     // CTC mode, counts up to 127 then resets
  TCCR2B = 0 << CS22 | 1 << CS21 | 0 << CS20;     // different for atmega8 (no 'B' i think)
  SPCR = 0x50;                                    // set up SPI port
  SPSR = 0x01;
  DDRB |= 0x2E;                                   // PB output for DAC CS, and SPI port
  PORTB |= (1 << 2);
  sei();                                          // global interrupt enable
}

void loop() {

  modulatorWaveForm = 0;
  waveForm = map(analogRead(pot1), 0, 1023, 0, 16);
  modulatorDepth = map(analogRead(pot3), 0, 1023, 0, 50);
  harmonicity = map(analogRead(pot3), 0, 1023, 0, 50);
  frequency = map(analogRead(pot2), 0, 1023, 20, 2500);

  // scale incoming CV to gain from 0-255
  // lowest and highest are lowest and highest CV reading coming in to the system
  // use cvDebug() function and turn on the serial monitor to check these values
  // cvDebug();
  gain = map(analogRead(cv), lowest, highest, 0, 255);
  
  delay(1);
}

void cvDebug() {
  int cvVal = analogRead(cv);
  lowest = min(lowest, cvVal);
  highest = max(highest, cvVal);
  Serial.print(cvVal);
  Serial.print("\t");
  Serial.print(lowest);
  Serial.print("\t");
  Serial.print(highest);
  Serial.print("\t");
  Serial.println();
  }
