
#include <FlexCAN_T4.h>

union shm{
    struct{
        float quaternion_hand[4];
        float quaternion_shoulder[4];
        float quaternion_base[4];
        float hand_quality;
        float shoulder_quality;
        float base_quality;
        uint8_t h1;
        uint8_t h2;
        uint8_t h3;
        uint8_t h4;
    }data_;
    uint8_t buffer[sizeof(data_)];
};
shm shm_t;
