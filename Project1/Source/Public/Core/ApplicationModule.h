#pragma once

class FApplicationModule {

public:
	virtual void OnStartup() {};
	virtual void OnShutdown() {};

	virtual void OnTick(float Delta) {}
};