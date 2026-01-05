#include <BMP280_DEV.h>
#include <Device.h>
#include <JetiExProtocol.h>



const byte idAlt = 1;
const byte idPress = 2;
const byte idTemp = 3;


float altitude;
float pressure;
float groundPressure;
float trueAltitude;
float temp;
bool wait = false;


JetiExProtocol Ext;
BMP280 bmp;


JETISENSOR_CONST sensors[] PROGMEM =

{

  {idAlt,     "Altitude",     "m",       JetiSensor::TYPE_14b, 0},
  {idPress,   "Pression",     "HPa",     JetiSensor::TYPE_14b, 0},
  {idTemp,    "Temperature",  "\xB0",    JetiSensor::TYPE_14b, 0},
  { 0 }

};

void setup()

{

  Ext.Start("Jeti Sensor", sensors );

  bmp.begin();

  bmp.setTimeStandby(TIME_STANDBY_2000MS);

  bmp.startNormalConversion();

  while (wait == false)

    {

      if (bmp.getMeasurements(temp, pressure, altitude))

      {

        groundAltitude = altitude;

        wait = true;
        
      }
      
    }

}

void loop()

{

  bmp.getMeasurements(temp, pressure, altitude);

  trueAltitude = altitude-groundAltitude;
  
  Ext.SetSensorValue(idAlt, trueAltitude);
  Ext.SetSensorValue(idPress, pressure);
  Ext.SetSensorValue(idTemp, temp);

  Ext.DoJetiSend();

}
//test de julien
