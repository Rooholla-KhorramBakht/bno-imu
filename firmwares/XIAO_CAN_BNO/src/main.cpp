
#include <Wire.h>
#include "driver/gpio.h"
#include "driver/twai.h"
#include <SPI.h>
#include "SparkFun_BNO080_Arduino_Library.h"
#include "params.hpp"

BNO080 myIMU;
// pin used for interrupts
byte imuINTPin = D5;
int imuInterruptFlag = 0;
// Initial Quaternion value
float qx = 0.0;
float qy = 0.0;
float qz = 0.0;
float qw = 1.0;
float radAccuracy = 0.0;
byte reliablility = 0;

// This function is called whenever an interrupt is detected by the arduino
void interrupt_handler()
{
  imuInterruptFlag = 1;
}

void transmitQuaternion(const float qx, const float qy, const float qz, const float qw, uint32_t address)
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
  quat_packet.data.q[3] = (int16_t)(qw*32768.);
  twai_message_t message;
  message.identifier = address;
  message.extd = 1;
  message.data_length_code = 8;
  for (int i = 0; i < 8; i++) {
      message.data[i] = quat_packet.buffer[i];
  }
  //Queue message for transmission
  if (!twai_transmit(&message, pdMS_TO_TICKS(CAN_TRANSMIT_TIMEOUT)) == ESP_OK) 
  {
    printf("Failed to queue quaternion message for transmission\n");
  }
}

void transmitStatus(float radError, uint8_t quality, uint32_t address)
{
  union{
    struct{
      // float_t radError;
      uint8_t quality;
    }data;
    uint8_t buffer[1];
  }quat_packet;
  // quat_packet.data.radError = radError;
  quat_packet.data.quality = quality;
  twai_message_t message;
  message.identifier = address;
  message.extd = 1;
  message.data_length_code = 1;
  for (int i = 0; i < 1; i++) {
      message.data[i] = quat_packet.buffer[i];
  }
  //Queue message for transmission
  if (!twai_transmit(&message, pdMS_TO_TICKS(CAN_TRANSMIT_TIMEOUT)) == ESP_OK) 
  {
    printf("Failed to queue status message for transmission\n");
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO080 Read example with Interrupt handler, getReading and multiple access");

  Wire.begin();

  if (myIMU.begin(BNO080_DEFAULT_ADDRESS, Wire, D1) == false)
  {
    Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1);
  }

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  // prepare interrupt on falling edge (= signal of new data available)
  attachInterrupt(digitalPinToInterrupt(imuINTPin), interrupt_handler, FALLING);
  interrupts();
  // myIMU.enableRotationVector(5); //Send data update every 50ms
  myIMU.enableARVRStabilizedRotationVector(5); //Send data update every 100 ms

  delay(10);

  //Initialize configuration structures using macro initializers
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_4, GPIO_NUM_5, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  //Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
      Serial.println("Driver installed\n");
  } else {
      Serial.println("Failed to install driver\n");
      return;
  }

  //Start TWAI driver
  if (twai_start() == ESP_OK) {
     Serial.println("Driver started\n");
  } else {
      Serial.println("Failed to start driver\n");
      return;
  }
}

int can_transmit=0;
void loop()
{
  if(imuInterruptFlag)
  {
   switch (myIMU.getReadings())
   {
      case SENSOR_REPORTID_LINEAR_ACCELERATION: {
        //Do stuff needed when getting this report
      }
      break;

      case SENSOR_REPORTID_ROTATION_VECTOR:
      case SENSOR_REPORTID_AR_VR_STABILIZED_ROTATION_VECTOR: {
        myIMU.getQuat(qx, qy, qz, qw, radAccuracy, reliablility);
        can_transmit=1;
        Serial.print(qx, 2);
        Serial.print(F(","));
        Serial.print(qy, 2);
        Serial.print(F(","));
        Serial.print(qz, 2);
        Serial.print(F(","));
        Serial.print(qw, 2);
        Serial.print(F(","));
        Serial.print(reliablility);
        Serial.print(F(","));
        float norm = qx*qx + qy*qy + qz*qz + qw*qw;
        Serial.print(sqrt(norm));
        Serial.print(F(","));
        Serial.println();
        transmitStatus(radAccuracy, reliablility, CAN_STATUS_ADDRESS);
        delayMicroseconds(100);
        transmitQuaternion(qx, qy, qz, qw, CAN_QUAT_ADDRESS);


      }
      break;
      default:
         // Unhandled Input Report
         break;
   }
  }

  if(can_transmit)
  {
    can_transmit=0;

  }

}