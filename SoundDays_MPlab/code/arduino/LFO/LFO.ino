#include <SPI.h>
#include <DAC_MCP49xx.h>
#define SS_PIN 10 // slave select / chip select
DAC_MCP49xx dac(DAC_MCP49xx::MCP4921, SS_PIN);

// NB: pins used by DAC: 10, 11, 13

int pot1 = A3, pot2 = A4, pot3 = A5;
float counter = 0;

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

  p1 = 50;

  float increment = (p1 + 1) / 500.0;     // dividing by 500.0 forces result to be float
  counter = counter + increment;
  
  if (counter > 360) counter = 0;

  float sinVal = sin(radians(counter));   // sin reads radians and returns value from -1 to 1
  int lfoVal = (sinVal + 1) * 2048;       // add 1 and multiply by 2048 to fit 12 bit DAC output range of 0-4096
  dac.output(lfoVal);
  //printPots();
  Serial.println(lfoVal);

  delay(1);
}

void printPots() {
  int p1 = analogRead(pot1);
  int p2 = analogRead(pot2);
  int p3 = analogRead(pot3);

  Serial.print(p1);
  Serial.print("\t");
  Serial.print(p2);
  Serial.print("\t");
  Serial.print(p3);
  Serial.println();
}
