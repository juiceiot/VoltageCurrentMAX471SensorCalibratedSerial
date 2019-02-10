using System;
using System.Threading;
using NUnit.Framework;
namespace VoltageCurrentMAX471SensorCalibratedSerial.Tests.Integration
{
	public class VoltageCurrentMAX471SensorPowerTestHelper : GreenSenseHardwareTestHelper
	{
		public int ReadInterval = 1;

		public void TestVoltageCurrentMAX471SensorPower()
		{
			WriteTitleText("Starting soil moisture sensor power test");

			ConnectDevices();

			SetDeviceReadInterval(ReadInterval);

			var data = WaitForDataEntry();

			AssertDataValueEquals(data, "I", ReadInterval);

			var sensorDoesTurnOff = ReadInterval > DelayAfterTurningVoltageCurrentMAX471SensorOn;

			if (sensorDoesTurnOff)
			{
				Console.WriteLine("The soil moisture sensor should turn off when not in use.");

				TestVoltageCurrentMAX471SensorPowerTurnsOnAndOff();
			}
			else
			{
				Console.WriteLine("The soil moisture sensor should stay on permanently.");

				TestVoltageCurrentMAX471SensorPowerStaysOn();
			}
		}

		public void TestVoltageCurrentMAX471SensorPowerStaysOn()
		{
			WriteParagraphTitleText("Waiting until the soil moisture sensor is on before starting the test...");

			WaitUntilVoltageCurrentMAX471SensorPowerPinIs(On);

			var durationInSeconds = ReadInterval * 5;

			WriteParagraphTitleText("Checking that soil moisture sensor power pin stays on...");

			AssertVoltageCurrentMAX471SensorPowerPinForDuration(On, durationInSeconds);
		}

		public void TestVoltageCurrentMAX471SensorPowerTurnsOnAndOff()
		{
			WriteParagraphTitleText("Waiting until the soil moisture sensor has turned on then off before starting the test...");

			WaitUntilVoltageCurrentMAX471SensorPowerPinIs(On);
			WaitUntilVoltageCurrentMAX471SensorPowerPinIs(Off);

			CheckVoltageCurrentMAX471SensorOnDuration();
			CheckVoltageCurrentMAX471SensorOffDuration();
		}

		public void CheckVoltageCurrentMAX471SensorOnDuration()
		{
			WriteParagraphTitleText("Getting the total on time...");

			var totalOnTime = WaitWhileVoltageCurrentMAX471SensorPowerPinIs(On);

			WriteParagraphTitleText("Checking the total on time is correct...");

			var expectedOnTime = DelayAfterTurningVoltageCurrentMAX471SensorOn;

			AssertIsWithinRange("total on time", expectedOnTime, totalOnTime, TimeErrorMargin);
		}

		public void CheckVoltageCurrentMAX471SensorOffDuration()
		{
			WriteParagraphTitleText("Getting the total off time...");

			var totalOffTime = WaitWhileVoltageCurrentMAX471SensorPowerPinIs(Off);

			WriteParagraphTitleText("Checking the total off time is correct...");

			var expectedOffTime = ReadInterval - DelayAfterTurningVoltageCurrentMAX471SensorOn;

			AssertIsWithinRange("total off time", expectedOffTime, totalOffTime, TimeErrorMargin);
		}
	}
}
