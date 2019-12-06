#pragma once

class IDevice
{
public:
	virtual void Init() = 0;

	virtual void SetCreds(const char* login, const char* pass) = 0;
	
	virtual void StartLive() = 0;

	virtual void StopLive() = 0;
private:
};