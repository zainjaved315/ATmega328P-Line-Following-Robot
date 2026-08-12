#include <avr/io.h>
#include <util/delay.h>

// ----- Sensor pins -----
#define S1 PC0   // far left
#define S2 PC1   // left
#define S3 PC2   // center
#define S4 PC3   // right
#define S5 PC4   // far right

// ----- Motor speed settings -----
#define SPEED_FORWARD   90     // straight-line duty cycle (~35%)
#define SPEED_TURN_FAST 90     // outer wheel during a turn
#define SPEED_TURN_SLOW 40     // inner wheel during a turn

// ------------------------------------------------------------
// PWM setup — Timer0, Fast PWM, non-inverting, prescaler 64
// ------------------------------------------------------------
void PWM_init(void)
{
    DDRD |= (1 << PD6) | (1 << PD5);   // ENA, ENB as outputs

    TCCR0A |= (1 << WGM00) | (1 << WGM01);              // Fast PWM mode
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);             // non-inverting
    TCCR0B |= (1 << CS01) | (1 << CS00);                 // prescaler 64
}

// ------------------------------------------------------------
// Motor direction: both wheels forward
// ------------------------------------------------------------
void setForward(void)
{
    // Left motor forward
    PORTD |= (1 << PD7);
    PORTB &= ~(1 << PB0);

    // Right motor forward
    PORTB |= (1 << PB1);
    PORTB &= ~(1 << PB2);
}

// ------------------------------------------------------------
// Pivot left: slow the left wheel, keep the right wheel driving
// ------------------------------------------------------------
void turnLeft(void)
{
    OCR0A = SPEED_TURN_SLOW;   // left wheel (ENA)
    OCR0B = SPEED_TURN_FAST;   // right wheel (ENB)
}

// ------------------------------------------------------------
// Pivot right: slow the right wheel, keep the left wheel driving
// ------------------------------------------------------------
void turnRight(void)
{
    OCR0A = SPEED_TURN_FAST;
    OCR0B = SPEED_TURN_SLOW;
}

// ------------------------------------------------------------
// Stop both motors (line lost)
// ------------------------------------------------------------
void stopMotors(void)
{
    OCR0A = 0;
    OCR0B = 0;
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main(void)
{
    // Motor pins as output; PORTC (sensors) left as input
    DDRD |= (1 << PD7);
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

    PWM_init();

    while (1)
    {
        // ----- Read sensors -----
        int s1 = (PINC & (1 << S1)) ? 1 : 0;
        int s2 = (PINC & (1 << S2)) ? 1 : 0;
        int s3 = (PINC & (1 << S3)) ? 1 : 0;
        int s4 = (PINC & (1 << S4)) ? 1 : 0;
        int s5 = (PINC & (1 << S5)) ? 1 : 0;

        setForward();

        // ----- Control logic -----
        if (s3 == 1)
        {
            // Center sensor sees the line -> go straight
            OCR0A = SPEED_FORWARD;
            OCR0B = SPEED_FORWARD;
        }
        else if (s1 == 1 || s2 == 1)
        {
            // Line drifted left -> correct left
            turnLeft();
        }
        else if (s4 == 1 || s5 == 1)
        {
            // Line drifted right -> correct right
            turnRight();
        }
        else
        {
            // No sensor sees the line -> stop
            stopMotors();
        }

        _delay_ms(5);   // ~200 Hz control loop
    }
}
