#pragma once

class IDevice
{
public:
	virtual void Init() = 0;
	
	virtual void StartLive() = 0;

	virtual void StopLive() = 0;
private:
};