#include <BMP280_DEV.h>
#include <Device.h>
#include <JetiExProtocol.h>



const byte idAlt = 1;
const byte idVario = 2;
const byte idPress = 3;
const byte idVbat = 4;
const byte idTemp = 5;

const byte pinVbat = 0;
const word sendDelay = 500;


float altitude;
unsigned int trueAltitude;
unsigned int groundAltitude;
float vario;
float pressure;
float temp;
unsigned int rawVbat;
float vBat;
float varioTimer;
float sendTimer;


bool wait = false;


JetiExProtocol Ext;
BMP280_DEV bmp;


JETISENSOR_CONST sensors[] PROGMEM =

{

  {idAlt,     "Altitude",            "m",       JetiSensor::TYPE_14b, 0},
  {idVario,   "Vario",               "m",       JetiSensor::TYPE_14b, 1},
  {idPress,   "Pression",            "HPa",     JetiSensor::TYPE_14b, 0},
  {idVbat,    "Tension Batterie,     "V",       JetiSensir::TYPE_14b, 2},
  {idTemp,    "Temperature",         "\xB0",    JetiSensor::TYPE_14b, 0},
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

  rawVbat = analogRead(pinVbat);

  vBat = map(rawVbat,0,1023,0,10);

  bmp.getMeasurements(temp, pressure, altitude);

  trueAltitude = altitude-groundAltitude;

  if(millis()>=sendTimer+sendDelay)

  {

    Ext.SetSensorValue(idAlt, trueAltitude);
    Ext.SetSensorValue(idVArio, vario);
    Ext.SetSensorValue(idPress, pressure);
    Ext.SetSensorValue(idVbat, vBat);
    Ext.SetSensorValue(idTemp, temp);

    Ext.DoJetiSend();

  }
  
  
}
