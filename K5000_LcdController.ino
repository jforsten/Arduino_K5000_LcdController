//
// Kawai K5000 LCD background LED control with data encoder 
// (c) Juha Forsten 23/06/2017
//
// Encoder read adapted from: http://www.hobbytronics.co.uk/arduino-tutorial6-rotary-encoder
//
// Pro Micro 5V/16MHz

#include <EEPROM.h>

// Pin setup
const int ENC_PIN_A = 2;  
const int ENC_PIN_B = 3;  
const int LED_OUT = 10;  

const int WAIT_ON_START = 2000; // Active time (ms) on start
const int WAIT_ON_SAVE = 4000; // Wait until save

const int EEPROM_STORE = 0;

// Vars
byte brightness = 0;
byte fadeAmount = 5;

unsigned long current_time;
unsigned long loop_time;

byte enc_A;
byte enc_B;
byte enc_A_last_value=0;

unsigned long last_trig = 0;

void setup()  {
  pinMode(10, OUTPUT);
  pinMode(ENC_PIN_A, INPUT);
  pinMode(ENC_PIN_B, INPUT);
  
  current_time = millis();
  loop_time = current_time;
  
  brightness = EEPROM.read(EEPROM_STORE);
  analogWrite(10,brightness);
} 

void loop()  {
  
  current_time = millis();

  if (current_time > WAIT_ON_START && last_trig==0) return;
  
  if (current_time - last_trig > WAIT_ON_SAVE) {
    
        for(int i=0;i<4;i++) {
          analogWrite(LED_OUT, 0);
          delay(100);   
          analogWrite(LED_OUT,255);   
          delay(100);   
        }
        
        EEPROM.write(EEPROM_STORE, brightness);
        
        analogWrite(LED_OUT,brightness);  
        
        last_trig = 0;
        return;
  }
   
  if(current_time >= (loop_time+2)) {
    
    enc_A = digitalRead(ENC_PIN_A);
    enc_B = digitalRead(ENC_PIN_B);
       
    if((!enc_A) && (enc_A_last_value)) {
      if(enc_B) {
        if(brightness + fadeAmount <= 255) brightness += fadeAmount;               
      }   
      else {
        if(brightness - fadeAmount >= 0) brightness -= fadeAmount;               
      }   
      last_trig = millis();
    }   
    
    enc_A_last_value = enc_A;     
    loop_time = current_time;

    analogWrite(LED_OUT, brightness);   
  }                  
}

