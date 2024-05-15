// PIC16F877A Configuration Bit Settings
// 'C' source line config statements
// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
/*
 * File:   main.c
 * Author: Wodan
 *
 * Created on 14 de Maio de 2024, 15:10
 */


#include <xc.h>

__bit state; // Unsigned integer de 1 bit
void __interrupt() Toggle(void){
    if(INTCONbits.INTF == 1)
    {
        state = ~state;
        INTCONbits.INTF = 0;
        
        if(state){
            CVRCON = 0b11001111;
        } else {
            CVRCON = 0b11000000;
        }
    }
}

void main(void) {
    
    CMCON = 0b00000110; // config do registrador do CCP
    CVRCON = 0b11000000; // config do registrador do VRef
    
    TRISC = 0x00; // habilita e limpa os pinos de output
    TRISB = 0xFF; // habilita todos os pinos de entrada, mas só vamos usar um
    
    ADCON0bits.ADON = 1; // habilita o conversor A/D
    INTCONbits.GIE = 1; // habilita a flag global
    INTCONbits.INTE = 1; // habilita a interrupção por meio externo
    INTCONbits.PEIE = 0; // habilita a interrupção por periféricos
    
    OPTION_REGbits.INTEDG = 0x01; // config da borda de pulso
    
    while(1){
        ADCON0bits.CHS = 0b0000;
        ADCON0bits.GO = 1;
        
        while(ADCON0bits.GO);
        if(CMCONbits.C1OUT) // liga ou desliga led
            PORTCbits.RC2 = 1;
        else
            PORTCbits.RC2 = 0;
            
    }
    return;
}
