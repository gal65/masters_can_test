// CANtest for Teensy 4.0
#include <FlexCAN_T4.h>

#define debug(msg) Serial.print("["); Serial.print(__FILE__); Serial.print("::"); Serial.print(__LINE__);  Serial.print("::"); Serial.print(msg); Serial.println("]");
void debug_pause(void)
{
  Serial.print("Paused...");
  while (!Serial.available());
  while (Serial.available()) Serial.read();
  Serial.println("Restarted.");
}

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

static CAN_message_t msg;
static CAN_message_t inMsg;

// -------------------------------------------------------------
void setup(void)
{
  Serial.begin(115200);
  int iSerialTimeout = 1000000;
  delay(100);
  while (!Serial && (iSerialTimeout-- != 0));  
  debug (F("start setup"));

  Can1.begin();  
  Can1.setBaudRate(250000);

  pinMode(13, OUTPUT);

  // t4 missing msg.ext = 0;
  msg.id = 0x000;
  msg.len = 8;
  msg.flags.extended = 0;
  msg.flags.remote   = 0;
  msg.flags.overrun  = 0;
  msg.flags.reserved = 0;
  msg.buf[0] = 10;
  msg.buf[1] = 20;
  msg.buf[2] = 0;
  msg.buf[3] = 100;
  msg.buf[4] = 128;
  msg.buf[5] = 64;
  msg.buf[6] = 32;
  msg.buf[7] = 16;
  debug (F("setup complete"));
  //debug_pause();

  Can1.write(msg);
}


// -------------------------------------------------------------
void loop(void)
{
  if (Can1.read(inMsg)) 
  {
    digitalWrite(13, HIGH);
    Serial.print("CAN1 "); 
    Serial.print("MB: "); Serial.print(inMsg.mb);
    Serial.print("  ID: 0x"); Serial.print(inMsg.id, HEX );
    Serial.print("  EXT: "); Serial.print(inMsg.flags.extended );
    Serial.print("  LEN: "); Serial.print(inMsg.len);
    Serial.print(" DATA: ");
    for ( uint8_t i = 0; i < 8; i++ ) {
      Serial.print(inMsg.buf[i]); Serial.print(" ");
    }
    Serial.print("  TS: "); Serial.println(inMsg.timestamp);
    digitalWrite(13, LOW);
  }
  else
  {
    Serial.println("Sending data...");
    digitalWrite(13, HIGH);
    msg.buf[0]--;
    Can1.write(msg);
    msg.buf[0]--;
    Can1.write(msg);
    msg.buf[0]--;
    Can1.write(msg);
    msg.buf[0]--;
    Can1.write(msg);
    msg.buf[0]--;
    Can1.write(msg);  
    delay(20);
    digitalWrite(13, LOW);
    Serial.println("Sent data!");
  }
}
