#include <Arduino.h>
#include <SPI.h>
#include "driver/gpio.h"
#include "driver/twai.h"

#define TX_GPIO_NUM   GPIO_NUM_10  // Connects to CTX
#define RX_GPIO_NUM   GPIO_NUM_9  // Connects to CRX
#define CAN_BASE_ADDRESS   0x121
#define CAN_QUAT_ADDRESS   (CAN_BASE_ADDRESS<<4)|0x0
#define CAN_STATUS_ADDRESS   (CAN_BASE_ADDRESS<<4)|0x1

void setup()
{
  Serial.begin(115200);
  //Initialize configuration structures using macro initializers
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_4, GPIO_NUM_5, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    //Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        printf("Driver installed\n");
    } else {
        printf("Failed to install driver\n");
        return;
    }

    //Start TWAI driver
    if (twai_start() == ESP_OK) {
        printf("Driver started\n");
    } else {
        printf("Failed to start driver\n");
        return;
    }
}


void transmitQuaternion(float qx, float qy, float qz, float qw, uint32_t address)
{
  union{
    struct{
      int16_t q[4];
    }data;
    uint8_t buffer[8];
  }quat_packet;
  quat_packet.data.q[0] = (int16_t)(qx*32768.);
  quat_packet.data.q[1] = (int16_t)(qy*32768.);
  quat_packet.data.q[2] = (int16_t)(qz*32768.);
  quat_packet.data.q[3] = (int16_t)(qy*32768.);
  twai_message_t message;
  message.identifier = address;
  message.extd = 1;
  message.data_length_code = 8;
  for (int i = 0; i < 8; i++) {
      message.data[i] = quat_packet.buffer[i];
  }
  //Queue message for transmission
  if (!twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) 
  {
    printf("Failed to queue quaternion message for transmission\n");
  }
}
void transmitStatus(float radError, uint8_t quality, uint32_t address)
{
  union{
    struct{
      float_t radError;
      uint8_t quality;
    }data;
    uint8_t buffer[5];
  }quat_packet;
  quat_packet.data.radError = radError;
  quat_packet.data.quality = quality;
  twai_message_t message;
  message.identifier = address;
  message.extd = 1;
  message.data_length_code = 5;
  for (int i = 0; i < 5; i++) {
      message.data[i] = quat_packet.buffer[i];
  }
  //Queue message for transmission
  if (!twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) 
  {
    printf("Failed to queue status message for transmission\n");
  }
}


auto old_stamp = micros();
void loop()
{
  float q[4]={.1,.2,.3,.4};
  transmitQuaternion(q[0], q[1], q[2], q[3], CAN_QUAT_ADDRESS);
  transmitStatus(0.1, 1, CAN_STATUS_ADDRESS);

  delay(5);
}
