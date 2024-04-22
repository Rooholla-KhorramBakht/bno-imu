#include <SPI.h>

#ifndef __SHM__
#define __SHM__

struct imu_data{
    float q[4];
    float accel[3];
    float gyro[3];
    float mag[3];
    float gravity[3];
    uint8_t quat_accuracy;
    uint8_t h1;
    uint8_t h2;
    uint8_t h3;
    uint8_t h4;
};

union shm_data {
    imu_data data;
    uint8_t buffer[4*16 + 1 + 4];
};

shm_data packet;

#endif