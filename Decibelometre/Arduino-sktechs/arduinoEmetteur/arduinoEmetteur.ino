#include <VirtualWire.h>

const int sampleWindow = 50;
unsigned int sample;
const char *msg = "";
char nombre[VW_MAX_MESSAGE_LEN];
char message[VW_MAX_MESSAGE_LEN];


void setup()   {

  Serial.begin(9600);
  vw_set_ptt_inverted(true);
  vw_setup(2000);

}

void loop()      {

  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  /* Calcul des décibels */
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);

    if (sample < 1024)
    {
      if (sample > signalMax)
      {
        signalMax = sample;
      }
      else if (sample < signalMin)
      {
        signalMin = sample;
      }
    }
  }
  peakToPeak = signalMax - signalMin;
  double db =  20.0  * log10 (peakToPeak  + 1);

  /* Envoi au recepteur si décibel supérieur a 8 */
  if (db > 8) {
    itoa(db, nombre, 10);
    strcpy (message, msg);
    strcat(message, nombre);
    vw_send((uint8_t *)message, strlen(message));
    Serial.println(db);
    vw_wait_tx();
  }

  delay(500);


}
