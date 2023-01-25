
#ifndef LIB_INCLUDE_H_
#define LIB_INCLUDE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _40MHZ 9
#define _20MHZ 19
#define _10MHZ 39
#define _50MHZ 0x07
#define _25MHZ 0x0F
#define _80MHZ 0x04 
 
 
  volatile uint16_t Result[3];
  volatile uint16_t Result_2[2];
  volatile uint16_t Result_3[1];
  

#include "TM4C123GH6PM.h"
#include "GPIO.h"
#include "PLL.h"
#include "UART.h"
#include "ADC.h"


#endif /* INCLUDE_H_ */