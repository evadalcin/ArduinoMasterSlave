// SLAVE 1

#define STEPS 200

#include <ModbusSlave.h>
#include <SoftwareSerial.h>
#include <Stepper.h>

#define PHOTOCELL_PIN A0

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 32, 34, 36, 38);
SoftwareSerial RS485Serial(10, 11);  // RX, TX
Modbus slave(RS485Serial, 1, 2);
int motorSpeed;

uint16_t pieces = 0;
int photocellState = 0;
const int ledR = 7;  // Pin PWM per il LED rosso
const int ledG = 8;  // Pin PWM per il LED verde
const int ledB = 9;  // Pin PWM per il LED blu

int ledStatus = 0;
bool allarm = 0;

uint8_t readHoldingRegs(uint8_t fc, uint16_t address, uint16_t length, void *context);
uint8_t writeHoldingRegs(uint8_t fc, uint16_t address, uint16_t length, void *context);

void updateLEDState() {
  // Aggiorna il colore del LED in base al numero di pacchi
  if (ledStatus == 2) {
    // Allarme - LED rosso
    analogWrite(ledR, 255);  // Rosso a piena intensità
    analogWrite(ledG, 0);    // Nessun verde
    analogWrite(ledB, 0);    // Nessun blu
  } else if (ledStatus == 1) {
    // Stato di avviso - LED arancione
    analogWrite(ledR, 255);  // Rosso a piena intensità
    analogWrite(ledG, 30);  // Verde a intensità moderata
    analogWrite(ledB, 0);    // Nessun blu
  } else {
    // Funzionamento normale - LED verde
    analogWrite(ledR, 0);    // Nessun rosso
    analogWrite(ledG, 255);  // Verde a piena intensità
    analogWrite(ledB, 0);    // Nessun blu
  }
}

void MotorRun(int velocity) {
  stepper.setSpeed(velocity);

  if (velocity > 0) {
    stepper.step(1);
  }
}

uint8_t writeHoldingRegs(uint8_t fc, uint16_t address, uint16_t length, void *context) {

  for (uint16_t i = 0; i < length; i++) {
    if (address + i == 1) {
      if (slave.readRegisterFromBuffer(i) == 0) {
        motorSpeed = 0;
      } else {
        motorSpeed = 60;
      }
      //Serial.print("Writing motor speed: ");
      //Serial.println(motorSpeed);
    } else if (address + i == 3) {
      //Serial.println("Writing led status");
      ledStatus = slave.readRegisterFromBuffer(i);
    }

  }

  return STATUS_OK;
}

uint8_t readHoldingRegs(uint8_t fc, uint16_t address, uint16_t length, void *context) {
  //Serial.print("Reading register ");

  for (uint16_t i = 0; i < length; i++) {
    if (address + i == 2) {
      //Serial.print("reading motor speed");
      slave.writeRegisterToBuffer(i, motorSpeed);
    } else {
      slave.writeRegisterToBuffer(i, 0);
    }
    if (address + i == 4) {
      //Serial.println("pieces register");
      slave.writeRegisterToBuffer(i, pieces);
    }

    /*else {
         slave.writeRegisterToBuffer(i, 0);
      }*/
  }
  return STATUS_OK;
}


void setup() {
  /* register handler functions.
     into the modbus slave callback vector.
  */

  //pinMode(PHOTOCELL_PIN, INPUT);  // Pin per la fotocellula
  pinMode(ledR, OUTPUT);          // Impostiamo i pin LED come output
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  slave.cbVector[CB_WRITE_HOLDING_REGISTERS] = writeHoldingRegs;
  slave.cbVector[CB_READ_HOLDING_REGISTERS] = readHoldingRegs;

  // set Serial and slave at baud 9600.
  Serial.begin(9600);
  Serial.println("STARTING SLAVE 1");
  RS485Serial.begin(9600);
  slave.begin(9600);
  analogWrite(ledG, 255);
}

void loop() {
  MotorRun(motorSpeed);
  int photocellValue = analogRead(PHOTOCELL_PIN);

  if (photocellValue > 900) {
    if (photocellState == 1) {
      photocellState = 0;  // Passa allo stato vuoto
    }
  } else if (photocellValue < 720) {
    // Se il valore è basso, significa che il nastro è occupato
    if (photocellState == 0) {
      photocellState = 1;  // Passa allo stato occupato
      pieces++;            // Incrementa il conteggio dei pacchi
      updateLEDState();    // Aggiorna lo stato del LED in base al conteggio
    }
  }

  // Monitoraggio dei pacchi in tempo reale

  //Serial.print("Pieces: ");
  //Serial.println(pieces);
  //Serial.println(photocellValue);
  updateLEDState();

  slave.poll();
}
