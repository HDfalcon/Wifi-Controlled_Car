#include <Arduino.h>

#define pot A0
#define joystick A1
#define PORTC _SFR_IO8(0x08);

typedef struct message
{
  int16_t speed;
  int16_t dir;
} message;

message outgoing_message;

void send(message *table);
void adcPotRead();
void adcStickRead();

void setup()
{
  UCSR0B |= _BV(TXEN0);    // I used arduino as a master in this project, so I only enabled TX.
  UCSR0C |= (~(1 << UMSEL01)) && (~(1 << UMSEL00)) && (~(1 << UPM01)) && (~(1 << UPM00)) && (~(1 << USBS0));    //  Asynchronous mode
  UCSR0B |= (~(1 << UCSZ02));   // adjusting message size 8 bit
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); 
  UCSR0A |= (1 << U2X0);    //  fast mode
  UBRR0 = 16;       //  adjusting baudrate to 115200
}

void loop()
{
  adcPotRead();
  adcStickRead();
  send(&outgoing_message);
  Serial.println();
  delay(25);
}

void send(message *table)
{
  Serial.write((char *)table, sizeof(message));
}

void adcPotRead(){
  ADMUX |= 0b00000001;  // adc1
  ADCSRA |= 0b11000000;
  while(bit_is_set(ADCSRA, ADSC));
  outgoing_message.speed = ADCL | (ADCH << 8);
}

void adcStickRead(){
  ADMUX |= 0b00000000;  // adc0
  ADCSRA |= 0b11000000;
  while(bit_is_set(ADCSRA, ADSC));
  outgoing_message.dir = ADCL | (ADCH << 8);
}
