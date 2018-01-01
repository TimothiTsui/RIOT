//#pragma once
//
//#include <iostream>
//#include <string>
//
//class phSensor
//{
//public:
//	phSensor(int position);
//	~phSensor();
//	float getNewPHReading();
//	float getPHReading();
//	int getBusAddress();
//	int getSlotPosition();
//	float getTempCompensation();
//	bool setTempCompensation(float newTemp);
//	int getCalibrationStatus();
//	bool clearCalibration();
//	bool lowpointCalibration(float phVal);
//	bool midpointCalibration(float phVal);
//	bool highpointCalibration(float phVal);
//	bool startSleepmode();
//	std::string getDeviceInfo();
//	bool getSlope(float &acidCalibration, float &baseCalibration); //fehlerhaft. manchal z.B.90% aber auch 100% möglich
//	bool setNewBusAddress(int newAddr);
//	bool checkDeviceModell();
//	bool isDisconnected();
//	bool isOperating();
//	void setOperatingStatus(bool running);
//
//private:
////	bool calibration(std::string cmd, float phVal);
//
//	float phValue;
//	int deviceID;
//	int busAddress;
//	int slotPosition;
//	bool running;
//	bool disconnected;
//};
//
//
