
// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = INTOSC_XT     // Oscillator Selection bits (XT oscillator (XT))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 16000000

//VARIÁVEIS P/ MOTOR
#define motor PORTDbits.RD0
#define botao_motor1 PORTDbits.RD2
#define botao_motor2 PORTDbits.RD3
bit flag_botao_motor1, flag_botao_motor2 = 0;
unsigned int duty_motor = 62000;
unsigned char rep_motor_cnt1, rep_motor_cnt2 = 0;

//VARIÁVEIS P/ SERVO
#define servo PORTDbits.RD1
#define botao_servo1 PORTDbits.RD4
#define botao_servo2 PORTDbits.RD5
bit flag_botao_servo1, flag_botao_servo2 = 0;
unsigned int duty_servo = 13;
unsigned char rep_servo_cnt1, rep_servo_cnt2 = 0;

void interrupt isr(void){
        
    if (TMR1IF){
        TMR1IF = 0;
        TMR1 = 58000;
        PORTDbits.RD0 = 1;
    }
    
    if (CCP1IF){
        CCP1IF = 0;
        PORTDbits.RD0 = 0;
    }

    if(TMR2IF){                                                            
        TMR2IF = 0x00;                    
        if(!servo){                            
            PR2 = duty_servo;
            TMR2 = 0;
            servo = 0x01;                   
        }else{                                  
            PR2 = 255 - duty_servo;
            TMR2 = 0;
            servo = 0x00;                      
        }
    }
    
}

void main(){
    OSCCON = 0b01100000;
    GIE = 1;
    PEIE = 1;
    T1CON = 0x01; 
    T2CON = 0b01111101;
    T3CON = 0x00; 
    TMR1 = 58000;    
    CCP1CON = 0b00001010;                    
    CCPR1  = duty_motor;      
    TMR1IE = 1;
    CCP1IE = 1;     
    TMR2IE = 1;
    ADCON0 = 0x80;  
    ADCON1 = 0x8E;    
    ADCON2 = 0b10010110;
    ADCON0bits.ADON = 1; 
    PORTC = 0x00; 
    PORTD = 0x3C;
    TRISC = 0x00;
    TRISD = 0x3C;   
    TMR2ON = 1;
    
    while(1){
        __delay_us(500);
        
        //MOTOR
        CCPR1 = duty_motor;       
        if(!botao_motor1){
          flag_botao_motor1 = 1; rep_motor_cnt1 = 0;
        } else rep_motor_cnt1++;
      
        if(!botao_motor2){
          flag_botao_motor2 = 1; rep_motor_cnt2 = 0;
        } else rep_motor_cnt2++;

        if (botao_motor1 && flag_botao_motor1) {                                                   
            flag_botao_motor1 = 0;
            duty_motor = duty_motor - 80;
        } else if (rep_motor_cnt1 > 10) {
            rep_motor_cnt1 = 0;
            duty_motor = duty_motor - 80;
        }

        if (botao_motor2 && flag_botao_motor2){                                                  
            flag_botao_motor2 = 0;
            duty_motor = duty_motor + 80;
        } else if (rep_motor_cnt2 > 10) {
            rep_motor_cnt2 = 0;
            duty_motor = duty_motor + 80;
        }
           
        if(duty_motor >= 64000) duty_motor = 64000;
        else if (duty_motor < 59000) duty_motor = 59000;   
        
        //SERVO      
        if(!botao_servo1){
            flag_botao_servo1 = 0x01; rep_servo_cnt1 = 0;
        } else rep_servo_cnt1++; 
        
        if(!botao_servo2){
            flag_botao_servo2 = 0x01; rep_servo_cnt2 = 0;
        } else rep_servo_cnt2++;
   
        if(botao_servo1 && flag_botao_servo1){                                                   
            flag_botao_servo1 = 0x00;                                   
            duty_servo = duty_servo + 1;                                        
        } else if (rep_servo_cnt1 > 10){
            rep_servo_cnt1 = 0;
            duty_servo = duty_servo + 1;
        }    
              
        if(botao_servo2 && flag_botao_servo2){                                                   
            flag_botao_servo2 = 0x00;                                   
            duty_servo = duty_servo - 1;                                           
        } else if (rep_servo_cnt2 > 10){
            rep_servo_cnt2 = 0;
            duty_servo = duty_servo - 1;   
        }
        
        if(duty_servo > 40) duty_servo = 40;
        else if (duty_servo < 10) duty_servo = 10;   
    }
                      
}        
