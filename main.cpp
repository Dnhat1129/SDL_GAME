#include "Engine.h"
int main(int arc, char* argv[]) {

	Engine::GetInstance()->Init();
	while (Engine::GetInstance()->IsRunning()) {
		Engine::GetInstance()->Events();
		Engine::GetInstance()->Update();
		Engine::GetInstance()->Render();
	}

	Engine::GetInstance()->Clean();
	return 0;
}