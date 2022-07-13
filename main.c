

#include <xc.h>
#include"config.h"


#define _XTAL_FREQ 8000000
#define PreLoad 55535
#define SevenSeg1 PORTD
#define SevenSeg1_D TRISD

 
unsigned char Check=0;
unsigned char Segments_Code[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned int Digits[4] = {0,0,0,0};
 
void TMR1_Init();
void SevenSeg1_Init();
void SevenSeg1_Write(unsigned int);
void ADC_Init(void);
unsigned int ADC_Read();
 
void main(void) {
    ADC_Init();
    SevenSeg1_Init();
    unsigned int analog_to_digital;
    float Voltage_input;
    
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    
    PORTCbits.RC0 = 0;
    PORTCbits.RC1 =0;
    PORTCbits.RC2 =0;
    PORTCbits.RC3 =0;
    
    while(1)
    {
        analog_to_digital = ADC_Read();
        
    }
        
    
   

}

void __interrupt() ISR(void)
{
  if(TMR1IF)
  {
    if(Check==0)
    {
      SevenSeg1 = 0x00;
      PORTCbits.RC0= 1;
      PORTCbits.RC1 =0;
      PORTCbits.RC2 =0;
      PORTCbits.RC3 =0;
      SevenSeg1 = Segments_Code[Digits[0]];
    }
    if(Check==1)
    {
      SevenSeg1 = 0x00;
      PORTCbits.RC0= 0;
      PORTCbits.RC1 =1;
      PORTCbits.RC2 =0;
      PORTCbits.RC3 =0;
      SevenSeg1 = Segments_Code[Digits[1]];
      PORTDbits.RD7 =1;
    }
    if(Check==2)
    {
      SevenSeg1 = 0x00;
      PORTCbits.RC0= 0;
      PORTCbits.RC1 =0;
      PORTCbits.RC2 =1;
      PORTCbits.RC3 =0;
      SevenSeg1 = Segments_Code[Digits[2]];
    }
    if(Check==3)
    {
      SevenSeg1 = 0x00;
      PORTCbits.RC0= 0;
      PORTCbits.RC1 =0;
      PORTCbits.RC2 =0;
      PORTCbits.RC3 =1;
      SevenSeg1 = Segments_Code[Digits[3]];
    }
    
  Check++;
    if(Check==4)
    Check=0;
    TMR1IF = 0;
    TMR1 = PreLoad; // Preloading Timer1
  }
}
void TMR1_Init()
{
 
  // Preload TMR1 Register Pair
  TMR1 = PreLoad;
  // Choose the local clock source (timer mode)
  TMR1CS = 0;
  // Choose the desired prescaler ratio (1:8)
  T1CKPS0 = 0;
  T1CKPS1 = 0;
  // Switch ON Timer1 Module!
  TMR1ON = 1;
  
  TMR1IE = 1;
  TMR1IF = 0;
  PEIE = 1;
  GIE = 1;
}
void SevenSeg1_Init()
{
  TMR1_Init();
  SevenSeg1_D = 0x00;
  SevenSeg1 = 0x00;
 
}
void SevenSeg1_Write(unsigned int num)
{
 
    Digits[0] = num / 1000;
    num%=1000;
    Digits[1] = num / 100;
    num%=100;
    Digits[2] = num / 10;
    num%=10;
    Digits[3]=num;
    
 }
void ADC_Init(void)
{
    
    ADCON0= 0b01000001;
    ADCON1 =0b11000000;
    
}
unsigned int ADC_read() 
{
   
   ADCON0 &= 0xC5; 
   ADCON0 |= 0 << 3; 
   __delay_ms(2);
   GO_nDONE = 1; 
   while (GO_nDONE);  
   return ((ADRESH << 8) + ADRESL); 
}
