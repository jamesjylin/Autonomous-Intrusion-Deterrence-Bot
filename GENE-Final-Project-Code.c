bool isApproaching(float waitDuration, int detectDist, int approachDist, tSensors ultraSonic, bool rotateDirection)
{
	int firstDetection = 0, secondDetection = 0;
	if(SensorValue(ultraSonic) > detectDist){
		return false;
	}
	else{
		motor[motorA] = motor[motorD] = 0;
		firstDetection = SensorValue(ultraSonic);
		wait1Msec(waitDuration);
		secondDetection = SensorValue(ultraSonic);
		if((firstDetection - secondDetection) > approachDist){
			return true;
		}
		else
		{
			if(rotateDirection == false)
			{
				motor[motorA] = -15;
				motor[motorD] = 15;
			}
			else
			{
				motor[motorA] = 15;
				motor[motorD] = -15;
			}
			clearTimer(T1);
			while(time1[T1] < 500)
			{
			}
			return false;
		}
	}
}

void releaseSling(float motorPower, float incrementAngle)
{
	nMotorEncoder(motorC) = 0;
	while(nMotorEncoder(motorC) > incrementAngle){
		motor[motorC] = motorPower;
	}
	motor[motorC] = 0;
}

bool isLoaded(float waitDuration, tSensors colorSens)
{
	if(SensorValue(colorSens) == (int)colorWhite)
	{
		return true;
	}
	else
	{
		motor[motorA] = motor[motorD] = 0;
		eraseDisplay();
		displayString(4, "Checking for loaded Marshmallows...");
		for(int timeInterval = 0; timeInterval <= waitDuration - 200; timeInterval += 200)
		{
			if(SensorValue(colorSens) == (int)colorWhite)
			{
				eraseDisplay();
				displayString(4, "Marshmallows loaded.");
				wait1Msec(3000);
				return true;
			}
			wait1Msec(200);
		}
		return false;
	}
}

void dischargeAmmo(int maxAmmo)
{
	for(int shotCount = 0; shotCount < maxAmmo; shotCount++)
	{
		releaseSling(-25,-360/maxAmmo);
		wait1Msec(1500);
	}
	eraseDisplay();
	displayString(4,"Discharged.");
}

void startUp(tSensors colorSens)
{
	if(SensorValue(colorSens) != (int)colorWhite)
	{
		displayString(3,"No Marshmallows!");
		while(SensorValue(colorSens) != (int)colorWhite)
		{
			//eyy lmao the program waits
		}
	}
	eraseDisplay();
	displayString(3,"Marshmallows loaded.");
	displayString(5,"Press up button to discharge");
	displayString(6,"and then start, or just");
	displayString(7,"press enter to start");
	while(!getButtonPress(buttonAny))
	{
	}
	if(getButtonPress(buttonEnter))
	{
		while(getButtonPress(buttonEnter))
		{
			//it's like that dr seuss book
		}
	}
	else if(getButtonPress(buttonUp))
	{
		while(getButtonPress(buttonUp))
		{
		}
		dischargeAmmo(4);
	}
	eraseDisplay();
	displayString(3,"program started");
}

void rotateAngle(int motorPower, int arcAngle, tSensors gyroMeter)
{
	int endAngle1 = arcAngle/2;
	int endAngle2 = -arcAngle/2;
	bool rotateRightDirection = false;

	while(isApproaching(1000,120,10,S4,rotateRightDirection) == false && isLoaded(20000,S2) == true && getButtonPress(buttonEnter) == false){
		displayString(10, "gyro is: %d", getGyroDegrees(gyroMeter));
		if(rotateRightDirection)
		{
			motor[motorA] = motorPower;
			motor[motorD] = -motorPower;
			if(getGyroDegrees(gyroMeter) >= endAngle1)
			{
				rotateRightDirection = false;
			}
		}
		else
		{
			motor[motorA] = -motorPower;
			motor[motorD] = motorPower;
			if(getGyroDegrees(gyroMeter) <= endAngle2)
			{
				rotateRightDirection = true;
			}
		}
	}
}


task main()
{
	SensorType[S2] = sensorEV3_Color;
	wait1Msec(50);
	SensorType[S4] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S2] = modeEV3Color_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
	bool firstFire = false;

	startUp(S2);
	resetGyro(S3);
	while(isLoaded(20000,S2) == true && getButtonPress(buttonEnter) == false)
	{
		rotateAngle(15,180,S3);
			if(firstFire == false)
			{
				releaseSling(-30,-112);
				firstFire = true;
			}
			else
			{
				releaseSling(-30,-47);
			}
				wait1Msec(500);
	}
	motor[motorA] = motor[motorD] = 0;
	while(getButtonPress(buttonAny))
	{
	}
	eraseDisplay();
	displayString(7,"press enter to discharge and end");
	while(!getButtonPress(buttonEnter))
	{
	}
	while(getButtonPress(buttonEnter))
	{
	}
	dischargeAmmo(4);
	eraseDisplay();
	displayString(3,"Program ended.");
	wait1Msec(2000);
}
