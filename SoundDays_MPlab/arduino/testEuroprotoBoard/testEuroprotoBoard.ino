#include <SPI.h>
#include <DAC_MCP49xx.h>

#define SS_PIN 10 // slave select / chip select
DAC_MCP49xx dac(DAC_MCP49xx::MCP4921, SS_PIN);

// NB: pins used by DAC: 10, 11, 13

int pot1 = A3, pot2 = A4, pot3 = A5;

int counter = 0;

void setup() {
  // setup DAC
  dac.setSPIDivider(SPI_CLOCK_DIV16);
  dac.setPortWrite(true);
  dac.setGain(2);
  
  Serial.begin(115200);
}

void loop() {
  int p1 = analogRead(pot1);
  int p2 = analogRead(pot2);
  int p3 = analogRead(pot3);
  Serial.print(p1);
  Serial.print("\t");
  Serial.print(p2);
  Serial.print("\t");
  Serial.print(p3);
  Serial.print("\t");
  Serial.println();
  
  dac.output(counter); // dac is 12 bit: 0-4095, analogRead is 10 bit: 0-1023
  counter++;
  counter = counter % 4096;
  
  delay(20);
}
