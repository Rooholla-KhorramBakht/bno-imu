#ifndef __BNO_55__
#define __BNO_55__

#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <shm.hpp>


namespace bno_imu{

class BNO055{
    public:
        
        BNO055() : sensor_(55, 0x28, &Wire){};
        void init(){
            /* Initialise the sensor */
            if (!sensor_.begin())
            {
                /* There was a problem detecting the BNO055 ... check your connections */
                // Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
                while (1);
            }
        };

        void get_data(shm_data & packet){
            //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
            sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
            sensor_.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
            sensor_.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
            sensor_.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
            sensor_.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
            sensor_.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
            sensor_.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);


            // Serial.println("--");
            imu::Quaternion q = sensor_.getQuat();
            packet.data.q[0]=q.x();
            packet.data.q[1]=q.y();
            packet.data.q[2]=q.z();
            packet.data.q[3]=q.w();

            packet.data.accel[0]=linearAccelData.acceleration.x;
            packet.data.accel[1]=linearAccelData.acceleration.y;
            packet.data.accel[2]=linearAccelData.acceleration.z;

            packet.data.gyro[0]=angVelocityData.gyro.x;
            packet.data.gyro[1]=angVelocityData.gyro.y;
            packet.data.gyro[2]=angVelocityData.gyro.z;

            packet.data.mag[0]=magnetometerData.magnetic.x;
            packet.data.mag[1]=magnetometerData.magnetic.y;
            packet.data.mag[2]=magnetometerData.magnetic.z;

            packet.data.gravity[0]=gravityData.acceleration.x;
            packet.data.gravity[1]=gravityData.acceleration.y;
            packet.data.gravity[2]=gravityData.acceleration.z;
            
            delay(BNO055_SAMPLERATE_DELAY_MS);
        };


    private:
        /* Set the delay between fresh samples */
        const uint16_t BNO055_SAMPLERATE_DELAY_MS = 10;
        // Check I2C device address and correct line below (by default address is 0x29 or 0x28)
        //                                   id, address
        Adafruit_BNO055 sensor_; // = Adafruit_BNO055();

};// bno class
} // namespace



#endif