#include <VirtualWire.h>

void setup()
{
  Serial.begin(9600);  // Debugging only
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);  // Bits per sec

  vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    int i;
    digitalWrite(13, true); // Flash a light to show received good message
    
    for (i = 0; i < buflen; i++)
    {
      Serial.write(buf[i]);
      Serial.write("\n");
    }
    digitalWrite(13, false);
  }
}
