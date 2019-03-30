/*
 * Project SIT210-WebHook
 * Author: Brett T Best
 * Date: March 2019
 */

#include "../lib/idDHT22/src/idDHT22/idDHT22.h"
#include <Particle.h>

int idDHT22pin = D4;
void dht22_wrapper();
bool handleDHTResult(int result);

idDHT22 DHT22(idDHT22pin, dht22_wrapper);

void dht22_wrapper()
{
  DHT22.isrCallback();
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print("\nRetrieving information from sensor: ");
  Serial.print("Read sensor: ");

  DHT22.acquire();
  while (DHT22.acquiring());

  int result = DHT22.getStatus();
  if (false == handleDHTResult(result))
  {
    delay(2000);
    return;
  }

  Serial.print("Temperature (C): ");
  float temperature =  DHT22.getCelsius();
  Serial.println(temperature, 2);
  Particle.publish("Temperature (C)", String(temperature), PRIVATE);

  delay(2000);
}

bool handleDHTResult(int result)
{
  switch (result)
  {
  case IDDHTLIB_OK:
    Serial.println("OK");
    return true;
  case IDDHTLIB_ERROR_CHECKSUM:
    Serial.println("Error\n\r\tChecksum error");
    break;
  case IDDHTLIB_ERROR_ISR_TIMEOUT:
    Serial.println("Error\n\r\tISR Time out error");
    break;
  case IDDHTLIB_ERROR_RESPONSE_TIMEOUT:
    Serial.println("Error\n\r\tResponse time out error");
    break;
  case IDDHTLIB_ERROR_DATA_TIMEOUT:
    Serial.println("Error\n\r\tData time out error");
    break;
  case IDDHTLIB_ERROR_ACQUIRING:
    Serial.println("Error\n\r\tAcquiring");
    break;
  case IDDHTLIB_ERROR_DELTA:
    Serial.println("Error\n\r\tDelta time to small");
    break;
  case IDDHTLIB_ERROR_NOTSTARTED:
    Serial.println("Error\n\r\tNot started");
    break;
  default:
    Serial.println("Unknown error");
    break;
  }

  return false;
}