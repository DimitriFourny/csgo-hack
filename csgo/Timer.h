#pragma once

class Timer {
public:
	Timer();
	void start(unsigned int ms);
	bool isElapsed();

private:
	unsigned int m_started;
	unsigned int m_waitingTime;
};