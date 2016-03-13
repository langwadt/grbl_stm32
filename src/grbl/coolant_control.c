/*
  coolant_control.c - coolant control methods
  Part of Grbl

  Copyright (c) 2012-2015 Sungeun K. Jeon

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

#include "grbl.h"


void coolant_init()
{
//  COOLANT_FLOOD_DDR |= (1 << COOLANT_FLOOD_BIT);
//  #ifdef ENABLE_M7
//    COOLANT_MIST_DDR |= (1 << COOLANT_MIST_BIT);
//  #endif

  #ifdef FLOOD_COOLANT
  set_as_output(FLOOD_COOLANT);
  #endif

  #ifdef ENABLE_M7
     set_as_output(MIST_COOLANT);
  #endif

  coolant_stop();
}


void coolant_stop()
{
//  COOLANT_FLOOD_PORT &= ~(1 << COOLANT_FLOOD_BIT);
//  #ifdef ENABLE_M7
//    COOLANT_MIST_PORT &= ~(1 << COOLANT_MIST_BIT);
//  #endif
#ifdef FLOOD_COOLANT
	GPIO_ResetBits(FLOOD_COOLANT);
#endif
#ifdef ENABLE_M7
	GPIO_ResetBits(MIST_COOLANT);
#endif
}


void coolant_set_state(uint8_t mode)
{
  if (sys.abort) { return; } // Block during abort.
  
//  if (mode == COOLANT_FLOOD_ENABLE) {
//    COOLANT_FLOOD_PORT |= (1 << COOLANT_FLOOD_BIT);
//
//  #ifdef ENABLE_M7
//    } else if (mode == COOLANT_MIST_ENABLE) {
//      COOLANT_MIST_PORT |= (1 << COOLANT_MIST_BIT);
//  #endif
  if (mode == COOLANT_FLOOD_ENABLE) {
#ifdef FLOOD_COOLANT
	GPIO_SetBits(FLOOD_COOLANT);
#endif
  #ifdef ENABLE_M7  
	} else if (mode == COOLANT_MIST_ENABLE) {
		GPIO_SetBits(MIST_COOLANT);
  #endif
  ;
  } else {
    coolant_stop();
  }
}


void coolant_run(uint8_t mode)
{
  if (sys.state == STATE_CHECK_MODE) { return; }
  protocol_buffer_synchronize(); // Ensure coolant turns on when specified in program.  
  coolant_set_state(mode);
}
