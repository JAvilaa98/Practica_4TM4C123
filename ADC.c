/*
 * ADC.c
 Jesús Avila - 283216
 */
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{
    /*
    Usando el modulo 0 y 1 , configurar la tarjeta a la frecuencia asignada, para adquirir 6 señales analógicas a una velocidad de 1 millón de muestras por segundo,
    por los canales asignados  y guardar los valores en un arreglo para ser enviados con un botones  externos asociado al gpio D a través del protocolo de comunicación asíncrona 
    a una velocidad de 115200 todo esto usando interrupciones:
    
    Desarrollando inciso a) 1,3,4,5,11,9 - 55MHz - 115200 - secuenciador 2 y secuenciador 1
    .
    */
    
    SYSCTL->RCGCADC = (1<<0)|(1<<1); //Habilitamos módulo 0 y módulo 1 de reloj del adc RCGCADC. Pág 352
    //Pag 382 (RGCGPIO) Puertos base habilitación del reloj
    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (0<<5)|(1<<4)|(1<<3)|(0<<2)|(1<<1)|(0<<0)|(1<<12)|(1<<8); // Encendemos el puerto D --> GPIO, el puerto E, D Y B para los canales
    //Pag 801 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
    GPIOE_AHB->DIR = (0<<4) | (0<<2) | (0<<0); //PE0 Canal 3, PE2 Canal 1 Y PE4 Canal 9
    GPIOD_AHB->DIR = (0<<3) | (0<<2); //PD2 Canal 5 y PD3 Canal 4
    GPIOB_AHB->DIR = (0<<5); //PB5 Canal 11
    //(GPIOAFSEL) pag.770 Enable alternate función para que el modulo analógico tenga control de esos pines AFSEL
    GPIOE_AHB->AFSEL = (0<<4) | (0<<2) | (0<<0); //PE0 Canal 3, PE2 Canal 1 Y PE4 Canal 9
    GPIOD_AHB->AFSEL = (0<<3) | (0<<2); //PD2 Canal 5 y PD3 Canal 4
    GPIOB_AHB->AFSEL = (0<<5); //PB5 Canal 11
    //(GPIODEN) pag.781 desabilita el modo digital
    GPIOE_AHB->DEN = (0<<4) | (0<<2) | (0<<0); //PE0 Canal 3, PE2 Canal 1 Y PE4 Canal 9
    GPIOD_AHB->DEN = (0<<3) | (0<<2); //PD2 Canal 5 y PD3 Canal 4
    GPIOB_AHB->DEN = (0<<5); //PB5 Canal 11
    //Pag 787 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFFF0F0F0); //Habilitamos pines 0, 2 y 4 del puerto E
    GPIOD_AHB->PCTL = GPIOE_AHB->PCTL & (0xFFFF00FF); //Habilitamos pines 2 y 1 del puerto D
    GPIOB_AHB->PCTL = GPIOB_AHB->PCTL & (0XFF0FFFFF); //Habilitamos pin 5 del puerto B
    
    //(GPIOAMSEL) pag.786 habilitar analogico
    GPIOE_AHB->AMSEL = (0<<4) | (0<<2) | (0<<0); //PE0 Canal 3, PE2 Canal 1 Y PE4 Canal 9
    GPIOD_AHB->AMSEL = (0<<3) | (0<<2); //PD2 Canal 5 y PD3 Canal 4
    GPIOB_AHB->AMSEL = (0<<5); //PB5 Canal 11

    //Pag 1159 El registro (ADCPC) establece la velocidad de conversión por segundo
    ADC0->PC = (0<<3)|(1<<2)|(1<<1)|(1<<0);// 1 Millón de muestras
    ADC1->PC = (0<<3)|(1<<2)|(1<<1)|(1<<0);// 1 Millón de muestras
    //Pag 1099 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC0->SSPRI = 0x3021; // El secuenciador 2 tiene prioridad en el adc0
    ADC1->SSPRI = 0X2103; // El secuenciador 1 tiene prioridad en el adc1
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);
    ADC1->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);
    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0x0000);
    ADC1->EMUX  = (0X0000);
    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    ADC0->SSMUX2 = 0x0420; //Colocamos los pines del puerto E
    ADC1->SSMUX1 = 0x0532; //Colocamos los pines del puerto D y B
    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    ADC0->SSCTL2 = (1<<2) | (1<<1) | (1<<6) | (1<<5) | (1<<10) | (1<<9); ///// Según los mux hay que colocar el el bit de muestreo e interrupción en función a los canales que hay configurados |MUX3|MUX2|MUX1|MUX0|
    ADC1->SSCTL1 = (1<<2) | (1<<1) | (1<<6) | (1<<5) | (1<<10) | (1<<9);
    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<2); /* Unmask ADC0 sequence 2 interrupt pag 1082*/
    ADC1->IM |= (1<<1);
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (0<<1) | (0<<0);
    ADC1->ACTSS = (0<<2) | (0<<2) | (1<<1) | (0<00);


    ADC0->PSSI |= (1<<2);
    ADC1->PSSI |= (1<<1);
}
extern void ADC0_InSeq2(uint16_t *Result){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI = 0x00000004;
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[1] = ADC0->SSFIFO2&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[0] = ADC0->SSFIFO2&0xFFF;
       printChar('A');
       ADC0->ISC = 0x0004;  //Conversion finalizada

}


