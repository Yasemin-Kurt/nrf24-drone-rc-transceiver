# NRF24 Drone RC Transceiver

Custom drone remote control communication system developed using **Arduino** microcontrollers and **NRF24L01 wireless transceiver modules**.

This project implements a **custom transmitter–receiver architecture** designed to provide control data from a handheld transmitter to a **Pixhawk flight controller**.

The transmitter reads and processes pilot control inputs, prepares the control data structure and **wirelessly transmits it to the receiver** using the NRF24L01 module.

On the receiver side, the data transmitted by the transmitter is received and interpreted. The receiver prepares the control values in a format suitable for the **Pixhawk RC input interface** and forwards them to the flight controller (Pixhawk).

This system was developed as part of the **TEKNOFEST International Unmanned Aerial Vehicle Competition**.

***The system was tested on an actual quadcopter, proving reliable operation with a Pixhawk-based flight controller.***

---

# System Overview

The system consists of two main modules:

## Transmitter

The transmitter unit collects control inputs from the pilot and **sends them wirelessly to the receiver**.

Main responsibilities:

* Reading control inputs 
* Processing and filtering input signals
* Structuring control data packets
* Wireless transmission using NRF24L01

Preferred components in this project:

* Arduino Uno
* NRF24L01 wireless transceiver module
* Dual-axis joystick modules
* Control switches
* Power supply

---

## Receiver

The receiver unit **receives the data transmitted by the transmitter** and prepares it for the flight controller.

Main responsibilities:

* Receiving wireless control data
* Interpreting transmitted control packets
* Preparing control signals compatible with Pixhawk
* Delivering control signals to the flight controller

Preferred components in this project:

* Arduino Uno
* NRF24L01 wireless transceiver module
* Pixhawk flight controller

---

# Flight Mode Control

Control switches on the transmitter are used to change flight modes of the UAV.

The switch positions generate different RC channel values that are sent to the Pixhawk flight controller. The corresponding flight modes are assigned by the user through **Mission Planner**, where the channel ranges can be configured according to preference.

This allows flexible mapping of switch positions to various flight modes, such as Position Hold, Guided, Auto, Land, Failsafe, and others configurable via Mission Planner.

The flight controller interprets the received RC channel values and activates the corresponding mode defined in Mission Planner.

---

# System Architecture

Control Inputs  ->  Transmitter Microcontroller  ->  NRF24L01 Wireless Communication  ->  Receiver Microcontroller  ->  Prepared RC Channel Signals  ->  Pixhawk Flight Controller

The Pixhawk flight controller is responsible for:

* flight stabilization
* attitude control
* motor control
* flight mode management
* UAV movement according to pilot inputs


---

# Applications

This project can be used as a reference for:

* Custom UAV controller development
* Embedded wireless communication systems
* NRF24L01 based remote control systems
* Pixhawk compatible RC interfaces
* Educational drone and robotics projects

---

# Possible Future Improvements

Different communication alternatives may be explored to extend the system.

---

# Authors

Yasemin KURT

Neziha BURHAN

---
