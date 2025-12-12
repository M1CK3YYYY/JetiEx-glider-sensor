#include <JetiExProtocol.h>
#include <BMP280.h>


const byte idAlt = 1;
const byte idPress = 2;
const byte idTemp = 3;


word altitude;
word pressure;
word groundPressure;
byte temp;


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

  Wire.begin();

  bmp.begin();

  groundPressure = bmp.getPressure()*0.01;

}

void loop()

{

  pressure = bmp.getPressure()*0.01;

  temp = bmp.getTemperature();


  altitude = 44340 * (1.0 - pow(pressure / groundPressure, 0.1905));
  
  Ext.SetSensorValue(idAlt, altitude);
  Ext.SetSensorValue(idPress, pressure);
  Ext.SetSensorValue(idTemp, temp);

  Ext.DoJetiSend();

}

//test de julien
