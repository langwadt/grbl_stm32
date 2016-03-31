#ifdef GRBL_PLATFORM
#error "cpu_map already defined: GRBL_PLATFORM=" GRBL_PLATFORM
#endif


// much of the stuff in this file is not really used for STM32

#define GRBL_PLATFORM "STM32F411"

#if 0
// Define serial port pins and interrupt vectors.
//#define SERIAL_RX     USART_RX_vect
//#define SERIAL_UDRE   USART_UDRE_vect

// Define step pulse output pins. NOTE: All step bit pins must be on the same port.
//#define STEP_DDR        DDRD
//#define STEP_PORT       PORTD
#define X_STEP_BIT      2  // Uno Digital Pin 2
#define Y_STEP_BIT      3  // Uno Digital Pin 3
#define Z_STEP_BIT      4  // Uno Digital Pin 4
#define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

// Define step direction output pins. NOTE: All direction pins must be on the same port.
//#define DIRECTION_DDR     DDRD
//#define DIRECTION_PORT    PORTD
#define X_DIRECTION_BIT   5  // Uno Digital Pin 5
#define Y_DIRECTION_BIT   6  // Uno Digital Pin 6
#define Z_DIRECTION_BIT   7  // Uno Digital Pin 7
//#define DIRECTION_MASK    ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits

//// Define stepper driver enable/disable output pin.
//#define STEPPERS_DISABLE_DDR    DDRB
//#define STEPPERS_DISABLE_PORT   PORTB
//#define STEPPERS_DISABLE_BIT    0  // Uno Digital Pin 8
//#define STEPPERS_DISABLE_MASK   (1<<STEPPERS_DISABLE_BIT)

// Define homing/hard limit switch input pins and limit interrupt vectors. 
// NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
//#define LIMIT_DDR        DDRB
//#define LIMIT_PIN        PINB
//#define LIMIT_PORT       PORTB
#define X_LIMIT_BIT      1  // Uno Digital Pin 9
#define Y_LIMIT_BIT      2  // Uno Digital Pin 10
#define Z_LIMIT_BIT	     4 // Uno Digital Pin 12

//#define LIMIT_MASK       ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits
//#define LIMIT_INT        PCIE0  // Pin change interrupt enable pin
//#define LIMIT_INT_vect   PCINT0_vect
//#define LIMIT_PCMSK      PCMSK0 // Pin change interrupt register

//// Define spindle enable and spindle direction output pins.
//#define SPINDLE_ENABLE_DDR    DDRB
//#define SPINDLE_ENABLE_PORT   PORTB
//// Z Limit pin and spindle PWM/enable pin swapped to access hardware PWM on Pin 11.
//#ifdef VARIABLE_SPINDLE
//  #ifdef USE_SPINDLE_DIR_AS_ENABLE_PIN
//    // If enabled, spindle direction pin now used as spindle enable, while PWM remains on D11.
//    #define SPINDLE_ENABLE_BIT    5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
//  #else
//    #define SPINDLE_ENABLE_BIT    3  // Uno Digital Pin 11
//  #endif
//#else
//  #define SPINDLE_ENABLE_BIT    4  // Uno Digital Pin 12
//#endif
//#ifndef USE_SPINDLE_DIR_AS_ENABLE_PIN
//  #define SPINDLE_DIRECTION_DDR   DDRB
//  #define SPINDLE_DIRECTION_PORT  PORTB
//  #define SPINDLE_DIRECTION_BIT   5  // Uno Digital Pin 13 (NOTE: D13 can't be pulled-high input due to LED.)
//#endif
  
// Define flood and mist coolant enable output pins.
// NOTE: Uno analog pins 4 and 5 are reserved for an i2c interface, and may be installed at
// a later date if flash and memory space allows.
//#define COOLANT_FLOOD_DDR   DDRC
//#define COOLANT_FLOOD_PORT  PORTC
//#define COOLANT_FLOOD_BIT   3  // Uno Analog Pin 3
//#ifdef ENABLE_M7 // Mist coolant disabled by default. See config.h to enable/disable.
//  #define COOLANT_MIST_DDR   DDRC
//  #define COOLANT_MIST_PORT  PORTC
//  #define COOLANT_MIST_BIT   4 // Uno Analog Pin 4
//#endif

// Define user-control controls (cycle start, reset, feed hold) input pins.
// NOTE: All CONTROLs pins must be on the same port and not on a port with other input pins (limits).
//#define CONTROL_DDR       DDRC
//#define CONTROL_PIN       PINC
//#define CONTROL_PORT      PORTC
#define RESET_BIT         0  // Uno Analog Pin 0
#define FEED_HOLD_BIT     1  // Uno Analog Pin 1
#define CYCLE_START_BIT   2  // Uno Analog Pin 2
#define SAFETY_DOOR_BIT   3  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
//#define CONTROL_INT       PCIE1  // Pin change interrupt enable pin
//#define CONTROL_INT_vect  PCINT1_vect
//#define CONTROL_PCMSK     PCMSK1 // Pin change interrupt register
#define CONTROL_MASK      ((1<<RESET_BIT)|(1<<FEED_HOLD_BIT)|(1<<CYCLE_START_BIT)|(1<<SAFETY_DOOR_BIT))
#define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.
  
//// Define probe switch input pin.
//#define PROBE_DDR       DDRC
//#define PROBE_PIN       PINC
//#define PROBE_PORT      PORTC
#define PROBE_BIT       0  // Uno Analog Pin 5
#define PROBE_MASK      (1<<PROBE_BIT)
//
//// Start of PWM & Stepper Enabled Spindle
//#ifdef VARIABLE_SPINDLE
//  // Advanced Configuration Below You should not need to touch these variables
//  #define PWM_MAX_VALUE     255.0 // Don't change. 328p fast PWM mode fixes top value as 255.
//  #define TCCRA_REGISTER	  TCCR2A
//  #define TCCRB_REGISTER	  TCCR2B
//  #define OCR_REGISTER      OCR2A
//  #define COMB_BIT	        COM2A1
//
//  // 1/8 Prescaler, 8-bit Fast PWM mode. Translates to about 7.8kHz PWM frequency.
//  #define TCCRA_INIT_MASK   ((1<<WGM20) | (1<<WGM21))
//  #define TCCRB_INIT_MASK   (1<<CS21)
//
//  // NOTE: On the 328p, these must be the same as the SPINDLE_ENABLE settings.
//  #define SPINDLE_PWM_DDR	  DDRB
//  #define SPINDLE_PWM_PORT  PORTB
//  #define SPINDLE_PWM_BIT	  3    // Uno Digital Pin 11
//#endif // End of VARIABLE_SPINDLE

#endif

#define X_STEP_BIT      0  // Uno Digital Pin 2
#define Y_STEP_BIT      1  // Uno Digital Pin 3
#define Z_STEP_BIT      2  // Uno Digital Pin 4
#define STEP_MASK       ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits

#define X_DIRECTION_BIT   3  // Uno Digital Pin 5
#define Y_DIRECTION_BIT   4  // Uno Digital Pin 6
#define Z_DIRECTION_BIT   5  // Uno Digital Pin 7

#define X_LIMIT_BIT      0  // Uno Digital Pin 9
#define Y_LIMIT_BIT      1  // Uno Digital Pin 10
#define Z_LIMIT_BIT	     2 // Uno Digital Pin 12


#define RESET_BIT         0  // Uno Analog Pin 0
#define FEED_HOLD_BIT     1  // Uno Analog Pin 1
#define CYCLE_START_BIT   2  // Uno Analog Pin 2
#define SAFETY_DOOR_BIT   3  // Uno Analog Pin 1 NOTE: Safety door is shared with feed hold. Enabled by config define.
#define CONTROL_MASK      ((1<<RESET_BIT)|(1<<FEED_HOLD_BIT)|(1<<CYCLE_START_BIT)|(1<<SAFETY_DOOR_BIT))
//#define CONTROL_INVERT_MASK   CONTROL_MASK // May be re-defined to only invert certain control pins.
#define CONTROL_INVERT_MASK   0 // May be re-defined to only invert certain control pins.

#define PROBE_BIT       0  // Uno Analog Pin 5
#define PROBE_MASK      (1<<PROBE_BIT)

#define STANDARD_GRBL

#ifdef STANDARD_GRBL

	#define DIRX     GPIOB,GPIO_Pin_4
	#define DIRY     GPIOB,GPIO_Pin_10
	#define DIRZ     GPIOA,GPIO_Pin_8

	#define STPX     GPIOA,GPIO_Pin_10
	#define STPY     GPIOB,GPIO_Pin_3
	#define STPZ     GPIOB,GPIO_Pin_5

	#define STPEN    GPIOA,GPIO_Pin_9

//	#define SPI1SCK  GPIOA,GPIO_Pin_5
//	#define SPI1MOSI GPIOA,GPIO_Pin_7
//	#define SPI1MISO GPIOA,GPIO_Pin_6
//	#define SPI1NSS  GPIOB,GPIO_Pin_6

//	#define STP_RST GPIOA,GPIO_Pin_9
//	#define STP_FLG GPIOA,GPIO_Pin_10

	#define TESTP GPIOB,GPIO_Pin_8

	#define LIMX GPIOC,GPIO_Pin_7
	#define LIMY GPIOB,GPIO_Pin_6
	#define LIMZ GPIOA,GPIO_Pin_7

	#define PROBE GPIOC,GPIO_Pin_0

	#ifndef VARIABLE_SPINDLE
	#define SPINDLE_EN   GPIOA,GPIO_Pin_6
	#define SPINDLE_DIR  GPIOA,GPIO_Pin_5
	#endif

	#define FLOOD_COOLANT GPIOB,GPIO_Pin_0
//	#define MIST_COOLANT

	#define RESET_PIN        GPIOA,GPIO_Pin_0
	#define FEED_HOLD_PIN    GPIOA,GPIO_Pin_1
	#define CYCLE_START_PIN  GPIOA,GPIO_Pin_4
//	#define SAFETY_DOOR_PIN

#else

	#define DIRX     GPIOB,GPIO_Pin_5
	#define STPX     GPIOB,GPIO_Pin_3
	#define DIRY     GPIOA,GPIO_Pin_8
	#define STPY     GPIOC,GPIO_Pin_7
	#define STPZ     GPIOB,GPIO_Pin_10
	#define DIRZ     GPIOB,GPIO_Pin_4

	#define SPI1SCK  GPIOA,GPIO_Pin_5
	#define SPI1MOSI GPIOA,GPIO_Pin_7
	#define SPI1MISO GPIOA,GPIO_Pin_6
	#define SPI1NSS  GPIOB,GPIO_Pin_6

	#define STP_RST GPIOA,GPIO_Pin_9
	#define STP_FLG GPIOA,GPIO_Pin_10

	#define TESTP GPIOB,GPIO_Pin_8

	#define LIMX GPIOA,GPIO_Pin_0
	#define LIMY GPIOA,GPIO_Pin_1
	#define LIMZ GPIOA,GPIO_Pin_4

	#define PROBE GPIOC,GPIO_Pin_1

	#ifndef VARIABLE_SPINDLE
	#define SPINDLE_EN   GPIOC,GPIO_Pin_1
	#define SPINDLE_DIR  GPIOC,GPIO_Pin_1
	#endif
	//#define FLOOD_COOLANT
	//#define MIST_COOLANT

	//#define RESET_PIN
	//#define FEED_HOLD_PIN
	//#define CYCLE_START_PIN
	//#define SAFETY_DOOR_PIN


	#define XCURRENT 1500
	#define YCURRENT 1500
	#define ZCURRENT  500

#endif

