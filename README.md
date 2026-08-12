###Working Principle

The five IR sensors continuously detect the position of the black line relative to the robot.

The ATmega328P reads the sensor signals through its GPIO pins and determines the required correction to the robot's movement.

Based on the selected control algorithm, the microcontroller adjusts the speed and direction of the two motors using PWM signals.

This process is repeated continuously to keep the robot aligned with the predefined path.

###Control Algorithms
Rule-Based Control

The rule-based approach determines the motor response according to the detected sensor pattern.

It provides simple and predictable behavior and is relatively easy to implement and tune.

PID Control

A PID controller was also implemented to improve the robot's tracking performance.

The controller uses the line-position error to calculate a correction based on proportional, integral, and derivative terms.

The motor speeds are then adjusted according to the calculated correction.

Compared with the rule-based approach, the PID controller provided:

Smoother directional corrections
Better performance on curved paths
Reduced oscillation
Improved tracking stability
###Embedded Software

The robot was programmed in Embedded C using Atmel Studio 7.

The implementation uses direct register-level programming on the ATmega328P rather than Arduino libraries or hardware abstraction layers.

Important registers and peripherals used include:

Data Direction Registers (DDR)
PORT registers
Timer/Counter Control Registers
Output Compare Registers
Timer0
GPIO
PWM

Timer0 was configured for PWM-based motor speed control.
