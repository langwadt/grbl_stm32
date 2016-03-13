/*
  serial.c - Low level functions for sending and recieving bytes via the serial port
  Part of Grbl

  Copyright (c) 2011-2015 Sungeun K. Jeon
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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


#ifdef STM32F4

#define USART_BAUD     (BAUD_RATE)
#define USART_DATA     (USART_WordLength_8b)
#define USART_PARITY   (USART_Parity_No)
#define USART_STOP     (USART_StopBits_1)
#define USART_FLOWCTRL (USART_HardwareFlowControl_None)


#define Open_USART                        USART2
#define Open_USART_CLK                    RCC_APB1Periph_USART2

#define Open_USART_TX_PIN                 GPIO_Pin_2
#define Open_USART_TX_GPIO_PORT           GPIOA
#define Open_USART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define Open_USART_TX_SOURCE              GPIO_PinSource2
#define Open_USART_TX_AF                  GPIO_AF_USART2

#define Open_USART_RX_PIN                 GPIO_Pin_3
#define Open_USART_RX_GPIO_PORT           GPIOA
#define Open_USART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define Open_USART_RX_SOURCE              GPIO_PinSource3
#define Open_USART_RX_AF                  GPIO_AF_USART2

#define Open_USART_IRQn                   USART2_IRQn
#define USARTx_IRQHANDLER                 USART2_IRQHandler





uint8_t serial_rx_buffer[RX_BUFFER_SIZE];
uint8_t serial_rx_buffer_head = 0;
volatile uint8_t serial_rx_buffer_tail = 0;

uint8_t serial_tx_buffer[TX_BUFFER_SIZE];
uint8_t serial_tx_buffer_head = 0;
volatile uint8_t serial_tx_buffer_tail = 0;


#ifdef ENABLE_XONXOFF
  volatile uint8_t flow_ctrl = XON_SENT; // Flow control state variable
#endif
  

// Returns the number of bytes used in the RX serial buffer.
uint8_t serial_get_rx_buffer_count()
{
  uint8_t rtail = serial_rx_buffer_tail; // Copy to limit multiple calls to volatile
  if (serial_rx_buffer_head >= rtail) { return(serial_rx_buffer_head-rtail); }
  return (RX_BUFFER_SIZE - (rtail-serial_rx_buffer_head));
}


// Returns the number of bytes used in the TX serial buffer.
// NOTE: Not used except for debugging and ensuring no TX bottlenecks.
uint8_t serial_get_tx_buffer_count()
{
  uint8_t ttail = serial_tx_buffer_tail; // Copy to limit multiple calls to volatile
  if (serial_tx_buffer_head >= ttail) { return(serial_tx_buffer_head-ttail); }
  return (TX_BUFFER_SIZE - (ttail-serial_tx_buffer_head));
}


void serial_init(void)
{
  GPIO_InitTypeDef       GPIO_InitStructure;
  USART_InitTypeDef      USART_InitStructure;
  USART_ClockInitTypeDef USART_ClockStructure;
  NVIC_InitTypeDef       NVIC_InitStructure;

  //------------------------------------------------------------------------------------------
  RCC_AHB1PeriphClockCmd(Open_USART_TX_GPIO_CLK,ENABLE);
  RCC_AHB1PeriphClockCmd(Open_USART_RX_GPIO_CLK,ENABLE);
  RCC_APB1PeriphClockCmd(Open_USART_CLK,ENABLE);
  RCC_APB1PeriphClockLPModeCmd(Open_USART_CLK,ENABLE);

  //------------------------------------------------------------------------------------------
  GPIO_PinAFConfig(Open_USART_TX_GPIO_PORT, Open_USART_TX_SOURCE, Open_USART_TX_AF);
  GPIO_PinAFConfig(Open_USART_RX_GPIO_PORT, Open_USART_RX_SOURCE, Open_USART_RX_AF);

  //------------------------------------------------------------------------------------------
  GPIO_InitStructure.GPIO_Pin = Open_USART_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //------------------------------------------------------------------------------------------
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(Open_USART_TX_GPIO_PORT, &GPIO_InitStructure);
  //------------------------------------------------------------------------------------------
  GPIO_InitStructure.GPIO_Pin = Open_USART_RX_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Open_USART_RX_GPIO_PORT, &GPIO_InitStructure);

  //------------------------------------------------------------------------------------------
  USART_InitStructure.USART_BaudRate = USART_BAUD;
  USART_InitStructure.USART_WordLength = USART_DATA;
  USART_InitStructure.USART_StopBits = USART_STOP;
  USART_InitStructure.USART_Parity = USART_PARITY;
  USART_InitStructure.USART_HardwareFlowControl = USART_FLOWCTRL;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  //------------------------------------------------------------------------------------------
  USART_Init(Open_USART, &USART_InitStructure);
  USART_ClockStructInit (&USART_ClockStructure);
  USART_ClockInit(Open_USART, &USART_ClockStructure);

  //------------------------------------------------------------------------------------------
  NVIC_InitStructure.NVIC_IRQChannel = Open_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //oder 0?
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //oder 1?
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //USART_ITConfig(Open_USART, USART_IT_TXE, ENABLE);  // TX register Empty - wir können was senden
  USART_ITConfig(Open_USART, USART_IT_RXNE, ENABLE); // RX register Not Empty - es wurde was empfangen

  //------------------------------------------------------------------------------------------
  USART_Cmd(Open_USART, ENABLE);
}



// Writes one byte to the TX serial buffer. Called by main program.
// TODO: Check if we can speed this up for writing strings, rather than single bytes.
void serial_write(uint8_t data) {
  // Calculate next head
  uint8_t next_head = serial_tx_buffer_head + 1;
  if (next_head == TX_BUFFER_SIZE) { next_head = 0; }

  // Wait until there is space in the buffer
  while (next_head == serial_tx_buffer_tail) {
    // TODO: Restructure st_prep_buffer() calls to be executed here during a long print.
    if (sys_rt_exec_state & EXEC_RESET) { return; } // Only check for abort to avoid an endless loop.
  }

  // Store data and advance head
  serial_tx_buffer[serial_tx_buffer_head] = data;
  serial_tx_buffer_head = next_head;

  // Enable Data Register Empty Interrupt to make sure tx-streaming is running
  //UCSR0B |=  (1 << UDRIE0);

  USART_ITConfig(Open_USART, USART_IT_TXE, ENABLE);
}


// Data Register Empty Interrupt handler
//ISR(SERIAL_TX)
void serial_txint()
{
  uint8_t tail = serial_tx_buffer_tail; // Temporary serial_tx_buffer_tail (to optimize for volatile)


  #ifdef ENABLE_XONXOFF
    if (flow_ctrl == SEND_XOFF) {
      //UDR0 = XOFF_CHAR;
    	USART_SendData(Open_USART,XOFF_CHAR);
      flow_ctrl = XOFF_SENT;
    } else if (flow_ctrl == SEND_XON) {
      //UDR0 = XON_CHAR;
   	  USART_SendData(Open_USART,XON_CHAR);
      flow_ctrl = XON_SENT;
    } else
  #endif
  {
    // Send a byte from the buffer
    //UDR0 = serial_tx_buffer[tail];

    if(tail != serial_tx_buffer_head)
    {
    	USART_SendData(Open_USART, serial_tx_buffer[tail]);

    	// Update tail position
    	tail++;
    	if (tail == TX_BUFFER_SIZE) { tail = 0; }

    	serial_tx_buffer_tail = tail;
    }
  }

  // Turn off Data Register Empty Interrupt to stop tx-streaming if this concludes the transfer
  if (tail == serial_tx_buffer_head) { USART_ITConfig(Open_USART, USART_IT_TXE, DISABLE); /*UCSR0B &= ~(1 << UDRIE0);*/ }
}


// Fetches the first byte in the serial read buffer. Called by main program.
uint8_t serial_read()
{
  uint8_t tail = serial_rx_buffer_tail; // Temporary serial_rx_buffer_tail (to optimize for volatile)
  if (serial_rx_buffer_head == tail) {
    return SERIAL_NO_DATA;
  } else {
    uint8_t data = serial_rx_buffer[tail];

    tail++;
    if (tail == RX_BUFFER_SIZE) { tail = 0; }
    serial_rx_buffer_tail = tail;

    #ifdef ENABLE_XONXOFF
      if ((serial_get_rx_buffer_count() < RX_BUFFER_LOW) && flow_ctrl == XOFF_SENT) {
        flow_ctrl = SEND_XON;
        //UCSR0B |=  (1 << UDRIE0); // Force TX
        USART_ITConfig(Open_USART, USART_IT_TXE, ENABLE);
      }
    #endif

    return data;
  }
}


//ISR(SERIAL_RX)
void serial_rxint()
{
  uint8_t data = USART_ReceiveData(Open_USART);//UDR0;
  uint8_t next_head;

  // Pick off realtime command characters directly from the serial stream. These characters are
  // not passed into the buffer, but these set system state flag bits for realtime execution.
  switch (data) {
    case CMD_STATUS_REPORT: system_set_exec_state_flag(EXEC_STATUS_REPORT); break; // Set as true
    case CMD_CYCLE_START:   system_set_exec_state_flag(EXEC_CYCLE_START); break; // Set as true
    case CMD_FEED_HOLD:     system_set_exec_state_flag(EXEC_FEED_HOLD); break; // Set as true
    case CMD_SAFETY_DOOR:   system_set_exec_state_flag(EXEC_SAFETY_DOOR); break; // Set as true
    case CMD_RESET:         mc_reset(); break; // Call motion control reset routine.
    default: // Write character to buffer
      next_head = serial_rx_buffer_head + 1;
      if (next_head == RX_BUFFER_SIZE) { next_head = 0; }

      // Write data to buffer unless it is full.
      if (next_head != serial_rx_buffer_tail) {
        serial_rx_buffer[serial_rx_buffer_head] = data;
        serial_rx_buffer_head = next_head;

        #ifdef ENABLE_XONXOFF
          if ((serial_get_rx_buffer_count() >= RX_BUFFER_FULL) && flow_ctrl == XON_SENT) {
            flow_ctrl = SEND_XOFF;
            USART_ITConfig(Open_USART, USART_IT_TXE, ENABLE);//UCSR0B |=  (1 << UDRIE0); // Force TX
          }
        #endif

      }
      //TODO: else alarm on overflow?
  }
}


void serial_reset_read_buffer() 
{
  serial_rx_buffer_tail = serial_rx_buffer_head;

  #ifdef ENABLE_XONXOFF
    flow_ctrl = XON_SENT;
  #endif
}

//=================================================================================
void USARTx_IRQHANDLER(void)
{
  //===============================================================================
  // USART RX Interrupt Service Routine
  //-------------------------------------------------------------------------------
  if (USART_GetFlagStatus(Open_USART, USART_FLAG_RXNE) == SET)
  {
	  //interrupt_SERIAL_RX();
	  serial_rxint();
  }
  //===============================================================================
  // USART TX Interrupt Service Routine
  //-------------------------------------------------------------------------------
  if (USART_GetFlagStatus(Open_USART, USART_FLAG_TXE) == SET )
  {
	  //interrupt_SERIAL_RX();
	  serial_txint();
  }
}
//=================================================================================

#endif

