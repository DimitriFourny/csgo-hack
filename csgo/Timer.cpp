#include "Timer.h"
#include <Windows.h>

Timer::Timer() {
	m_started = 0;
	m_waitingTime = 0;
}

void Timer::start(unsigned int ms) {
	m_waitingTime = ms;
	m_started = GetTickCount();
}

bool Timer::isElapsed() {
	unsigned int elapsedTime = GetTickCount() - m_started;
	return (elapsedTime >= m_waitingTime);
}