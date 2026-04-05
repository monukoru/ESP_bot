#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t controlByte;

// ---------- Motor Direction ----------
void left_forward()   { PORTD |=  (1 << PD2); }  // setting high 
void left_backward()  { PORTD &= ~(1 << PD2); }  // setting low 

void right_forward()  { PORTD |=  (1 << PD3); }
void right_backward() { PORTD &= ~(1 << PD3); }

// ---------- Motor Movements ----------
void motors_stop()
{
    OCR0A = 0;
    OCR0B = 0;
}

void motors_forward()
{
    left_forward();
    right_forward();
    OCR0A = 200;
    OCR0B = 200;
}

void motors_backward()
{
    left_backward();
    right_backward();
    OCR0A = 200;
    OCR0B = 200;
}

void motors_left()
{
    left_backward();
    right_forward();
    OCR0A = 200;
    OCR0B = 200;
}

void motors_right()
{
    left_forward();
    right_backward();
    OCR0A = 200;
    OCR0B = 200;
}

//  SPI Interrupt The main logic 
ISR(SPI_STC_vect)
{
    controlByte = SPDR;

    uint8_t f = controlByte & (1<<0);
    uint8_t b = controlByte & (1<<1);
    uint8_t l = controlByte & (1<<2);
    uint8_t r = controlByte & (1<<3);

    if(f && !b)
        motors_forward();
    else if(b && !f)
        motors_backward();
    else if(l)
        motors_left();
    else if(r)
        motors_right();
    else
        motors_stop();
}

// SPI Slave Init
void SPI_SlaveInit()
{
    DDRB |= (1 << PB4);                 // MISO output
    SPCR = (1 << SPE) | (1 << SPIE);    // Enable SPI + Interrupt
    sei();
}

//Generating PWM  
void PWM_Init()
{
    DDRD |= (1 << PD5) | (1 << PD6);    // PWM pins output

    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
    TCCR0A |= (1 << WGM00) | (1 << WGM01);
    TCCR0B |= (1 << CS01);              // Prescaler 8
}

//
int main(void)
{
    DDRD |= (1 << PD2) | (1 << PD3);    // Direction pins output

    PWM_Init();
    SPI_SlaveInit();


}