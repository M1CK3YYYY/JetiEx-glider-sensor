//include Lib for BMP and Jeti protocol

#include <BMP280_DEV.h>
#include <Device.h>
#include <JetiExProtocol.h>


//ID for Jeti lib

const byte idAlt = 1;
const byte idVario = 2;
const byte idPress = 3;
const byte idVbat = 4;
const byte idTemp = 5;


//Const for pin

const byte pinVbat = 0;

const word sendDelay = 500;  //Const for send delay


//Measurment variables

float altitude;
unsigned int trueAltitude;
unsigned int groundAltitude;
float vario;
float pressure;
float temp;
unsigned int rawVbat;
float vBat;


//Timer variables

float varioTimer;
float sendTimer;


bool wait = false;


//objects for Jeti and BMP libs

JetiExProtocol Ext;
BMP280_DEV bmp;


JETISENSOR_CONST sensors[] PROGMEM =

{
//ID,         "Name",                "unit"     data type,            precision
  {idAlt,     "Altitude",            "m",       JetiSensor::TYPE_14b, 0},
  {idVario,   "Vario",               "m",       JetiSensor::TYPE_14b, 1},
  {idPress,   "Pression",            "HPa",     JetiSensor::TYPE_14b, 0},
  {idVbat,    "Tension Batterie,     "V",       JetiSensir::TYPE_14b, 2},
  {idTemp,    "Temperature",         "\xB0",    JetiSensor::TYPE_14b, 0},
  { 0 }

};

void setup()

{
  
  //sending FW version on serial
  
  Serial.begin(9600);

  Serial.println("JetiEx-glider-sensor");
  Serial.println("");
  Serial.println("FW version : 0.0.0");

  Serial.end();
  

  delay(500); //delay between serial end and ext start 

  Ext.Start("Jeti Sensor", sensors );  //start comunication with Rx

  bmp.begin();  //start comunication with BMP

  bmp.setTimeStandby(TIME_STANDBY_2000MS);

  bmp.startNormalConversion();  //Set BMP in normal mode
  

  //Read ground altitude first time

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

  //Read and calculate Vbat

  rawVbat = analogRead(pinVbat);

  vBat = map(rawVbat,0,1023,0,10); //be careful !! work only with 2 cell Li-Po and with dividing bridge (use 2 10k resistor)
  

  //Read temp, pressure and altitude value and calculate true altitude (with ground altitude)

  bmp.getCurrentMeasurements(temp, pressure, altitude);

  trueAltitude = altitude-groundAltitude;
  

  //set data and send it to Rx

  if(millis()>=sendTimer+sendDelay)

  {

    //set data

    Ext.SetSensorValue(idAlt, trueAltitude);
    Ext.SetSensorValue(idVArio, vario);
    Ext.SetSensorValue(idPress, pressure);
    Ext.SetSensorValue(idVbat, vBat);
    Ext.SetSensorValue(idTemp, temp);

    Ext.DoJetiSend();  //send data

  }
  
}
