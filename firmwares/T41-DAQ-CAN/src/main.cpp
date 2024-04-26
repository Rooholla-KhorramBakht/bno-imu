#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "bno_manager.hpp";

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

BnoManager imu_base(IMU_BASE_ADDRESS);
BnoManager imu_hand(IMU_HAND_ADDRESS);
BnoManager imu_shoulder(IMU_SHOULDER_ADDRESS);

void canSniff(const CAN_message_t &msg) {

  imu_base.process_response(msg);
  imu_base.update_shm(shm_t.data_.quaternion_base, shm_t.data_.base_quality);
  imu_shoulder.process_response(msg);
  imu_shoulder.update_shm(shm_t.data_.quaternion_shoulder, shm_t.data_.shoulder_quality);
  imu_hand.process_response(msg);
  imu_hand.update_shm(shm_t.data_.quaternion_hand, shm_t.data_.hand_quality);

  // Serial.println("IMU_BASE");
  // Serial.print(shm_t.data_.quaternion_base[0], 2);
  // Serial.print(F(","));
  // Serial.print(shm_t.data_.quaternion_base[1], 2);
  // Serial.print(F(","));
  // Serial.print(shm_t.data_.quaternion_base[2], 2);
  // Serial.print(F(","));
  // Serial.print(shm_t.data_.quaternion_base[3], 2);
  // Serial.print(F(","));
  // float norm = 0;
  // for(unsigned i =0; i < 4; ++i){
  //   norm +=  imu_base.quat[i]*imu_base.quat[i];
  // };
  // Serial.print(sqrt(norm));
  // Serial.print(F(","));
  // Serial.print(shm_t.data_.base_quality);
  // Serial.print(F(","));
  // Serial.println();

  // Serial.println("IMU_SHOULDER");
  // Serial.print(imu_shoulder.quat[0], 2);
  // Serial.print(F(","));
  // Serial.print(imu_shoulder.quat[1], 2);
  // Serial.print(F(","));
  // Serial.print(imu_shoulder.quat[2], 2);
  // Serial.print(F(","));
  // Serial.print(imu_shoulder.quat[3], 2);
  // Serial.print(F(","));
  // norm = 0;
  // for(unsigned i =0; i < 4; ++i){
  //   norm += imu_shoulder.quat[i]*imu_shoulder.quat[i];
  // };
  // Serial.print(sqrt(norm));
  // Serial.print(F(","));
  // Serial.print(shm_t.data_.shoulder_quality);
  // Serial.print(F(","));
  // Serial.println();

  // Serial.println("IMU_HAND");
  // Serial.print(imu_hand.quat[0], 2);
  // Serial.print(F(","));
  // Serial.print(imu_hand.quat[1], 2);
  // Serial.print(F(","));
  // Serial.print(imu_hand.quat[2], 2);
  // Serial.print(F(","));
  // Serial.print(imu_hand.quat[3], 2);
  // Serial.print(F(","));
  // norm = 0;
  // for(unsigned i =0; i < 4; ++i){
  //   norm +=  imu_hand.quat[i]*imu_hand.quat[i];
  // };
  // Serial.print(sqrt(norm));
  // Serial.print(F(","));
  // Serial.print(shm_t.data_.hand_quality);
  // Serial.print(F(","));
  // Serial.println();
}


void setup(void) {
  Serial.begin(115200); delay(400);
  shm_t.data_.h1 = 'a';
  shm_t.data_.h2 = 'b';
  shm_t.data_.h3 = 'c';
  shm_t.data_.h4 = '\n';


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
  Serial.write(shm_t.buffer, sizeof(shm_t.buffer));
  delayMicroseconds(100);
  // delay(1);
}