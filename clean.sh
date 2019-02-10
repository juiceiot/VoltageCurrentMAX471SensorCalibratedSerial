#pio run --target clean


SOURCE_FILE="src/VoltageCurrentMAX471SensorCalibratedSerial/VoltageCurrentMAX471SensorCalibratedSerial.ino"

sed -i "s/#define VERSION .*/#define VERSION \"1-0-0-0\"/" $SOURCE_FILE
