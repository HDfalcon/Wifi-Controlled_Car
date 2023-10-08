#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Deneyap_Servo.h>

#define r_en 25
#define l_en 17
#define r_pwm 2
#define l_pwm 4
#define servo 13

#define LEDC_CHANNEL_2 2
#define LEDC_CHANNEL_3 3
#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 5000

void DataReceiverISR(const uint8_t * mac, const uint8_t *incomingMessage, int len);
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax);

esp_now_peer_info_t peer_info = {
  .peer_addr = { 0x3C, 0xE3, 0x0E, 0x0B, 0xB3, 0x4C },
  .lmk = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
  .channel = 0,
  .ifidx = WIFI_IF_STA,
  .encrypt = 0,
  .priv = NULL
};

typedef struct message{
  volatile int16_t speed;
  volatile int8_t dir; //0: forward /// [0, 35]: right /// [-35, 0]: left
}message;

Servo dir_servo;
message incoming_message;

void setup() {  
  pinMode(l_en, OUTPUT);
  pinMode(r_en, OUTPUT);
  pinMode(l_pwm, OUTPUT);
  pinMode(r_pwm, OUTPUT);
  pinMode(servo, OUTPUT);
 
  WiFi.mode(WIFI_STA);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  if (esp_now_init() != ESP_OK) {
    return;
  }

  esp_now_add_peer(&peer_info);
  esp_now_register_recv_cb(DataReceiverISR);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
    
  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_3, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(r_pwm, LEDC_CHANNEL_2);
  ledcAttachPin(l_pwm, LEDC_CHANNEL_3);
  
  digitalWrite(l_en, 1);
  digitalWrite(r_en, 1);

  dir_servo.attach(servo, 13);
}

void loop() {
  if(incoming_message.speed >= 0){
  ledcAnalogWrite(LEDC_CHANNEL_2, incoming_message.speed, 1000);
  }
  else{
  ledcAnalogWrite(LEDC_CHANNEL_3, abs(incoming_message.speed), 1000);
  }
  dir_servo.write(100 + incoming_message.dir); // dir must be between 70 - 130
}

void DataReceiverISR(const uint8_t * mac, const uint8_t *incomingMessage, int len){
  memcpy(&incoming_message, incomingMessage, sizeof(message));
}

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax){
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}