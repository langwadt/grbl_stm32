/*
  delay.c - replacement for the avr library of the same name to provide
  dummy functions

  Part of Grbl Simulator

  Copyright (c) 2012 Jens Geisler
  Copyright (c) 2015 Adam Shelly

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

// for 100 MHz STM32F4
#define COUNTS_PER_MICROSECOND 33
void _delay_us(int d)
{
	unsigned int count = d * COUNTS_PER_MICROSECOND - 2;
	__asm volatile(" mov r0, %[count]  \n\t"
			"1: subs r0, #1            \n\t"
			"   bhi 1b                 \n\t"
			:
			: [count] "r" (count)
			  : "r0");
}

void _delay_ms(int d)
{
	while (d--) _delay_us(999);
}
