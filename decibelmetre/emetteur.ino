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
    //Serial.print(hour());
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
    // flash a light to show message is sent
    digitalWrite(13, true);
      
    itoa(db,nombre,10); // 10 car décimal
    strcpy (message, msg);
    strcat(message,nombre);
    vw_send((uint8_t *)message, strlen(message));
    Serial.println(message);
    vw_wait_tx();
  
    digitalWrite(13, false);
  }

 delay(100);


}
