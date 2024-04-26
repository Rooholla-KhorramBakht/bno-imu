#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "bno_manager.hpp";


FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

BnoManager imu_base(IMU_BASE_ADDRESS);
BnoManager imu_hand(IMU_HAND_ADDRESS);
BnoManager imu_shoulder(IMU_SHOULDER_ADDRESS);

void canSniff(const CAN_message_t &msg) {

  imu_base.process_response(msg);
  imu_shoulder.process_response(msg);
  imu_hand.process_response(msg);


  Serial.println("IMU_BASE");
  Serial.print(imu_base.quat[0], 2);
  Serial.print(F(","));
  Serial.print(imu_base.quat[1], 2);
  Serial.print(F(","));
  Serial.print(imu_base.quat[2], 2);
  Serial.print(F(","));
  Serial.print(imu_base.quat[3], 2);
  Serial.print(F(","));
  float norm = 0;
  for(unsigned i =0; i < 4; ++i){
    norm +=  imu_base.quat[i]*imu_base.quat[i];
  };
  Serial.print(sqrt(norm));
  Serial.print(F(","));
  Serial.print(imu_base.quality);
  Serial.print(F(","));
  Serial.println();

  Serial.println("IMU_SHOULDER");
  Serial.print(imu_shoulder.quat[0], 2);
  Serial.print(F(","));
  Serial.print(imu_shoulder.quat[1], 2);
  Serial.print(F(","));
  Serial.print(imu_shoulder.quat[2], 2);
  Serial.print(F(","));
  Serial.print(imu_shoulder.quat[3], 2);
  Serial.print(F(","));
  norm = 0;
  for(unsigned i =0; i < 4; ++i){
    norm += imu_shoulder.quat[i]*imu_shoulder.quat[i];
  };
  Serial.print(sqrt(norm));
  Serial.print(F(","));
  Serial.print(imu_shoulder.quality);
  Serial.print(F(","));
  Serial.println();

  Serial.println("IMU_HAND");
  Serial.print(imu_hand.quat[0], 2);
  Serial.print(F(","));
  Serial.print(imu_hand.quat[1], 2);
  Serial.print(F(","));
  Serial.print(imu_hand.quat[2], 2);
  Serial.print(F(","));
  Serial.print(imu_hand.quat[3], 2);
  Serial.print(F(","));
  norm = 0;
  for(unsigned i =0; i < 4; ++i){
    norm +=  imu_hand.quat[i]*imu_hand.quat[i];
  };
  Serial.print(sqrt(norm));
  Serial.print(F(","));
  Serial.print(imu_hand.quality);
  Serial.print(F(","));
  Serial.println();
}


void setup(void) {
  Serial.begin(115200); delay(400);
  
  Can0.begin();
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();
}


void loop() {
  Can0.events();
  // delay(1);
}