#ifndef TIMER_H
#define TIMER_H

#include "SDL.h"

const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 1.5f;

class Timer {
public:
	void Tick();
	inline float GetDeltaTime() { return m_DeltaTime; }

	inline static Timer* GetInstance() {
		if (s_Instance != nullptr)
			return s_Instance;
		else
			return s_Instance = new Timer();
	}
private:
	Timer();
	static Timer* s_Instance;
	float m_DeltaTime;
	Uint32 m_LastTime;
};

#endif // !TIMER_H



