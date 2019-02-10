echo "Starting build for project"
echo "Dir: $PWD"

DIR=$PWD

xbuild src/VoltageCurrentMAX471SensorCalibratedSerial.sln /p:Configuration=Release
