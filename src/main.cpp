#include <SPI.h>
#include <bno55.hpp>
#include <bno08x.hpp>
#include <shm.hpp>

#include <utility/imumaths.h>


#ifdef USE_BNO055
  bno_imu::BNO055 imu_sensor = bno_imu::BNO055();
#else
  bno_imu::BNO08X imu_sensor = bno_imu::BNO08X();
#endif

void setup(void)
{
  packet.data.h1 = 'a';
  packet.data.h2 = 'b';
  packet.data.h3 = 'c';
  packet.data.h4 = '\n';
  Serial.begin(115200);

  while (!Serial) delay(10);  // wait for serial port to open!
  imu_sensor.init();

  delay(1000);
}

void loop(void)
{
    imu_sensor.get_data(&packet);
    Serial.write(packet.buffer, 4*16 + 4);

    // Serial.print(packet.data.q[0]); Serial.print(" "); Serial.print(packet.data.q[1]);  Serial.print(" ");
    // Serial.print(packet.data.q[2]); Serial.print(" "); Serial.print(packet.data.q[3]);  Serial.println(" ");

    delay(10);
}

// void printEvent(sensors_event_t* event) {
//   double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
//   if (event->type == SENSOR_TYPE_ACCELEROMETER) {
//     Serial.print("Accl:");
//     x = event->acceleration.x;
//     y = event->acceleration.y;
//     z = event->acceleration.z;
//   }
//   else if (event->type == SENSOR_TYPE_ORIENTATION) {
//     Serial.print("Orient:");
//     x = event->orientation.x;
//     y = event->orientation.y;
//     z = event->orientation.z;
//   }
//   else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
//     Serial.print("Mag:");
//     x = event->magnetic.x;
//     y = event->magnetic.y;
//     z = event->magnetic.z;
//   }
//   else if (event->type == SENSOR_TYPE_GYROSCOPE) {
//     Serial.print("Gyro:");
//     x = event->gyro.x;
//     y = event->gyro.y;
//     z = event->gyro.z;
//   }
//   else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
//     Serial.print("Rot:");
//     x = event->gyro.x;
//     y = event->gyro.y;
//     z = event->gyro.z;
//   }
//   else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
//     Serial.print("Linear:");
//     x = event->acceleration.x;
//     y = event->acceleration.y;
//     z = event->acceleration.z;
//   }
//   else if (event->type == SENSOR_TYPE_GRAVITY) {
//     Serial.print("Gravity:");
//     x = event->acceleration.x;
//     y = event->acceleration.y;
//     z = event->acceleration.z;
//   }
//   else {
//     Serial.print("Unk:");
//   }

//   Serial.print("\tx= ");
//   Serial.print(x);
//   Serial.print(" |\ty= ");
//   Serial.print(y);
//   Serial.print(" |\tz= ");
//   Serial.println(z);
// }

