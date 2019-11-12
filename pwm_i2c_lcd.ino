/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +3.3V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe
  modified 28 Feb 2017 for use with sduino
  by Michael Mayer

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/
#include <I2C.h>
#include <LiquidCrystal_I2C.h>
#define LCD_ADDR 0x27
LiquidCrystal_I2C(lcd, 0x27, 16, 2);

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 13; // Analog output pin that the LED is attached to
//variaveis de uso global
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int line1_duty = 0;
int line2_adc = 0;

//função de media
int media_adc(int contador,int analog){
  int armazenamento[128]; //array para buffer dos dados
  int long calculado = 0; //variavel  para salvar soma 
  int i=0;   
  for (i = 0; i<contador; i++)   //loop de leitura
    {      
    armazenamento[i] = analogRead(analog); //salva no array
    calculado += armazenamento[i]; //soma o array ao valor total
    delay(2);
    }
  calculado = calculado/contador; //calcula a media
  return (calculado); //retorna o valor medio
}
//função de limpeza do LCD
void update_lcd(int x,int y)
{
  lcd_setCursor(x, y);
  lcd_print_s(" ");  
}
//SETUP
void setup() {  
  // initialize the LCD
  lcd_begin();
  // Turn on the blacklight and print a message.
  lcd_backlight();
  lcd_clear();  
  lcd_setCursor(4, 1);
  lcd_print_s("% Duty+");
  lcd_setCursor(4, 0); 
  lcd_print_s(" Media ADC");
}
//LOOP
void loop() {    
  int duty_cycle = 0;
  // read the analog in value:
  sensorValue = media_adc(128,analogInPin); //lê adc e retorna sua media
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255); //transformar range 1023 para 255
  // change the analog out value:
  if(sensorValue <8) //Proteção no ciclo menor que 1%
  {
    outputValue = 1;  
  }
  if(sensorValue>1020) //Proteção no ciclo maior que 99%
  {
    outputValue = 254;
  }  
  analogWrite(analogOutPin, outputValue); //gerador PWM  
  duty_cycle = (outputValue*100)/255; //exibe porcentagem duty+  
  if (duty_cycle<10)  //limpador de lcd do DUTY
    { 
    update_lcd(1,1);
    }    
  lcd_setCursor(0, 1);
  lcd_print_u(duty_cycle); //escreve valor DUTY    
  if (sensorValue<1000)  //limpador de lcd do ADC
    { 
      update_lcd(3,0);
      if (sensorValue<100)
        {
          update_lcd(2,0); 
          if (sensorValue<10)
            {
             update_lcd(1,0); 
            } 
        }      
    }    
  lcd_setCursor(0, 0);
  lcd_print_u(sensorValue); //escreve valor ADC  
  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:  
  delay(10);      //opcional
}
