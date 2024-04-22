#include <Arduino.h>
#include <Wire.h>
#include <shm.hpp>
#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080


BNO080 myIMU;

// indicators of new data availability
volatile byte newQuat = 0;
volatile byte newLinAcc = 0;
// internal copies of the IMU data
float ax, ay, az, qx, qy, qz, qw; // (qx, qy, qz, qw = i,j,k, real)
byte linAccuracy = 0;
float quatRadianAccuracy = 0;
byte quatAccuracy = 0;
// pin used for interrupts
byte imuINTPin = 8;

//Given an accuracy number, print what it means
void printAccuracyLevel(byte accuracyNumber)
{
  if (accuracyNumber == 0) Serial.println(F("Unreliable"));
  else if (accuracyNumber == 1) Serial.println(F("Low"));
  else if (accuracyNumber == 2) Serial.println(F("Medium"));
  else if (accuracyNumber == 3) Serial.println(F("High"));
}
// This function is called whenever an interrupt is detected by the arduino
void interrupt_handler()
{
  // code snippet from ya-mouse
   switch (myIMU.getReadings())
   {
      case SENSOR_REPORTID_LINEAR_ACCELERATION: {
        newLinAcc = 1;
      }
      break;

      case SENSOR_REPORTID_AR_VR_STABILIZED_ROTATION_VECTOR:{
        newQuat = 1;
      }
      break;
      default:
         // Unhandled Input Report
         break;
   }
}

void setup()
{

  packet.data.h1 = 'a';
  packet.data.h2 = 'b';
  packet.data.h3 = 'c';
  packet.data.h4 = '\n';

  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO080 Read example with Interrupt handler, getReading and multiple access");

  Wire.begin();

  //myIMU.enableDebugging(); // Uncomment this line to enable debug messages on Serial

  // imuINTPin is used as an active-low interrupt. .begin configures the pinMode as INPUT_PULLUP
  if (myIMU.begin(BNO080_DEFAULT_ADDRESS, Wire, imuINTPin) == false)
  {
    Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1);
  }

  Wire.setClock(400000); //Increase I2C data rate to 400kHz

  // prepare interrupt on falling edge (= signal of new data available)
  attachInterrupt(digitalPinToInterrupt(imuINTPin), interrupt_handler, FALLING);
  // enable interrupts right away to not miss first reports
  interrupts();

  // myIMU.enableLinearAccelerometer(50);  // m/s^2 no gravity, data update every 50 ms
  myIMU.enableARVRStabilizedRotationVector(5); //Send data update every 100 ms
}


int flag = 0;

void loop()
{
  // Serial.println("Doing other things");
  delay(10); //You can do many other things. Interrupt handler will retrieve latest data

  // If new data on Rotation Vector sensor, retrieve it from the library memory
  if(newQuat) {
    myIMU.getQuat(qx, qy, qz, qw, quatRadianAccuracy, quatAccuracy);
    packet.data.q[0] = qx;
    packet.data.q[1] = qy;
    packet.data.q[2] = qz;
    packet.data.q[3] = qw;
    packet.data.quat_accuracy = quatAccuracy;

    newQuat = 0; // mark data as read
    // Serial.print(F("quat:"));
    // Serial.print(packet.data.q[0], 2);
    // Serial.print(F(","));
    // Serial.print(packet.data.q[1], 2);
    // Serial.print(F(","));
    // Serial.print(packet.data.q[2], 2);
    // Serial.print(F(","));
    // Serial.print(packet.data.q[3], 2);
    // Serial.print(F(","));
    // printAccuracyLevel(quatAccuracy);
    // if(quatAccuracy == 3 && !flag){
    //   myIMU.saveCalibration();
    //   flag = 1;
    //   Serial.print("finished calibration");
    // };

    Serial.write(packet.buffer, 4*16 + 1 + 4);

  }
}

