#ifndef VOLTAGE_CURRENT_MAX471_Sensor_H_
#define VOLTAGE_CURRENT_MAX471_Sensor_H_

extern float voltageCalibrated;
extern float voltageRaw;

extern float currentCalibrated;
extern float currentRaw;

extern long lastVoltageCurrentMAX471SensorReadingTime;
extern long voltageCurrentMAX471SensorReadingIntervalInSeconds;
extern int voltageCurrentMAX471SensorReadIntervalIsSetFlagAddress;

extern int drySoilMoistureCalibrationValue;
extern int wetSoilMoistureCalibrationValue;

extern long lastSensorOnTime;
extern bool voltageCurrentMAX471SensorReadingHasBeenTaken;

void setupVoltageCurrentMAX471Sensor();

void setupCalibrationValues();

void setupVoltageCurrentMAX471SensorReadingInterval();

void takeVoltageCurrentMAX471SensorReading();

double getAverageVoltageCurrentMAX471SensorReading();

double calculateSoilMoistureLevel(int voltageCurrentMAX471SensorReading);

void setEEPROMIsCalibratedFlag();

void setVoltageCurrentMAX471SensorReadingInterval(char* msg);
void setVoltageCurrentMAX471SensorReadingInterval(long readInterval);

long getVoltageCurrentMAX471SensorReadingInterval();

void setEEPROMVoltageCurrentMAX471SensorReadingIntervalIsSetFlag();
void removeEEPROMVoltageCurrentMAX471SensorReadingIntervalIsSetFlag();

void setDrySoilMoistureCalibrationValue(char* msg);

void setDrySoilMoistureCalibrationValueToCurrent();

void setDrySoilMoistureCalibrationValue(int drySoilMoistureCalibrationValue);

void setWetSoilMoistureCalibrationValue(char* msg);

void setWetSoilMoistureCalibrationValueToCurrent();

void setWetSoilMoistureCalibrationValue(int wetSoilMoistureCalibrationValue);

int getDrySoilMoistureCalibrationValue();

int getWetSoilMoistureCalibrationValue();

void setEEPROMIsCalibratedFlag();

void removeEEPROMIsCalibratedFlag();

void restoreDefaultVoltageCurrentMAX471SensorSettings();
void restoreDefaultVoltageCurrentMAX471SensorReadingIntervalSettings();
void restoreDefaultCalibrationSettings();

long readVcc();
#endif
/* VOLTAGE_CURRENT_MAX471_Sensor_H_ */
