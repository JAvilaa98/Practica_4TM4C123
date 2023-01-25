#include "lib/include.h"

extern void Configurar_UART2(void) //UART 2
{
    SYSCTL->RCGCUART  = (1<<2);   //Paso 1 (RCGCUART) pag.344 UART/Módulo 2
    SYSCTL->RCGCGPIO |= (1<<3);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port D
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOD -> AFSEL= (1<<7) | (1<<6); 
    // GPIOD->DIR |= (1<<7) | (0<<6); Apagamos el DIR porque no los necesitamos los canales digitales
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOD->PCTL |= (GPIOD->PCTL&0x00FFFFFF) | 0x11000000;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOD->DEN = (1<<6) | (1<<7);//PA1 PA0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART2->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 20,000,000 / (16 * 115200) = 10.8506               ME TOCO 55MHz Y CON 
    UARTFBRD[DIVFRAC] = integer(.8506 * 64 + 0.5) = 54.93 redondeo a 55
    */
    UART2->IBRD = 10;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915 AQUÍ VA LA PARTE ENTERA DE LA DIVISIÓN
    UART2->FBRD = 55;
    //  UART Line Control (UARTLCRH) pag.916   AQUÍ VA LA PARTE FRACCIONARIA MAS CERCANA SEGÚN EL RESULTADO DE LA MULTIPLICACIÓN 
    UART2->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939   HABILITA LA PILA
    UART2->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918  
    UART2->CTL = (1<<0) | (1<<8) | (1<<9);  // DEL 9 HACIA ATRÁS PRENDEMOS TRANSMISIÓN, HABILITACIÓN 

}



