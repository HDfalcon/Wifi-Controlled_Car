#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include <string.h>

#define rx 16
#define tx 17

HardwareSerial SerialPort(2);

#define interrupt_1 25
#define interrupt_2 26
#define LED_1 14
#define LED_2 12

void ISR_1();
void ISR_2();

esp_now_peer_info_t peer_info = {
  .peer_addr = {0x3C, 0xE9, 0x0E, 0x86, 0x0C, 0x84},
  .lmk = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
  .channel = 0,
  .ifidx = WIFI_IF_STA,
  .encrypt = 0,
  .priv = NULL
};

typedef struct message_uart{
  int16_t speed;
  int16_t dir;
}message_uart;
message_uart uart;

typedef struct message{
  int16_t speed;
  int8_t dir;
}message;
message outgoing_message;

void setup() {
  SerialPort.begin (115200, SERIAL_8N1, rx, tx);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  pinMode(interrupt_1, INPUT_PULLUP);
  pinMode(interrupt_2, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  WiFi.mode(WIFI_STA);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  if(esp_now_init() != ESP_OK){
    return;
  }

  esp_now_add_peer(&peer_info);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println("asdassadsa");
  attachInterrupt(digitalPinToInterrupt(interrupt_1), ISR_1, FALLING );
  attachInterrupt(digitalPinToInterrupt(interrupt_2), ISR_2, FALLING ); 
}

char c; 
uint8_t idx;
char buf[15];

void loop() {
  if(SerialPort.available() > 0){
    c = SerialPort.read();
    if(c == '\n'){
      memcpy(&uart, buf, sizeof(uart));
      outgoing_message.speed = map(uart.dir, 0, 1023, -1000, 1000);
      outgoing_message.dir = map(uart.speed, 0, 1023, -35, 35);
      esp_now_send(peer_info.peer_addr, (uint8_t*)&outgoing_message, sizeof(message));
      memset(buf, 0, sizeof(buf));
      idx = 0;
    }
    else{
      if(idx < 15){
      buf[idx] = c;
      idx ++;
      }
    }
  }
}

/*there was no need for interrupt, so I conserved them for further developments*/
void ISR_1(){
  digitalWrite(LED_2, HIGH);
}

void ISR_2(){
  digitalWrite(LED_2, LOW);
}