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

void setup()
{
  UCSR0B |= _BV(TXEN0);    // I used arduino as a master in this project, so I only enabled TX.
  UCSR0C |= (~(1 << UMSEL01)) && (~(1 << UMSEL00)) && (~(1 << UPM01)) && (~(1 << UPM00)) && (~(1 << USBS0));    //  Asynchronous mode
  UCSR0B |= (~(1 << UCSZ02));   // adjusting message size 8 bit
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); 
  UCSR0A |= (1 << U2X0);    //  fast mode
  UBRR0 = 16;       //  adjusting baudrate to 115200

  DDRC = _BV(DDC0) && _BV(DDC1); // ADC pins
}

void loop()
{
  outgoing_message.dir = PORTC(PORTC0);
  outgoing_message.speed = PORTC(PORTC1);
  send(&outgoing_message);
  Serial.println();
  delay(25);
}

void send(message *table)
{
  Serial.write((char *)table, sizeof(message));
}
