﻿using System;
using System.Threading;
using NUnit.Framework;
namespace VoltageCurrentMAX471SensorCalibratedSerial.Tests.Integration
{
	public class GreenSenseHardwareTestHelper : HardwareTestHelper
	{
		public int SoilMoistureSimulatorPin = 9;
		public int SoilMoistureSimulatorPowerPin = 3;

		public int DelayAfterTurningVoltageCurrentMAX471SensorOn = 3;

		public int RawValueMarginOfError = 27;
		public int CalibratedValueMarginOfError = 3;
		public double TimeErrorMargin = 0.2;

		public bool CalibrationIsReversedByDefault = true;

		public GreenSenseHardwareTestHelper()
		{
		}

		#region Enable Devices Functions
		public override void ConnectDevices(bool enableSimulator)
		{
			base.ConnectDevices(enableSimulator);

			PrepareDeviceForTest();
		}
		#endregion

		#region Prepare Device Functions
		public virtual void PrepareDeviceForTest()
		{
			PrepareDeviceForTest(true);
		}

		public virtual void PrepareDeviceForTest(bool consoleWriteDeviceOutput)
		{
			ResetDeviceSettings();

			SetDeviceReadInterval(1);

			if (CalibrationIsReversedByDefault)
				ReverseDeviceCalibration();

			if (consoleWriteDeviceOutput)
				ReadFromDeviceAndOutputToConsole();
		}
		#endregion

		#region General Device Command Settings
		public void SendDeviceCommand(string command)
		{
			WriteToDevice(command);

			WaitForMessageReceived(command);
		}

		public void WaitForMessageReceived(string message)
		{
			Console.WriteLine("");
			Console.WriteLine("Waiting for received message");
			Console.WriteLine("  Message: " + message);

			var output = String.Empty;
			var wasMessageReceived = false;

			var startTime = DateTime.Now;

			while (!wasMessageReceived)
			{
				output += ReadLineFromDevice();

				var expectedText = "Received message: " + message;
				if (output.Contains(expectedText))
				{
					wasMessageReceived = true;

					Console.WriteLine("  Message was received");

					ConsoleWriteSerialOutput(output);
				}

				var hasTimedOut = DateTime.Now.Subtract(startTime).TotalSeconds > TimeoutWaitingForResponse;
				if (hasTimedOut && !wasMessageReceived)
				{
					ConsoleWriteSerialOutput(output);

					Assert.Fail("Timed out waiting for message received (" + TimeoutWaitingForResponse + " seconds)");
				}
			}
		}
		#endregion

		#region Specific Device Command Functions
		public void ResetDeviceSettings()
		{
			var cmd = "X";

			Console.WriteLine("");
			Console.WriteLine("Resetting device default settings...");
			Console.WriteLine("  Sending '" + cmd + "' command to device");
			Console.WriteLine("");

			SendDeviceCommand(cmd);
		}

		public void SetDeviceReadInterval(int numberOfSeconds)
		{
			var cmd = "I" + numberOfSeconds;

			Console.WriteLine("");
			Console.WriteLine("Setting device read interval to " + numberOfSeconds + " second(s)...");
			Console.WriteLine("  Sending '" + cmd + "' command to device");
			Console.WriteLine("");

			SendDeviceCommand(cmd);
		}

		public void ReverseDeviceCalibration()
		{
			var cmd = "R";

			Console.WriteLine("");
			Console.WriteLine("Reversing device calibration settings...");
			Console.WriteLine("  Sending '" + cmd + "' command to device");
			Console.WriteLine("");

			SendDeviceCommand(cmd);
		}
		#endregion

		#region Soil Moisture Simulator Functions
		public void SimulateSoilMoisture(int voltagePercentage)
		{
			Console.WriteLine("");
			Console.WriteLine("Simulating soil moisture percentage");
			Console.WriteLine("  Sending analog percentage");
			Console.WriteLine("    PWM pin: " + SoilMoistureSimulatorPin);
			Console.WriteLine("    Soil Moisture Percentage: " + voltagePercentage + "%");
			Console.WriteLine("");

			SimulatorClient.AnalogWritePercentage(SoilMoistureSimulatorPin, voltagePercentage);
		}
		#endregion

		#region Wait for Pin Functions
		public int WaitUntilVoltageCurrentMAX471SensorPowerPinIs(bool expectedValue)
		{
			return WaitUntilSimulatorPinIs("soil moisture sensor power", SoilMoistureSimulatorPowerPin, expectedValue);
		}

		public double WaitWhileVoltageCurrentMAX471SensorPowerPinIs(bool expectedValue)
		{
			return WaitWhileSimulatorPinIs("soil moisture sensor power", SoilMoistureSimulatorPowerPin, expectedValue);
		}
		#endregion

		#region Assert Simulator Pin Functions
		public void AssertVoltageCurrentMAX471SensorPowerPinForDuration(bool expectedValue, int durationInSeconds)
		{
			AssertSimulatorPinForDuration("soil moisture sensor power", SoilMoistureSimulatorPowerPin, expectedValue, durationInSeconds);
		}
		#endregion
	}
}