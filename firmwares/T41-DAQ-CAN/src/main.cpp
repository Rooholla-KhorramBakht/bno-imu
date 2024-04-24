#include <Arduino.h>
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;
void canSniff(const CAN_message_t &msg);
void setup(void) {
  Serial.begin(115200); delay(400);
  pinMode(6, OUTPUT); digitalWrite(6, LOW); /* optional tranceiver enable pin */
  Can0.begin();
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();
}

union{
  struct{
    int16_t q[4];
  }data;
  uint8_t buffer[8];
}quat_packets[3];
  
union{
  struct{
    float_t radError;
    uint8_t quality;
  }data;
  uint8_t buffer[5];
}status_packets[3];

void decodeMessage(const CAN_message_t &msg)
{
  
}

void canSniff(const CAN_message_t &msg) {
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}

void loop() {
  Can0.events();
}