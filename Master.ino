#include <ModbusMaster.h>

#include <SoftwareSerial.h>
#include <Simple5641AS.h>
SoftwareSerial RS485Serial(10, 11); // RX, TX

#define MAX485_DE      2
int motor = 0;
int pieces_to_produce = 5; // idem
int pieces_produced = 0;
int ledStatus = 0;
int allarm  = 0;
int result = 15;

ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(9600);
  RS485Serial.begin(9600);

  node.begin(1, RS485Serial);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

const uint8_t segmentPins[] = {22, 23, 24, 25, 26, 27, 28, 29};
const uint8_t digitSelectionPins[] = {30, 31, 32, 33};
Simple5641AS display(segmentPins, digitSelectionPins);

void loop() {
  display.displayNumber(1, 100 * pieces_produced + pieces_to_produce);

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');

    if (command == "motoron") {
      node.writeSingleRegister(1, 1);
    } else if (command == "motoroff") {
      node.writeSingleRegister(1, 0);
    } else if (command == "alarmon") {
      allarm = true;
      node.writeSingleRegister(1, 0);
    } else if (command == "alarmoff") {
      allarm = false;
    } else if (command == "reset"){
      node.writeSingleRegister(5, 1);
      ledStatus = 0;
      pieces_produced = 0;
      allarm = false;
    }
  }

  node.clearResponseBuffer();
  result = node.readHoldingRegisters(0, 3);
  if (result == node.ku8MBSuccess) {
    int nn = node.getResponseBuffer(2);
    Serial.print("motorspeed ");
    Serial.println(nn);
  }

  node.clearResponseBuffer();

  readPackageCount();

  manageLEDStatus();

  if (pieces_produced >= pieces_to_produce) {
    node.writeSingleRegister(1, 0);
    pieces_produced = pieces_to_produce;
  }
}

void manageLEDStatus() {
  if (allarm) {
    ledStatus = 2;
  } else if (pieces_produced < pieces_to_produce) {
    ledStatus = 0;
  } else if (pieces_produced >= pieces_to_produce) {
    ledStatus = 1;
  }

  Serial.print("led status");
  Serial.println(ledStatus);
  node.writeSingleRegister(3, ledStatus);
}

void readPackageCount() {
  node.clearResponseBuffer();

  result = node.readHoldingRegisters(0, 8);
  Serial.println(result);
  if (result == node.ku8MBSuccess) {
    pieces_produced = node.getResponseBuffer(4);
    Serial.print("npezzi ");
    Serial.println(pieces_produced);
  } else {
    Serial.println("nessun dato");
  }
}
