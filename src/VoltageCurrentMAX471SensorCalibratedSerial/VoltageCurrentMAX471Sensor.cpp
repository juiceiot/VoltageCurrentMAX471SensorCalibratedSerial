#include <Arduino.h>
#include <EEPROM.h>

#include <duinocom.h>

#include "Common.h"
#include "VoltageCurrentMAX471Sensor.h"

#define voltagePin A0
#define currentPin A1

bool voltageCurrentMAX471SensorIsOn = true;
long lastSensorOnTime = 0;

bool voltageCurrentMAX471SensorReadingHasBeenTaken = false;
long voltageCurrentMAX471SensorReadingIntervalInSeconds = 5;
long lastVoltageCurrentMAX471SensorReadingTime = 0; // Milliseconds

float voltageCalibrated = 0;
float voltageRaw = 0;

float currentCalibrated = 0;
float currentRaw = 0;

int drySoilMoistureCalibrationValue = ANALOG_MAX;
int wetSoilMoistureCalibrationValue = 0;

#define voltageCurrentMAX471SensorIsCalibratedFlagAddress 1
#define drySoilMoistureCalibrationValueAddress 2
#define wetSoilMoistureCalibrationValueAddress 6

#define voltageCurrentMAX471SensorReadIntervalIsSetFlagAddress 10
#define voltageCurrentMAX471SensorReadingIntervalAddress 13

/* Setup */
void setupVoltageCurrentMAX471Sensor()
{
  setupCalibrationValues();

  setupVoltageCurrentMAX471SensorReadingInterval();
}

/* Sensor Readings */
void takeVoltageCurrentMAX471SensorReading()
{
  bool sensorReadingIsDue = lastVoltageCurrentMAX471SensorReadingTime + secondsToMilliseconds(voltageCurrentMAX471SensorReadingIntervalInSeconds) < millis()
    || lastVoltageCurrentMAX471SensorReadingTime == 0;

  if (sensorReadingIsDue)
  {
    if (isDebugMode)
      Serial.println("Sensor reading is due");

  	bool voltageCurrentMAX471SensorIsOnAndReady = voltageCurrentMAX471SensorIsOn;

    if (voltageCurrentMAX471SensorIsOnAndReady)
    {
      if (isDebugMode)
        Serial.println("Preparing to take reading");

      lastVoltageCurrentMAX471SensorReadingTime = millis();


      int rawVoltageReading = 0;
      int rawCurrentReading = 0;

      int numReadings = 10;
      int totalVoltageReading = 0;
      int totalCurrentReading = 0;

      for (int i = 0; i < numReadings; i++)
      {
        totalVoltageReading += analogRead(voltagePin);
        totalCurrentReading += analogRead(currentPin);

        delay(20);
      }

      rawVoltageReading = totalVoltageReading / numReadings;
      rawCurrentReading = totalCurrentReading / numReadings;

      float vcc = readVcc() / 1000.0;  // convert mV to V
      voltageRaw = ((rawVoltageReading * vcc) / 1023) * vcc ;
      currentRaw = (rawCurrentReading * vcc) / 1023;
      
/*      voltageRaw = getAverageVoltageCurrentMAX471SensorReading();

      voltageCalibrated = calculateSoilMoistureLevel(voltageRaw);

      if (voltageCalibrated < 0)
        voltageCalibrated = 0;

      if (voltageCalibrated > 100)
        voltageCalibrated = 100;
*/

      voltageCurrentMAX471SensorReadingHasBeenTaken = true;
    }
  }
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

double getAverageVoltageCurrentMAX471SensorReading()
{
  int readingSum  = 0;
  int totalReadings = 10;

  for (int i = 0; i < totalReadings; i++)
  {
    int reading = analogRead(voltagePin);

    readingSum += reading;
  }

  double averageReading = readingSum / totalReadings;

  return averageReading;
}

double calculateSoilMoistureLevel(int voltageCurrentMAX471SensorReading)
{
  return map(voltageCurrentMAX471SensorReading, drySoilMoistureCalibrationValue, wetSoilMoistureCalibrationValue, 0, 100);
}

/* Reading interval */
void setupVoltageCurrentMAX471SensorReadingInterval()
{
  bool eepromIsSet = EEPROM.read(voltageCurrentMAX471SensorReadIntervalIsSetFlagAddress) == 99;

  if (eepromIsSet)
  {
    if (isDebugMode)
    	Serial.println("EEPROM read interval value has been set. Loading.");

    voltageCurrentMAX471SensorReadingIntervalInSeconds = getVoltageCurrentMAX471SensorReadingInterval();
  }
  else
  {
    if (isDebugMode)
      Serial.println("EEPROM read interval value has not been set. Using defaults.");
  }
}

void setVoltageCurrentMAX471SensorReadingInterval(char* msg)
{
    int value = readInt(msg, 1, strlen(msg)-1);

    setVoltageCurrentMAX471SensorReadingInterval(value);
}

void setVoltageCurrentMAX471SensorReadingInterval(long newValue)
{
  if (isDebugMode)
  {
    Serial.print("Set sensor reading interval: ");
    Serial.println(newValue);
  }

  EEPROMWriteLong(voltageCurrentMAX471SensorReadingIntervalAddress, newValue);

  setEEPROMVoltageCurrentMAX471SensorReadingIntervalIsSetFlag();

  voltageCurrentMAX471SensorReadingIntervalInSeconds = newValue; 

  serialOutputIntervalInSeconds = newValue;
}

long getVoltageCurrentMAX471SensorReadingInterval()
{
  long value = EEPROMReadLong(voltageCurrentMAX471SensorReadingIntervalAddress);

  if (value == 0
      || value == 255)
    return voltageCurrentMAX471SensorReadingIntervalInSeconds;
  else
  {
    if (isDebugMode)
    {
      Serial.print("Read interval found in EEPROM: ");
      Serial.println(value);
    }

    return value;
  }
}

void setEEPROMVoltageCurrentMAX471SensorReadingIntervalIsSetFlag()
{
  if (EEPROM.read(voltageCurrentMAX471SensorReadIntervalIsSetFlagAddress) != 99)
    EEPROM.write(voltageCurrentMAX471SensorReadIntervalIsSetFlagAddress, 99);
}

void removeEEPROMVoltageCurrentMAX471SensorReadingIntervalIsSetFlag()
{
    EEPROM.write(voltageCurrentMAX471SensorReadIntervalIsSetFlagAddress, 0);
}

/* Calibration */
void setupCalibrationValues()
{
  bool eepromIsSet = EEPROM.read(voltageCurrentMAX471SensorIsCalibratedFlagAddress) == 99;

  if (eepromIsSet)
  {
    if (isDebugMode)
    	Serial.println("EEPROM calibration values have been set. Loading.");

    drySoilMoistureCalibrationValue = getDrySoilMoistureCalibrationValue();
    wetSoilMoistureCalibrationValue = getWetSoilMoistureCalibrationValue();
  }
  else
  {
    if (isDebugMode)
      Serial.println("EEPROM calibration values have not been set. Using defaults.");
    
    //setDrySoilMoistureCalibrationValue(drySoilMoistureCalibrationValue);
    //setWetSoilMoistureCalibrationValue(wetSoilMoistureCalibrationValue);
  }
}

void setDrySoilMoistureCalibrationValue(char* msg)
{
  int length = strlen(msg);

  if (length == 1)
    setDrySoilMoistureCalibrationValueToCurrent();
  else
  {
    int value = readInt(msg, 1, length-1);

//    Serial.println("Value:");
//    Serial.println(value);

    setDrySoilMoistureCalibrationValue(value);
  }
}

void setDrySoilMoistureCalibrationValueToCurrent()
{
  lastVoltageCurrentMAX471SensorReadingTime = 0;
  takeVoltageCurrentMAX471SensorReading();
  setDrySoilMoistureCalibrationValue(voltageRaw);
}

void setDrySoilMoistureCalibrationValue(int newValue)
{
  if (isDebugMode)
  {
    Serial.print("Setting dry voltage/current sensor calibration value: ");
    Serial.println(newValue);
  }

  drySoilMoistureCalibrationValue = newValue;
  
  EEPROMWriteLong(drySoilMoistureCalibrationValueAddress, newValue); // Must divide by 4 to make it fit in eeprom

  setEEPROMIsCalibratedFlag();
}

void setWetSoilMoistureCalibrationValue(char* msg)
{
  int length = strlen(msg);

  if (length == 1)
    setWetSoilMoistureCalibrationValueToCurrent();
  else
  {
    int value = readInt(msg, 1, length-1);

//    Serial.println("Value:");
//    Serial.println(value);

    setWetSoilMoistureCalibrationValue(value);
  }
}

void setWetSoilMoistureCalibrationValueToCurrent()
{
  lastVoltageCurrentMAX471SensorReadingTime = 0;
  takeVoltageCurrentMAX471SensorReading();
  setWetSoilMoistureCalibrationValue(voltageRaw);
}

void setWetSoilMoistureCalibrationValue(int newValue)
{
  if (isDebugMode)
  {
    Serial.print("Setting wet voltage/current sensor calibration value: ");
    Serial.println(newValue);
  }

  wetSoilMoistureCalibrationValue = newValue;

  EEPROMWriteLong(wetSoilMoistureCalibrationValueAddress, newValue);
  
  setEEPROMIsCalibratedFlag();
}

int getDrySoilMoistureCalibrationValue()
{
  int value = EEPROMReadLong(drySoilMoistureCalibrationValueAddress);

  if (value < 0
      || value > ANALOG_MAX)
    return drySoilMoistureCalibrationValue;
  else
  {
    int dryVoltageCurrentMAX471SensorValue = value;
  
    if (isDebugMode)
    {
      Serial.print("Dry calibration value found in EEPROM: ");
      Serial.println(dryVoltageCurrentMAX471SensorValue);
    }

    return dryVoltageCurrentMAX471SensorValue;
  }
}

int getWetSoilMoistureCalibrationValue()
{
  int value = EEPROMReadLong(wetSoilMoistureCalibrationValueAddress);

  if (value < 0
      || value > ANALOG_MAX)
    return wetSoilMoistureCalibrationValue;
  else
  {
    if (isDebugMode)
    {
      Serial.print("Wet calibration value found in EEPROM: ");
      Serial.println(value);
    }
  }

  return value;
}

void setEEPROMIsCalibratedFlag()
{
  if (EEPROM.read(voltageCurrentMAX471SensorIsCalibratedFlagAddress) != 99)
    EEPROM.write(voltageCurrentMAX471SensorIsCalibratedFlagAddress, 99);
}

void removeEEPROMIsCalibratedFlag()
{
    EEPROM.write(voltageCurrentMAX471SensorIsCalibratedFlagAddress, 0);
}

void restoreDefaultVoltageCurrentMAX471SensorSettings()
{
  restoreDefaultCalibrationSettings();
  restoreDefaultVoltageCurrentMAX471SensorReadingIntervalSettings();
}

void restoreDefaultVoltageCurrentMAX471SensorReadingIntervalSettings()
{
  removeEEPROMVoltageCurrentMAX471SensorReadingIntervalIsSetFlag();

  voltageCurrentMAX471SensorReadingIntervalInSeconds = 5;
  serialOutputIntervalInSeconds = 5;

  setVoltageCurrentMAX471SensorReadingInterval(voltageCurrentMAX471SensorReadingIntervalInSeconds);
}

void restoreDefaultCalibrationSettings()
{
  removeEEPROMIsCalibratedFlag();

  drySoilMoistureCalibrationValue = 0;
  wetSoilMoistureCalibrationValue = ANALOG_MAX;

  setDrySoilMoistureCalibrationValue(drySoilMoistureCalibrationValue);
  setWetSoilMoistureCalibrationValue(wetSoilMoistureCalibrationValue);
}
