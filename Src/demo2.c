

 /*Developed by Nam Phan*/
 

#include <xc.h>
#include"config.h"
#include<stdint.h>
#include<stdio.h>

#define _XTAL_FREQ 20000000
#define SevenSeg1 PORTD
#define SevenSeg1_D TRISD
#define SBIT_ADFM 7
#define SBIT_ADON 0
#define SBIT_CHS0 2



int8_t Segments_Code[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int16_t Digits[4];
uint16_t ADC_Value=0;
float Vin;
int Vin_convert;
int8_t check =0;

void SevenSeg1_Write( void);
void display_data(void);
void ADC_Init(void);
int ADC_Read(int ADC_channel);


void main(void) {
    
    TMR0 =0;// timer0 reset bit
    OPTION_REG = 0x83; // select prescalar value 1:16 for timer0
    INTCON=0xA0;//turn on global interrupt and timer0 overflow interrupt
    
    SevenSeg1_D = 0x00;
    SevenSeg1 = 0x00;
  
    
    
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 =1;
    PORTCbits.RC2 =1;
    PORTCbits.RC3 =1;
    ADC_Init();
    
    while(1)
    {
        
        
         ADC_Value =ADC_Read(0);
       
         Vin=(25*(float)ADC_Value)/1024;
         if(Vin>=10.0)
         {
             check =0;
             Vin_convert = Vin*100;
         }
         else
         {
             check=1;
             Vin_convert =Vin*1000;
         }
         
    
           __delay_ms(100);
    }
        
      
    
    return;
}

void SevenSeg1_Write(void)
{
	
    Digits[0] =(Vin_convert/1000) ;
  
    Digits[1] =(Vin_convert/100)%10 ;
    
    Digits[2] =(Vin_convert/10)%10; ;
    
    Digits[3]=(Vin_convert%10);
	
	
  
 }
void __interrupt() ISR(void)
{
   SevenSeg1_Write(); 
   display_data();
   INTCONbits.T0IF = 0;
}
void display_data(void)
{
	
		
    SevenSeg1 = Segments_Code[Digits[0]];
    PORTCbits.RC0 = 0;//on 
    __delay_ms(3);
    PORTCbits.RC0 = 1;//off
    
    SevenSeg1 = Segments_Code[Digits[1]];
    PORTCbits.RC1 = 0;//on 
    PORTDbits.RD7 = 1;// on dot at digit kth 2
    __delay_ms(3);
    PORTCbits.RC1 = 1;//off
    
    SevenSeg1 = Segments_Code[Digits[2]];
    PORTCbits.RC2 = 0;//on 
    __delay_ms(3);
    PORTCbits.RC2 = 1;//off
    
    SevenSeg1 = Segments_Code[Digits[3]];
    PORTCbits.RC3 = 0;//on 
    __delay_ms(3);
    PORTCbits.RC3 = 1;//off
   
        
	
	
    
}
void ADC_Init(void)
{
    ADCON0=0x00;// freq = osc_freq/2,ADC off 
    ADCON1= (1<<SBIT_ADFM);// All pins are configured as Analog pins and ADC result is right justified  
    
}
int ADC_Read(int ADC_channel)
{
    ADCON0=(1<<SBIT_ADON) | (ADC_channel<<SBIT_CHS0);//select required channel and turn ON adc
    __delay_ms(100);                   //Acquisition Time(Wait for Charge Hold Capacitor to get charged )
   
    GO=1;                           // Start ADC conversion
    while(GO_DONE==1);              // Wait for the conversion to complete
                                    // GO_DONE bit will be cleared once conversion is complete

    return((ADRESH<<8) + ADRESL);   // return right justified 10-bit result
    
    
}
