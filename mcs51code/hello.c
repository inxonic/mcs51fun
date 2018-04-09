#include <stdio.h>

#include <mcs51/8051.h>
#include <mcs51/serial_IO.h>


#define MAX 10000


__xdata unsigned char primes[MAX/8];


void print_primes() {
  unsigned int i, j;

  for (i=2; i<MAX; i++) {
    if (!(primes[i/8] & 1<<(i%8))) {
      printf_tiny("%d ", i);
      for(j=2*i; j<MAX; j+=i) primes[j/8] |= 1<<(j%8);
    }
  }
}

void main() {
  unsigned char c;

  printf_tiny("Welcome!\n");

  for (;;) {
    putchar('\n');
    putchar('#');
    putchar(' ');
    c = getchar();
    putchar(c);
    putchar('\n');
    switch (c) {
      case '1':
        P3_4 = 0;
        break;
      case '2':
        P3_4 = 1;
        break;
      case '3':
        P3_5 = 0;
        break;
      case '4':
        P3_5 = 1;
        break;
      case 'p':
        print_primes();
        break;
      default:
        printf_tiny("Invalid command.\n");
    }
  }
}
