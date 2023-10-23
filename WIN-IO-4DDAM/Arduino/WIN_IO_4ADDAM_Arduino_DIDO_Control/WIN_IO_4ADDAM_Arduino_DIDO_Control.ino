#include <ModbusMaster.h>  //Library for using ModbusMaster
#define MAX485_DE 3
#define MAX485_RE_NEG 2

ModbusMaster node;  //object node for class ModbusMaster
bool state = true;
bool state1 = false;

void preTransmission()  //Function for setting stste of Pins DE & RE of RS-485
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission() {
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup() {
  pinMode(8, OUTPUT);
  digitalWrite(8, 1);

  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);

  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  Serial.begin(9600);                     //Baud Rate as 9600
  node.begin(1, Serial);                  //Slave ID as 1
  node.preTransmission(preTransmission);  //Callback for configuring RS-485 Transreceiver correctly
  node.postTransmission(postTransmission);
}

void loop() {
  uint8_t result;
  uint8_t result1;
  uint8_t readResult;
  bool coilValue;

  result = node.writeSingleCoil(0x0000, state);
  state = !state;

  // Read the coil at address 0x0004
  readResult = node.readCoils(0x0004, 1);  // Read a single coil at address 40004
  if (readResult == node.ku8MBSuccess) {
    coilValue = node.getResponseBuffer(0);  // Get the coil value
    Serial.print("Sensor Status at DI1 is : ");
    if (coilValue == false) 
    {
      Serial.println(" On ");
      state1 = !state1;
      for (int i = 0; i < 3; i++) 
      {
        result1 = node.writeSingleCoil(0x0003, state1);
      }
      state1 = !state1;
    } 
    else if (coilValue == true) 
    {
      Serial.println(" Off ");
      for (int i = 0; i < 3; i++) {
        result1 = node.writeSingleCoil(0x0003, state1);
      }
    }
  } else {
    Serial.println("Read error");
  }
  // delay(1000);
}