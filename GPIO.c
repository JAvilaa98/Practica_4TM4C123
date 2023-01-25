#include "lib/include.h"
extern void Configurar_GPIO(void)
{
    //Habilitación de puerto 
    SYSCTL->RCGCGPIO |=(1<<0);  //pueto A
    
    //Habilitamos la función de GPIO
    GPIOA->AMSEL = 0x00; 

    //Pines trabajando como GPIO
    GPIOA->PCTL = 0x00000000;   

    //Entradas y salidas
    GPIOA->DIR = (0<<2) | (0<<3) | (0<<4) | (0<<5) | (0<<6) | (0<<7); // PA2 - PA7 son entradas 

    //Desactivar función alternativa
    GPIOA->AFSEL = 0x00;   

    // Pines en configuración pull-up (pág 678)
    GPIOA->PUR = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);// PA5 - PA0

    //Habilitar entradas digitales
    GPIOA->DEN = (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);     

    //Limpiamos pines de interrupciones pag. 667  
    GPIOA -> IM |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);

    GPIOA -> IS |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5); // Detección de flancos

    //Registro para indicar que el IEV va a configurar que flancos detectar (pág 665)
    GPIOA -> IBE |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5); 

    GPIOA -> IEV |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5); //Detección de blanco de subida

    //Establecemos que no ha ocurrido ninguna interrupción en esos pines (pág 668)
    GPIOA -> RIS |= (0<<0) | (0<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);

    //La interrupción del pin es enviada al controlador de interrup (pág 667)
    GPIOA -> IM |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);
 
    //Configurar la interrupción correspondiente al puerto A -> int 0 (pág 104)
    NVIC -> IP[0] = (NVIC ->IP[0]&0xFFFFFF00) | 0x00000002; //0 es el numero de prioridad (n)

    // Habilitar interrupción (pág 142) 
    NVIC -> ISER[0] = 0x00000001; //se pone 1 en la interrupción 0
}
