#!/bin/bash

docker run -i --rm --device /dev/ttyUSB0 --device /dev/ttyUSB1 -v /home/$USER/.nuget:/home/$USER/.nuget -v /var/run/docker.sock:/var/run/docker.sock -v $PWD:/project compulsivecoder/ubuntu-arm-iot-mono /bin/bash -c "git clone http://github.com/GreenSense/VoltageCurrentMAX471SensorCalibratedSerial && cd VoltageCurrentMAX471SensorCalibratedSerial && sh init.sh && sh build.sh && sh upload.sh && sh upload-simulator.sh && sh test.sh"
