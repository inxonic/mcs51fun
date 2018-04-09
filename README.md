# MCS51Fun

This is a fun project similar to [z80fun](https://github.com/inxonic/z80fun):
An MCS-51 (80C31) microcontroller supported by an AVR, that provides ROM, extended RAM and clock.

## Status

- The hardware is wired on a breadboard.
- It's based on a Pro Micro 3.3V/8MHz board.
- The 80C31 runs at 1/26 the speed of the AVR which is about 308kHz.
- There is some MCS-51 example code written in C.
- It can communicate via the 80C31 integrated UART (at 1600 baud).
- And it can compute primes (just press _p_).
- RAM is essentially limited by the AVR's internal SRAM (2.5kB).
- ROM size is limited by the number of address lines (4kB).

## Schematic

8031     | AVR | Pro Micro Pin
---------|-----|--------------
P0.0/AD0 | PB1 | 15
P0.1/AD1 | PB2 | 16
P0.2/AD2 | PB3 | 14
P0.3/AD3 | PB4 | 8
P0.4/AD4 | PF4 | A3
P0.5/AD5 | PF5 | A2
P0.6/AD6 | PF6 | A1
P0.7/AD7 | PF7 | A0
P2.0/A8  | PD2 | RXI
P2.1/A9  | PD3 | TXO
P2.2/A10 | PD4 | 4
P2.3/A11 | PD7 | 6
ALE      | PE6 | 7
/PSEN    | PD0 | 3
/RD      | PD1 | 2
/WR      | PC6 | 5
RST      | PB5 | 9
XTAL1    | PB6 | 10

8031     | peripherals
---------|------------
P3.0/RxD | Serial-to-USB converter (3.3V)
P3.1/TxD | Serial-to-USB converter (3.3V)
P3.4     | LED and resistor in series connected to VCC
P3.5     | LED and resistor in series connected to VCC
