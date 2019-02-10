using System;
using NUnit.Framework;

namespace VoltageCurrentMAX471SensorCalibratedSerial.Tests.Integration
{
	[TestFixture(Category = "Integration")]
	public class VoltageCurrentMAX471SensorPowerTestFixture : BaseTestFixture
	{
		[Test]
		public void Test_VoltageCurrentMAX471SensorPower_AlwaysOn_1SecondReadInterval()
		{
			using (var helper = new VoltageCurrentMAX471SensorPowerTestHelper())
			{
				helper.ReadInterval = 1;

				helper.DevicePort = GetDevicePort();
				helper.DeviceBaudRate = GetDeviceSerialBaudRate();

				helper.SimulatorPort = GetSimulatorPort();
				helper.SimulatorBaudRate = GetSimulatorSerialBaudRate();

				helper.TestVoltageCurrentMAX471SensorPower();
			}
		}

		[Test]
		public void Test_VoltageCurrentMAX471SensorPower_AlwaysOn_3SecondReadInterval()
		{
			using (var helper = new VoltageCurrentMAX471SensorPowerTestHelper())
			{
				helper.ReadInterval = 3;

				helper.DevicePort = GetDevicePort();
				helper.DeviceBaudRate = GetDeviceSerialBaudRate();

				helper.SimulatorPort = GetSimulatorPort();
				helper.SimulatorBaudRate = GetSimulatorSerialBaudRate();

				helper.TestVoltageCurrentMAX471SensorPower();
			}
		}

		[Test]
		public void Test_VoltageCurrentMAX471SensorPower_OnAndOff_4SecondReadInterval()
		{
			using (var helper = new VoltageCurrentMAX471SensorPowerTestHelper())
			{
				helper.ReadInterval = 4;

				helper.DevicePort = GetDevicePort();
				helper.DeviceBaudRate = GetDeviceSerialBaudRate();

				helper.SimulatorPort = GetSimulatorPort();
				helper.SimulatorBaudRate = GetSimulatorSerialBaudRate();

				helper.TestVoltageCurrentMAX471SensorPower();
			}
		}
		[Test]
		public void Test_VoltageCurrentMAX471SensorPower_OnAndOff_6SecondReadInterval()
		{
			using (var helper = new VoltageCurrentMAX471SensorPowerTestHelper())
			{
				helper.ReadInterval = 6;

				helper.DevicePort = GetDevicePort();
				helper.DeviceBaudRate = GetDeviceSerialBaudRate();

				helper.SimulatorPort = GetSimulatorPort();
				helper.SimulatorBaudRate = GetSimulatorSerialBaudRate();

				helper.TestVoltageCurrentMAX471SensorPower();
			}
		}
	}
}
