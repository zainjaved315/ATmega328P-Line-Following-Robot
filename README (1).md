# ATmega328P Line Following Robot

An autonomous, real-time line-following robot built on the ATmega328P microcontroller, developed for the MPS EE-222: Microprocessor Systems course.

---

## Project Overview

This project presents the design and implementation of an autonomous line-following robot using the ATmega328P microcontroller. The objective was to develop a low-cost embedded system capable of detecting and tracking a black line on a white surface in real time, addressing the broader problem of autonomous path navigation in embedded robotics and intelligent transport systems.

The robot uses a **five-sensor infrared (IR) array** to determine the position of the line and accordingly adjusts the speed and direction of two DC geared motors via PWM signals. The ATmega328P processes the sensor inputs and drives an **L298 dual H-bridge motor driver** to control motor movement. Two control strategies were implemented and experimentally compared: a **rule-based algorithm** and a **PID controller**.

---

## Objectives

Based on the problem definition in the report, the project's objectives were to:

- Design a microcontroller-based autonomous robot that processes sensor inputs, makes real-time navigation decisions, and controls motors to follow a predefined path without human intervention.
- Integrate sensing, control, and actuation hardware with time-critical software under cost and power constraints.
- Achieve real-time processing of sensor data (~200 Hz loop) to maintain accurate path tracking.
- Implement and compare two distinct control strategies (rule-based and PID) on the same hardware platform to quantitatively evaluate performance.

---

## Features

- Autonomous line following on a black line / white surface track
- Five-sensor IR array for line-position detection
- ATmega328P-based real-time control (~200 Hz control loop)
- PWM-based dual motor speed control via Timer0 (Fast PWM mode)
- L298 dual H-bridge motor driver for bidirectional motor control
- Two selectable/comparable control strategies: rule-based control and PID control
- Line-loss detection with immediate stop behavior
- Direct register-level embedded C implementation (no Arduino libraries/HAL)

---

## Hardware Components

| Component | Specification / Role |
|---|---|
| ATmega328P Microcontroller | 8-bit AVR MCU @ 16 MHz |
| IR Sensor Array (5 Sensors) | Digital line detection module |
| L298 Motor Driver | Dual H-bridge driver, 2A max per channel |
| DC Geared Motors (×2) | Geared DC motors for differential drive |
| Li-ion Battery Pack | 3 × 3.7V, 3000 mAh (11.1V total) |
| Chassis & Wheels | Mechanical robot frame |

*Justifications for each component (as given in the report):* the ATmega328P was chosen for its low cost and built-in PWM, GPIO, ADC, and timers; the IR array for accurate line position with full edge coverage; the L298 for safe bidirectional control with sufficient current supply; the geared motors for high torque and stable turning; the Li-ion pack for lightweight, rechargeable, adequate-runtime power; and the chassis for stable sensor alignment and weight distribution.

---

## Software & Tools

| Tool / Software | Purpose |
|---|---|
| Atmel Studio 7 | IDE for writing, compiling, and debugging the embedded C code; generates the `.hex` file for flashing |
| Proteus 8 | Circuit simulation software used to verify circuit design and logic before hardware implementation |
| AVRDUDE | Command-line tool for burning the compiled `.hex` file to the ATmega328P via a USB programmer |
| Embedded C | Programming language used for the entire firmware implementation |

---

## System Architecture

The system architecture follows a sense–decide–act loop, as described in the report:

```text
IR Sensor Array (5 sensors)
        ↓
ATmega328P (GPIO read, Timer0 PWM)
        ↓
Control Algorithm (Rule-Based or PID)
        ↓
PWM Signal + Motor Direction Logic
        ↓
L298 Dual H-Bridge Motor Driver
        ↓
DC Geared Motors (×2, differential drive)
```

---

## Working Principle

1. **Sensor detection** — The five-sensor IR array continuously reads the line position via digital GPIO inputs.
2. **Sensor data processing** — The ATmega328P reads sensor states directly as digital inputs (selected over ADC-based reading for faster response and simpler processing).
3. **Line-position determination** — The pattern of the five sensor readings indicates the position of the black line relative to the robot.
4. **Control algorithm** — Either the rule-based logic or the PID controller computes the corrective action needed.
5. **Motor speed/direction adjustment** — Timer0 generates PWM signals (via `OCR0A`/`OCR0B`) that set the ENA/ENB duty cycles on the L298 driver, adjusting each wheel's speed independently; direction is set via the IN1–IN4 driver inputs.
6. **Continuous feedback** — The control loop repeats at approximately 200 Hz (driven by a 5 ms delay), continuously updating motor output based on sensor input.
7. **Line-loss behavior** — If the line is lost, the robot stops immediately; this was confirmed reliable during testing.

---

## Control Algorithms

### Rule-Based Control

The rule-based method uses fixed logic based on sensor patterns to determine corrective motor actions. According to the report:

- Provides **stable and predictable operation**.
- Proved **easier to tune**.
- Delivered **reliable, predictable performance** under straightforward track conditions.

### PID Control

A PID controller was implemented and tuned experimentally alongside the rule-based method. According to the report:

- Delivers **smoother corrections and improved tracking accuracy**.
- Showed **superior smoothness on curves and reduced oscillation** compared to the rule-based method.
- Successfully followed **tight curve edges** during sharp-edge test cases.
- **PID gains (Kp, Ki, Kd) required iterative experimental tuning** for optimal performance. Exact tuned gain values are not specified in the report.

The report does not provide the explicit PID error/output equation; only the qualitative description above and the tuning process are documented.

---

## Flowchart / Control Flow

The main control loop, as described in the report, follows this general flow:

```text
Start
  ↓
Initialize ATmega328P (GPIO, Timer0 PWM)
  ↓
Read 5 IR Sensors
  ↓
Determine Line Position
  ↓
Apply Control Algorithm (Rule-Based / PID)
  ↓
Set Motor PWM Duty Cycle + Direction
  ↓
Line Lost?
 ┌───────┴───────┐
Yes              No
 ↓                ↓
Stop Motors     Continue Loop
 ↓                │
 └───────←────────┘
```

A dedicated flowchart image and code-snippet images for the main control loop are included in the original project report.

---

## PWM and Timer Configuration

Timer0 of the ATmega328P is configured in **Fast PWM mode**. The following mathematical analysis is derived directly from the hardware configuration used in the project.

**Given parameters**

- CPU Clock Frequency (F_CPU): **16 MHz**
- Prescaler (N): **64**
- Timer type: **8-bit (0–255, 256 counts)**

**Timer Tick Time**

```
Tick = Prescaler / F_CPU
Tick = 64 / (16 × 10^6)
Tick = 4 μs
```

**Timer Frequency**

```
f_timer = 1 / Tick
f_timer = 1 / (4 × 10^-6)
f_timer = 250 kHz
```

**PWM Frequency (Fast PWM Mode)**

```
f_PWM = F_CPU / (N × 256)
f_PWM = 16,000,000 / (64 × 256)
f_PWM ≈ 976.56 Hz ≈ 976 Hz
```

**PWM Time Period**

```
T_PWM = 1 / f_PWM
T_PWM = 1 / 976.56
T_PWM ≈ 1.024 ms
```

**Duty Cycle Calculation**

```
Duty Cycle (%) = (OCR0x / 255) × 100
```

Example given in the report: `OCR0A = 90` →

```
Duty Cycle = (90 / 255) × 100 ≈ 35%
```

**Control Loop Frequency**

The software delay `_delay_ms(5)` sets the control loop period:

```
T_delay = 5 ms
f_loop = 1 / T_delay = 1 / 0.005 = 200 Hz
```

**Summary Table**

| Quantity | Formula | Result |
|---|---|---|
| Timer Tick Time (T_tick) | Prescaler / F_CPU | 4 μs |
| Timer Frequency (f_timer) | 1 / T_tick | 250 kHz |
| PWM Frequency (f_PWM) | F_CPU / (N × 256) | ≈ 976 Hz |
| PWM Period (T_PWM) | 1 / f_PWM | ≈ 1.024 ms |
| Duty Cycle (OCR0A = 90) | (OCR0x / 255) × 100 | ≈ 35% |
| Control Loop Frequency (f_loop) | 1 / 0.005 | 200 Hz |

In the circuit, Timer0 drives the PWM signal on output pin **OC0A**. The ~976 Hz PWM frequency is fast enough that the DC motors experience smooth, continuous control rather than discrete pulses. The duty cycle set via `OCR0A` directly controls the average voltage delivered to the motors, and the 5 ms control loop delay ensures the system updates its output 200 times per second for stable, responsive real-time behavior.

---

## Hardware Connections

The report describes hardware connections at the signal/register level rather than by exact pin number:

- **IR sensor outputs** — read as digital inputs by the ATmega328P (GPIO, configured via the Data Direction Registers).
- **Motor driver control signals (IN1–IN4)** — driven as digital outputs from the ATmega328P to the L298 motor driver for direction control.
- **Motor driver enable pins (ENA, ENB)** — driven by PWM signals generated from Timer0 (`OCR0A`, `OCR0B`) for independent speed control of each wheel.
- **Port registers used:** `PORTB`, `PORTD`, `PORTC` for sensor reading and motor direction control.

Exact ATmega328P pin numbers / port bit assignments are not specified in the report and are therefore omitted here.

---

## Simulation

**Proteus 8** was used as the circuit simulation software to verify the circuit design and control logic before physical hardware implementation. This allowed the team to validate the system architecture and logic in software ahead of assembling the physical robot.

---

## Implementation

The entire system was implemented in **Embedded C** using **Atmel Studio 7**, with **direct register-level programming** — without Arduino libraries or HAL abstraction layers — to maintain full control over hardware peripherals. This approach allowed precise configuration of:

- Data Direction Registers (DDR) for GPIO configuration
- Timer Control Registers (`TCCR0A`, `TCCR0B`) for PWM setup
- Output Compare Registers (`OCR0A`, `OCR0B`) for duty cycle control
- Port registers (`PORTB`, `PORTD`, `PORTC`) for sensor reading and motor direction control

No full assembly code was used; however, the low-level register programming style closely mirrors assembly-level control, providing direct and deterministic hardware interaction without compiler abstraction overhead.

---

## Testing & Results

The robot was tested systematically under different track conditions on a continuous black-line course printed on white paper: straight path tracking, gradual left/right turns, sharp edge detection, and line-loss condition.

| Test Case | Expected Result | Actual Result |
|---|---|---|
| Straight line | Smooth forward motion with no deviation | Achieved stable forward movement with minimal drift |
| Slight left turn | Small left correction without overshoot | Accurate and quick left correction observed |
| Slight right turn | Small right correction without overshoot | Accurate and quick right correction observed |
| Sharp edge / curve | Proper turning, no overshoot on edges | Successfully followed tight curve edges with PID control |
| Line loss | Robot should stop immediately | Robot stopped immediately upon line loss — confirmed reliable |

**Overall analysis:** The system demonstrated high tracking accuracy on continuous paths with stable motion and minimal deviation. Direct digital sensor reading combined with PWM-based motor control resulted in fast response times, maintaining reliable line following during repeated test runs. The PID-based control approach showed superior smoothness on curves and reduced oscillation compared to the rule-based method. Both approaches achieved consistent performance without significant errors or instability under normal operating conditions. No numerical accuracy percentages are reported.

---

## Rule-Based vs. PID Comparison

| Parameter | Rule-Based | PID |
|---|---|---|
| Operation | Stable and predictable | Smoother corrections, improved tracking accuracy |
| Curved-path performance | Reliable under straightforward conditions | Superior smoothness on curves, reduced oscillation |
| Tuning | Easier to tune | Required iterative experimental tuning of Kp, Ki, Kd |
| Reliability | Reliable, predictable performance | Consistent performance, no significant instability |

Overall, the report concludes the **PID-based approach demonstrated superior performance** in terms of smoothness and curve-tracking, while the **rule-based approach was easier to tune** and provided predictable behavior on straightforward track conditions.

---

## Innovation / Engineering Complexity

**Innovation:** Rather than simple fixed-speed on/off motor control, the system employs adaptive PWM-based speed control that dynamically modulates individual wheel speeds based on sensor input. Both rule-based logic and PID control were fully implemented and experimentally compared on the same hardware platform — providing quantitative performance evaluation, which the report notes is uncommon in undergraduate semester projects.

**Integration:** The project achieves tight hardware-software integration across multiple subsystems operating simultaneously in real time:
- Five IR sensors provide continuous position feedback via GPIO digital inputs
- Timer0 generates precise PWM signals for dual-channel motor speed control
- The L298 motor driver translates low-power MCU logic signals into high-current motor drive
- The control loop integrates all subsystems at 200 Hz for real-time autonomous navigation

**Complexity dimensions:**
- Multi-sensor input: simultaneous reading and interpretation of five IR sensors
- Real-time control: sensor-to-motor update cycle of approximately 5 ms, requiring deterministic timing
- Dual algorithm implementation: rule-based and PID control both implemented, tuned, and tested
- Hardware-software co-design performed in parallel (circuit design, embedded programming, testing)
- Parameter tuning: PID gains (Kp, Ki, Kd) required iterative experimental tuning

---

## Recommended Repository Structure

> The structure below is a **suggested** organization for hosting this project on GitHub. It does not represent an existing repository layout, as no repository file structure is specified in the report.

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

---

## How to Run / Build the Project

Based on the tools and workflow described in the report:

1. Open the project source in **Atmel Studio 7**.
2. Compile the Embedded C code to generate the `.hex` file.
3. (Optional) Verify circuit design and control logic using **Proteus 8** simulation before programming hardware.
4. Flash the generated `.hex` file to the ATmega328P using **AVRDUDE** via a USB programmer.
5. Power the assembled robot (ATmega328P, IR sensor array, L298 driver, motors, battery pack) and place it on a black-line track for testing.

Specific programmer hardware models, COM port settings, or exact AVRDUDE command-line flags are not specified in the report and are therefore not included here.

---

## Safety Considerations

- The system operates at low, safe voltage levels — 5V for MCU and sensor logic, and a regulated motor supply from the 11.1V battery pack — within safe limits for handling during assembly and testing.
- The L298 motor driver provides electrical isolation between the microcontroller and high-current motor circuits, preventing damage to the ATmega328P from inductive motor loads or current spikes.
- All connections were made with wire gauges rated for expected current levels.
- The lithium-ion battery pack was handled according to standard Li-ion safety guidelines (no overcharging, no short circuits).
- Structured, well-commented embedded C code and hardware timer-based delays (replacing unreliable software delays) improve maintainability and timing determinism.
- The system underwent repeated testing across multiple track runs to verify consistent behavior before final demonstration.

---

## Team Members

| Member | Reg. No. | Contribution |
|---|---|---|
| Muhammad Zain | 502138 | Participated in hardware implementation and prepared the project report |
| Muhammad Umair | 506068 | Participated in hardware implementation and developed the Proteus simulation |
| Moeen ul Haq Farooqi | 522745 | Participated in hardware implementation and prepared the project presentation (PPT) |
| Mubashir Qureshi | 509662 | Participated in hardware implementation and assisted in testing and overall project development |

All team members participated in project planning, integration testing, and final demonstration preparation, with regular team meetings held to review progress, resolve technical issues, and coordinate hardware-software integration milestones.

---

## Academic Information

- **Course:** MPS EE-222: Microprocessor Systems
- **Lab Instructor:** Engr. Muhammad Adnan
- **Department:** Electrical Engineering
- **Date:** 12th May 2026

---

## Possible Future Improvements

> The report does not include an explicit "future improvements" section. The following are general suggestions and are **not** claimed as implemented or planned features of this project.

- Extending the PID implementation with auto-tuning or adaptive gain adjustment.
- Adding wireless telemetry for real-time monitoring of sensor and control data.

---

## Conclusion

This project successfully designed and implemented an autonomous line-following robot using the ATmega328P microcontroller, a five-sensor IR array, and PWM-based dual motor control via the L298 motor driver. The system operates in real time by continuously reading sensor inputs at approximately 200 Hz and dynamically adjusting motor speed and direction to accurately follow a predefined black-line path.

Both rule-based logic and PID control strategies were implemented in embedded C and experimentally evaluated. The PID-based approach demonstrated superior performance, delivering smoother directional corrections, improved stability on curved sections, and reduced oscillation compared to the simpler rule-based method. The rule-based approach, however, proved easier to tune and provided reliable, predictable performance under straightforward track conditions.

The project provided comprehensive hands-on experience with the ATmega328P microcontroller, embedded C programming, and hardware-software integration — applying theoretical concepts including PWM generation, timer configuration, GPIO interfacing, ADC operation, feedback control, and Ohm's Law calculations to a real engineering problem.

---

## References

1. [Microchip Technology — ATmega328P Datasheet](https://www.microchip.com/en-us/product/ATmega328P). Microchip Technology Inc., 2020.
2. [STMicroelectronics — L298 Dual Full-Bridge Driver Datasheet](https://www.st.com/resource/en/datasheet/l298.pdf).
3. [Design and Implementation of a Line Follower Robot Using PID Controller](https://www.researchgate.net/publication/335445451_Design_and_Implementation_of_a_Line_Follower_Robot_Using_PID_Controller).
4. [Development of an Autonomous Line Follower Robot Based on AVR Microcontroller](https://www.researchgate.net/publication/343437940_Development_of_an_Autonomous_Line_Follower_Robot_Based_on_AVR_Microcontroller).
5. Mazidi, M. A., Naimi, S., & Naimi, S. *AVR Microcontroller and Embedded Systems Using Assembly and C*. Pearson Education, 2011.
6. Ogata, K. *Modern Control Engineering*. Pearson Education, 2010.
