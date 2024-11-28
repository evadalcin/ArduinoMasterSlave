# Arduino Master and Slave: Industrial Production Line Simulation with Dashboard Control

### Group Project for Embedded course by Eva Dal Cin, Giacomo Di Bert, Giosuè Iaccarino, Matteo Monino and Matteo Santi

## Overview

This project simulates an industrial production line using Arduino, with Modbus RTU communication between a **Master** and a **Slave** device. The Master supervises and coordinates the operation of the production line, while the Slave simulates a conveyor belt and his sensors. The system includes features such as motor control, piece counting, alarm signaling, and LED status indication. The system also features a **C# MVC-based dashboard** for remote control and monitoring of the production line, including motor and alarm control.

### Key Features

- **Modbus Communication**: Uses Modbus RTU protocol for reliable data exchange between the Master and Slave.
- **Production Line Simulation**: Simulates a production line where the Slave represents a station that produces items.
- **LED Status Indicators**: The Slave uses RGB LEDs to represent different operational states (normal, warning, alarm).
- **Piece Counting**: The Slave counts the produced pieces and manages the production progress.
- **Motor Control**: The Master can start/stop the motor on the Slave device.
- **Alarm Handling**: The system can trigger alarms and handle reset events.
- **IoT Dashboard**: The C# MVC-based dashboard allows for remote monitoring and control of the system, including motor and alarm commands.

---

## Hardware Requirements

- 2x Arduino boards (for Master and Slave)
- RS485 transceivers (for Modbus communication)
- Stepper motor (connected to the Slave)
- Photocell sensor (to detect items passing by, connected to the Slave)
- RGB LEDs (to indicate different machine statuses, connected to the Slave)
- 7-segment display (to show the current piece count, connected to the Master)
- Power supply for the motor and Arduino boards

---

## Software Requirements

- **Arduino IDE**: Used for programming the Arduino boards.
- **ModbusMaster**: Modbus library for the Master (for communication with the Slave).
- **ModbusSlave**: Modbus library for the Slave (for handling requests from the Master).
- **Simple5641AS**: Library for controlling the 7-segment display on the Master.
- **SoftwareSerial**: For RS485 communication between the Arduino boards.
- **C# ASP.NET MVC**: Used for creating the dashboard that allows controlling the system remotely.
---

## System Components

### Master Code (Arduino - `master.cpp`)

The Master device supervises the overall production line, controls the Slave, and interacts with a 7-segment display to show production progress.

Key functionalities:
- Sends Modbus commands to the Slave.
- Displays the number of pieces produced and the motor speed on a 7-segment display.
- Monitors alarms and controls LED status on the Slave.

### Slave Code (Arduino - `slave.cpp`)

The Slave simulates a machine on the production line, controls a stepper motor, counts the number of items produced, and manages LED status.

Key functionalities:
- Controls a stepper motor.
- Tracks the number of items produced based on input from a photocell sensor.
- Updates LED status based on operational state (normal, warning, alarm).
- Responds to Modbus requests from the Master.

### Dashboard (C# MVC Web App)

The dashboard is built using ASP.NET Core MVC and enables remote control of the production line. It provides buttons for the user to control the motor and alarm. The communication between the dashboard and the Arduino is handled through the **ArduinoReader** service, which sends commands to the Arduino devices via serial communication.

Key functionalities:
- **Motor Control**: Buttons to turn the motor on and off.
- **Alarm Control**: Buttons to activate and deactivate the alarm.

---
## Example Commands

 Example commands you can send to the Master:

- `motoron`: Turns the motor on in the Slave.
- `motoroff`: Turns the motor off in the Slave.
- `alarmon`: Activates the alarm on the Slave (Red LED).
- `alarmoff`: Deactivates the alarm (resumes normal operation).
- `reset`: Resets the system (production count and motor status).

---

## License

This project is open-source and can be freely used and modified. Please give appropriate credit when using this code.

