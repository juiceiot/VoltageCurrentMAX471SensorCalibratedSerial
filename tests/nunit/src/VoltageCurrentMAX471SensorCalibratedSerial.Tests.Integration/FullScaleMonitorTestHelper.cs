using System;

namespace VoltageCurrentMAX471SensorCalibratedSerial.Tests.Integration
{
	public class FullScaleMonitorTestHelper : GreenSenseHardwareTestHelper
	{
		public FullScaleMonitorTestHelper()
		{
		}

		public void RunFullScaleTest()
		{
			WriteTitleText("Starting full scale test");

			ConnectDevices();

			int step = 25;

			for (int i = 100; i >= 0; i -= step)
			{
				RunFullScaleTestSegment(i);
			}

			for (int i = 0; i <= 100; i += step)
			{
				RunFullScaleTestSegment(i);
			}
		}

		public void RunFullScaleTestSegment(int voltagePercentage)
		{
			WriteSubTitleText("Starting full scale test segment");

			Console.WriteLine("Soil moisture: " + voltagePercentage + "%");
			Console.WriteLine("");

			SimulateSoilMoisture(voltagePercentage);

			var data = WaitForData(3); // Wait for 3 data entries to give the simulator time to stabilise

			Console.WriteLine("");
			Console.WriteLine("Checking calibrated value");
			Console.WriteLine("");

			AssertDataValueIsWithinRange(data[data.Length - 1], "C", voltagePercentage, CalibratedValueMarginOfError);

			Console.WriteLine("");
			Console.WriteLine("Checking raw value");
			Console.WriteLine("");

			var expectedRawValue = voltagePercentage * AnalogPinMaxValue / 100;

			AssertDataValueIsWithinRange(data[data.Length - 1], "R", expectedRawValue, RawValueMarginOfError);
		}
	}
}