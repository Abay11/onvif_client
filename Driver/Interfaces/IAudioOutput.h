#pragma once
class IAudioOutput
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Send(void* /*audio_data*/, size_t /*size*/) = 0;
};

