#include "Engine.h"
#include "Timer.h"



int main(int arc, char* argv[]) {

	Engine::GetInstance()->Init();
	Engine::GetInstance()->GetPrevious();
	while (Engine::GetInstance()->IsRunning()) {
		Engine::GetInstance()->Luu();
		Engine::GetInstance()->Events();
		Engine::GetInstance()->Update();
		Engine::GetInstance()->Render();
		Timer::GetInstance()->Tick();
	}

	Engine::GetInstance()->Clean();
	return 0;
}