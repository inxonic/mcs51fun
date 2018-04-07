#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
#include <util/delay.h>


#define PSEN_HIGH (PIND & _BV(0))
#define RD_HIGH (PIND & _BV(1))
#define WR_HIGH (PINC & _BV(6))


#define DEBUG_0 { PORTD &= ~_BV(7); }
#define DEBUG_1 { PORTD |= _BV(7); }


static inline void set_data_input() {
  DDRB &= ~0x1e;
  DDRF &= ~0xf0;
}

static inline void set_data_output() {
  DDRB |= 0x1e;
  DDRF |= 0xf0;
}

static inline uint8_t read_data() {
  return PINF & 0xf0 | (PINB>>1 & 0x0f);
}

static inline uint16_t read_address() {
  return read_data();
}

static inline void write_data(uint8_t data) {
  PORTB = PORTB & ~(0x0f<<1) | (data & 0x0f)<<1;
  PORTF = PORTF & ~0xf0 | data & 0xf0;
}


/* /PSEN rising edge */
ISR(INT0_vect) {
  set_data_input();
}

/* /RD rising edge */
ISR(INT1_vect) {
  set_data_input();
}

/* /ALE falling edge */
ISR(INT6_vect) {
  uint16_t address;
  uint8_t data;

  address = read_address();

  for (;;) {
    if ( !PSEN_HIGH ) {
      switch ( address & 0x3 ) {
        case 0x00:
          DEBUG_0;
          data = 0xd2;
          break;
        case 0x02:
          DEBUG_0;
          data = 0xc2;
          break;
        case 0x01:
        case 0x03:
          DEBUG_1;
          data = 0x90;
          break;
      }
      write_data(data);
      set_data_output();
      break;
    }
    else if ( !RD_HIGH ) {
      write_data(0x00);
      set_data_output();
      break;
    }
    else if ( !WR_HIGH ) {
      data = read_data();
      break;
    }
  }
}


int main() {
  /* activate reset */
  PORTB |= _BV(6);

  /* setup reset and clock pins */
  DDRB |= _BV(6) | _BV(5);

  /* setup debug port */
  DDRD |= _BV(7);

  /* setup clock */
  TCCR1A = _BV(COM1A0);
  TCCR1B = _BV(WGM12) | _BV(CS10);
  OCR1A = 15 - 1;

  /* setup interrupts */
  EICRA = _BV(ISC11) | _BV(ISC10) | _BV(ISC01) | _BV(ISC00);
  EICRB = _BV(ISC61);
  EIMSK = _BV(INT6) | _BV(INT1) | _BV(INT0);
  sei();

  /* wait then deactivate reset */
  _delay_ms(500);
  PORTB &= ~_BV(6);

  for (;;);
}
