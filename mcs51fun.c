#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define F_CPU 8000000UL
#include <util/delay.h>

#include "mcs51rom.h"


#define PSEN_HIGH (PIND & _BV(0))
#define RD_HIGH (PIND & _BV(1))
#define WR_HIGH (PINC & _BV(6))


#define set_data_input() do {\
    DDRB &= ~_BV(1); DDRB &= ~_BV(2); DDRB &= ~_BV(3); DDRB &= ~_BV(4);\
    DDRF &= ~_BV(4); DDRF &= ~_BV(5); DDRF &= ~_BV(6); DDRF &= ~_BV(7);\
  } while (0)

#define set_data_output() do {\
    DDRB |= _BV(1); DDRB |= _BV(2); DDRB |= _BV(3); DDRB |= _BV(4);\
    DDRF |= _BV(4); DDRF |= _BV(5); DDRF |= _BV(6); DDRF |= _BV(7);\
  } while (0)

#define write_data(VALUE) do {\
    PORTB = PORTB & ~(0x0f<<1) | (VALUE & 0x0f)<<1;\
    PORTF = PORTF & ~0xf0 | VALUE & 0xf0;\
  } while (0)

#define read_data() (PINF & 0xf0 | (PINB>>1 & 0x0f))

#define read_address_high() \
  (PIND & _BV(7) ? PIND>>2 & 0x07 + 0x80 : PIND>>2 & 0x07)


uint8_t mcs51ram[2096];


/* /PSEN rising edge */
ISR(INT0_vect, ISR_NAKED) {
  set_data_input();
  reti();
}

/* /RD rising edge */
ISR(INT1_vect, ISR_NAKED) {
  set_data_input();
  reti();
}

/* /ALE falling edge */
ISR(INT6_vect) {
  uint16_t address;
  uint8_t data;

  address = read_data() | read_address_high()<<8;

  for (;;) {
    if ( !PSEN_HIGH ) {
      write_data(pgm_read_byte(&(mcs51rom[address])));
      set_data_output();
      break;
    }
    else if ( !RD_HIGH ) {
      write_data(mcs51ram[address]);
      set_data_output();
      break;
    }
    else if ( !WR_HIGH ) {
      data = read_data();
      mcs51ram[address] = data;
      break;
    }
  }
}


int main() {
  /* activate reset */
  PORTB |= _BV(5);

  /* setup reset and clock pins */
  DDRB |= _BV(6) | _BV(5);

  /* setup clock */
  TCCR1A = _BV(COM1B0);
  TCCR1B = _BV(WGM12) | _BV(CS10);
  OCR1A = 13 - 1;

  /* setup interrupts */
  EICRA = _BV(ISC11) | _BV(ISC10) | _BV(ISC01) | _BV(ISC00);
  EICRB = _BV(ISC61);
  EIMSK = _BV(INT6) | _BV(INT1) | _BV(INT0);
  sei();

  /* wait then deactivate reset */
  _delay_ms(500);
  PORTB &= ~_BV(5);

  for (;;) {
  }
}
