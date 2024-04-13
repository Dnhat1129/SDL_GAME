/*#include "Timer.h"
#include "SDL.h"

Timer* Timer::s_Instance = nullptr;


Timer::Timer() {
	m_DeltaTime = 0.0f;
	m_LastTime = 0.0f;
}

void Timer::Tick() {
	m_DeltaTime = (SDL_GetTicks() - m_LastTime) * (TARGET_FPS / 1000.0f );
	
	if (m_DeltaTime > TARGET_DELTATIME) {
		m_DeltaTime = TARGET_DELTATIME;
	}
	m_LastTime = SDL_GetTicks();
}*/

#include "Timer.h"
#include "SDL.h"

Timer* Timer::s_Instance = nullptr;

Timer::Timer() {
    m_DeltaTime = 0.0f;
    m_LastTime = 0;
}

void Timer::Tick() {
    // L?y th?i ?i?m hi?n t?i
    Uint32 currentTime = SDL_GetTicks();

    // Tính delta time d?a trên th?i gian trôi qua t? l?n g?i tr??c ??n l?n g?i hi?n t?i
    m_DeltaTime = static_cast<float>((currentTime - m_LastTime)) * (TARGET_FPS / 1000.0f);

    // Gi?i h?n delta time ?? tránh gi?m t?c ?? khi quá l?n
    if (m_DeltaTime > TARGET_DELTATIME) {
        m_DeltaTime = TARGET_DELTATIME;
    }

    // C?p nh?t th?i ?i?m cu?i cùng
    m_LastTime = currentTime;
}