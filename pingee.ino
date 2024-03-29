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
  msg.id = 0x001;
  msg.len = 8;
  msg.flags.extended = 0;
  msg.flags.remote   = 0;
  msg.flags.overrun  = 0;
  msg.flags.reserved = 0;
  msg.buf[0] = 69;
  msg.buf[1] = 69;
  msg.buf[2] = 69;
  msg.buf[3] = 69;
  msg.buf[4] = 69;
  msg.buf[5] = 69;
  msg.buf[6] = 69;
  msg.buf[7] = 69;
  debug (F("setup complete"));
  //debug_pause();
}


// -------------------------------------------------------------
void loop(void)
{
  digitalWrite(13, HIGH);
  if (Can1.read(inMsg)) 
  {
    if (inMsg.buf[0] == 1) {
      Can1.write(msg); 
    }
  }
  digitalWrite(13, LOW);
}
