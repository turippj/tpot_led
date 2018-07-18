#include <TURIPserverSPI.h>
#include <EEPROM.h>

#define MODEL 0x1001
#define SEREAL 0x0
#define PIN_LEDR 5
#define PIN_LEDG 6
#define PIN_LEDB 9

TURIPport* PortR;
TURIPport* PortG;
TURIPport* PortB;
TURIPport* PortSerialNum;

void setup() {
  pinMode(PIN_LEDR, OUTPUT);
  pinMode(PIN_LEDG, OUTPUT);
  pinMode(PIN_LEDB, OUTPUT);

  TURIPserver.begin(MODEL, SEREAL);
  PortR = TURIPserver.newPort(1);
  PortG = TURIPserver.newPort(2);
  PortB = TURIPserver.newPort(3);
  PortSerialNum = TURIPserver.getPort(TURIP_PORT_SERIAL);
  PortSerialNum->writeUint32(getSerialNum());
  PortSerialNum->postReceiveFunc = updateSerialNum;
  PortR->writeUint16(0);
  PortG->writeUint16(0);
  PortB->writeUint16(0);
  PortR->postReceiveFunc = changeLedValue;
  PortG->postReceiveFunc = changeLedValue;
  PortB->postReceiveFunc = changeLedValue;
  TURIPserverSPI.begin();
}

void loop() {
  TURIPserverSPI.update();
}

int changeLedValue(){
  analogWrite(PIN_LEDR, constrain(PortR->readUint16(), 0, 255));
  analogWrite(PIN_LEDG, constrain(PortG->readUint16(), 0, 255));
  analogWrite(PIN_LEDB, constrain(PortB->readUint16(), 0, 255));
  return 0;
 }

 int updateSerialNum(){
  uint32_t serialNum = PortSerialNum->readUint32();
  EEPROM.update(0, (uint8_t)serialNum);
  EEPROM.update(1, (uint8_t)(serialNum >> 8));
  EEPROM.update(2, (uint8_t)(serialNum >> 16));
  EEPROM.update(3, (uint8_t)(serialNum >> 24));
 }

uint32_t getSerialNum(){
  return EEPROM[0] + (EEPROM[1] << 8) + (EEPROM[2] << 16) + (EEPROM[3] << 24);
}

