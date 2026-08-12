# ATmega328P Line Following Robot

An autonomous line-following robot built on the ATmega328P microcontroller, using a five-sensor IR array and PID/rule-based control to track a black line in real time.

## Overview

This project implements a low-cost embedded system that autonomously detects and follows a black line on a white surface. A five-sensor IR array reports line position to the ATmega328P, which computes a corrective action and drives two DC geared motors through an L298 dual H-bridge motor driver via PWM. Two control strategies — a rule-based algorithm and a PID controller — were implemented and experimentally compared on the same hardware.

## Features

- Autonomous line following on a black-line / white-surface track
- Five-sensor IR array for line-position detection (direct digital GPIO reading)
- Real-time control loop running at ~200 Hz
- PWM-based independent dual motor speed control (Timer0, Fast PWM mode)
- L298 dual H-bridge motor driver for bidirectional motor control
- Rule-based and PID control strategies, both implemented and compared
- Line-loss detection with immediate stop
- Direct register-level embedded C (no Arduino libraries/HAL)

## Hardware

| Component | Specification / Role |
|---|---|
| ATmega328P Microcontroller | 8-bit AVR MCU @ 16 MHz |
| IR Sensor Array (5 Sensors) | Digital line detection module |
| L298 Motor Driver | Dual H-bridge driver, 2A max per channel |
| DC Geared Motors (×2) | Differential drive |
| Li-ion Battery Pack | 3 × 3.7V, 3000 mAh (11.1V total) |
| Chassis & Wheels | Mechanical robot frame |

## Software & Tools

| Tool | Purpose |
|---|---|
| Atmel Studio 7 | Writing, compiling, and debugging embedded C; generates `.hex` |
| Proteus 8 | Circuit simulation prior to hardware implementation |
| AVRDUDE | Flashing the `.hex` file to the ATmega328P via USB programmer |
| Embedded C | Firmware language (direct register-level programming) |

## System Architecture

```text
IR Sensor Array (5 sensors)
        ↓
ATmega328P (GPIO read, Timer0 PWM)
        ↓
Control Algorithm (Rule-Based / PID)
        ↓
PWM + Motor Direction Signals
        ↓
L298 Dual H-Bridge Motor Driver
        ↓
DC Geared Motors (×2)
```

## How It Works

1. The five IR sensors are polled as digital GPIO inputs to determine line position.
2. The control algorithm (rule-based or PID) computes the corrective motor response.
3. Timer0 (Fast PWM mode, ~976 Hz) sets motor speed via `OCR0A`/`OCR0B`, while direction is set through the L298's IN1–IN4 inputs.
4. The loop repeats at ~200 Hz (5 ms delay), continuously updating motor output from fresh sensor data.
5. If the line is lost, the robot stops immediately.

## Control Algorithm

### Rule-Based Control

Fixed logic based on sensor patterns. Stable and predictable, easier to tune, and reliable on straightforward track conditions.

### PID Control

A PID controller (Kp, Ki, Kd, tuned experimentally) computes corrections from the line-position error. Delivered smoother directional corrections, better tracking accuracy, and reduced oscillation on curves compared to the rule-based method. Exact tuned gain values are not specified in the report.

## Getting Started

### Build

1. Open the firmware project in **Atmel Studio 7**.
2. Compile the embedded C source to generate the `.hex` file.

### Flash

3. Flash the `.hex` file to the ATmega328P using **AVRDUDE** via a USB programmer.

### Simulation

Circuit design and control logic were verified in **Proteus 8** before hardware implementation. Open the provided Proteus project to reproduce the simulation.

## Results

| Test Case | Expected Result | Actual Result |
|---|---|---|
| Straight line | Smooth forward motion, no deviation | Stable forward movement, minimal drift |
| Slight left/right turn | Small correction without overshoot | Accurate, quick correction |
| Sharp edge / curve | Proper turning, no overshoot | Successfully followed tight curves with PID |
| Line loss | Robot stops immediately | Stopped immediately — confirmed reliable |

Overall, tracking was stable and accurate across straight lines, curves, and edges. The PID controller showed smoother, less oscillatory performance on curves, while the rule-based method was easier to tune and equally reliable on straightforward sections. No numerical accuracy percentages are reported.

## Repository Structure

> Suggested layout — not an existing repository structure, as none is specified in the report.

```text
ATmega328P-Line-Follower/
│
├── README.md
├── src/
├── proteus/
├── hex/
├── docs/
├── images/
└── LICENSE
```

## References

1. [Microchip — ATmega328P Datasheet](https://www.microchip.com/en-us/product/ATmega328P). Microchip Technology Inc., 2020.
2. [STMicroelectronics — L298 Dual Full-Bridge Driver Datasheet](https://www.st.com/resource/en/datasheet/l298.pdf).
3. [Design and Implementation of a Line Follower Robot Using PID Controller](https://www.researchgate.net/publication/335445451_Design_and_Implementation_of_a_Line_Follower_Robot_Using_PID_Controller).
4. [Development of an Autonomous Line Follower Robot Based on AVR Microcontroller](https://www.researchgate.net/publication/343437940_Development_of_an_Autonomous_Line_Follower_Robot_Based_on_AVR_Microcontroller).
5. Mazidi, M. A., Naimi, S., & Naimi, S. *AVR Microcontroller and Embedded Systems Using Assembly and C*. Pearson Education, 2011.
6. Ogata, K. *Modern Control Engineering*. Pearson Education, 2010.
