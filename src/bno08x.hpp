#ifndef __BNO_08x__
#define __BNO_08x__

#include <Adafruit_BNO08x.h>
#include <Wire.h>
#include <shm.hpp>


namespace bno_imu{

class BNO08X{
    public:
        
        BNO08X() : sensor_(-1){};
        void init(){
            /* Initialise the sensor */
            // The default address is 0X4A but try 0X4b if the former fails
            if (!sensor_.begin_I2C(0x4B, &Wire, 55)) {
                // Serial.println("Failed to find BNO08x chip");
                while (1) { delay(10); }
            };
            // Serial.println("BNO08x Found!");

            if (!sensor_.enableReport(reportType, reportIntervalUs)) {
                // Serial.println("Could not enable stabilized remote vector");
                while (1) { delay(10);}
            }
        };

        void get_data(shm_data *packet){

            if(sensor_.getSensorEvent(&sensorValue_)){
                // Serial.println("--");
                packet->data.q[0]= sensorValue_.un.arvrStabilizedRV.i;
                packet->data.q[1]= sensorValue_.un.arvrStabilizedRV.j;
                packet->data.q[2]= sensorValue_.un.arvrStabilizedRV.k;
                packet->data.q[3]= sensorValue_.un.arvrStabilizedRV.real;

                // Serial.print(packet->data.q[0]); Serial.print(" "); Serial.print(packet->data.q[1]);  Serial.print(" ");
                // Serial.print(packet->data.q[2]); Serial.print(" "); Serial.print(packet->data.q[3]);  Serial.println(" ");

            
                packet->data.accel[0]= sensorValue_.un.linearAcceleration.x;
                packet->data.accel[1]= sensorValue_.un.linearAcceleration.y;
                packet->data.accel[2]= sensorValue_.un.linearAcceleration.z;

                packet->data.gyro[0]= sensorValue_.un.gyroscope.x;
                packet->data.gyro[1]= sensorValue_.un.gyroscope.y;
                packet->data.gyro[2]= sensorValue_.un.gyroscope.z;

                packet->data.mag[0]=sensorValue_.un.rawMagnetometer.x;
                packet->data.mag[1]=sensorValue_.un.rawMagnetometer.y;
                packet->data.mag[2]=sensorValue_.un.rawMagnetometer.z;

                packet->data.gravity[0]=sensorValue_.un.gravity.x;
                packet->data.gravity[1]=sensorValue_.un.gravity.y;
                packet->data.gravity[2]=sensorValue_.un.gravity.z;
            }
        };


    private:
        // Check I2C device address and correct line below (by default address is 0x29 or 0x28)
        //                                   id, address
        Adafruit_BNO08x sensor_;
        sh2_SensorValue_t sensorValue_;

        #ifdef FAST_MODE
            // Top frequency is reported to be 1000Hz (but freq is somewhat variable)
            sh2_SensorId_t reportType = SH2_GYRO_INTEGRATED_RV;
            long reportIntervalUs = 2000;
        #else
            // Top frequency is about 250Hz but this report is more accurate
            sh2_SensorId_t reportType = SH2_ARVR_STABILIZED_RV;
            long reportIntervalUs = 10000;
        #endif


};// bno class
} // namespace



#endif