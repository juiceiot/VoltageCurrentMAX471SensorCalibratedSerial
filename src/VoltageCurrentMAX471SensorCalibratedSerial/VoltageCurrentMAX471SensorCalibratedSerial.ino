#include <Arduino.h>
#include <EEPROM.h>

#include <duinocom.h>

#include "Common.h"
#include "VoltageCurrentMAX471Sensor.h"

#define SERIAL_MODE_CALIBRATED 1
#define SERIAL_MODE_RAW 2
#define SERIAL_MODE_CSV 3
#define SERIAL_MODE_QUERYSTRING 4

#define VERSION "1-0-0-0"

int serialMode = SERIAL_MODE_CSV;

void setup()
{
  Serial.begin(9600);

  Serial.println("Starting voltage/current monitor");

  setupVoltageCurrentMAX471Sensor();

  serialOutputIntervalInSeconds = voltageCurrentMAX471SensorReadingIntervalInSeconds;
}

void loop()
{
  loopNumber++;

  serialPrintLoopHeader();

  checkCommand();

  takeVoltageCurrentMAX471SensorReading();

  serialPrintData();

  serialPrintLoopFooter();

  delay(1);
}

/* Commands */
void checkCommand()
{
  if (isDebugMode)
  {
    Serial.println("Checking incoming serial commands");
  }

  if (checkMsgReady())
  {
    char* msg = getMsg();
        
    char letter = msg[0];

    int length = strlen(msg);

    Serial.print("Received message: ");
    Serial.println(msg);

    switch (letter)
    {
      case 'D':
        setDrySoilMoistureCalibrationValue(msg);
        break;
      case 'W':
        setWetSoilMoistureCalibrationValue(msg);
        break;
      case 'I':
        setVoltageCurrentMAX471SensorReadingInterval(msg);
        break;
      case 'X':
        restoreDefaultSettings();
        break;
      case 'Z':
        Serial.println("Toggling isDebugMode");
        isDebugMode = !isDebugMode;
        break;
    }

    forceSerialOutput();
  }
  delay(1);
}

/* Settings */
void restoreDefaultSettings()
{
  Serial.println("Restoring default settings");

  restoreDefaultVoltageCurrentMAX471SensorSettings();
}

/* Serial Output */
void serialPrintData()
{
  bool isTimeToPrintData = lastSerialOutputTime + secondsToMilliseconds(serialOutputIntervalInSeconds) < millis()
      || lastSerialOutputTime == 0;

  bool isReadyToPrintData = isTimeToPrintData && voltageCurrentMAX471SensorReadingHasBeenTaken;

  if (isReadyToPrintData)
  {
    if (isDebugMode)
      Serial.println("Ready to serial print data");
  
    long numberOfSecondsOnline = millis()/1000;

    if (serialMode == SERIAL_MODE_CSV)
    {
      Serial.print("D;"); // This prefix indicates that the line contains data.
      //Serial.print("T:");
      //Serial.print(numberOfSecondsOnline);
      //Serial.print(";");
      Serial.print("VR:");
      Serial.print(voltageRaw);
      Serial.print(";");
      Serial.print("VC:");
      Serial.print(voltageCalibrated);
      Serial.print(";");
      Serial.print("CR:");
      Serial.print(currentRaw);
      Serial.print(";");
      Serial.print("CC:");
      Serial.print(currentCalibrated);
      Serial.print(";");
      Serial.print("I:");
      Serial.print(voltageCurrentMAX471SensorReadingIntervalInSeconds);
      Serial.print(";");
      Serial.print("D:");
      Serial.print(drySoilMoistureCalibrationValue);
      Serial.print(";");
      Serial.print("W:");
      Serial.print(wetSoilMoistureCalibrationValue);
      Serial.print(";");
      Serial.print("Z:");
      Serial.print(VERSION);
      Serial.print(";;");
      Serial.println();
    }
    else if (serialMode == SERIAL_MODE_QUERYSTRING)
    {
      //Serial.print("time=");
      //Serial.print(numberOfSecondsOnline);
      //Serial.print("&");
      Serial.print("raw=");
      Serial.print(voltageRaw);
      Serial.print("&");
      Serial.print("calibrated=");
      Serial.print(voltageCalibrated);
      Serial.print("&");
      Serial.print("readInterval=");
      Serial.print(voltageCurrentMAX471SensorReadingIntervalInSeconds); // Convert to seconds
      Serial.print("&");
      Serial.print("dry=");
      Serial.print(drySoilMoistureCalibrationValue);
      Serial.print("&");
      Serial.print("wet=");
      Serial.print(wetSoilMoistureCalibrationValue);
      Serial.println();
    }
    else if (serialMode == SERIAL_MODE_CALIBRATED)
    {
      Serial.println(voltageCalibrated);
    }
    else if (serialMode == SERIAL_MODE_RAW)
    {
      Serial.println(voltageRaw);
    }

    lastSerialOutputTime = millis();
  }
  else
  {
    if (isDebugMode)
    {    
      Serial.println("Not ready to serial print data");

      Serial.print("  Is time to serial print data: ");
      Serial.println(isTimeToPrintData);
      if (!isTimeToPrintData)
      {
        Serial.print("    Time remaining before printing data: ");
        Serial.print(millisecondsToSecondsWithDecimal(lastSerialOutputTime + secondsToMilliseconds(serialOutputIntervalInSeconds) - millis()));
        Serial.println(" seconds");
      }
      Serial.print("  voltage/current sensor reading has been taken: ");
      Serial.println(voltageCurrentMAX471SensorReadingHasBeenTaken);
      Serial.print("  Is ready to print data: ");
      Serial.println(isReadyToPrintData);

    }
  }
}
