#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"
#include "Point.h"
#include "Vector2D.h"
#include "Engine.h"


class Camera
{
	public:
		inline static Camera* GetInstance() {
			if (s_Instance != nullptr)
				return s_Instance;
			else
				return s_Instance = new Camera();
		}

		inline SDL_Rect GetViewBox() { return m_ViewBox; }
		inline Vector2D GetPosition() { return m_Position; }
		inline void SetTarget(Point* target) { m_Target = target; }

		void Update(float dt);

	private:
		Camera() { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT; };

		Point* m_Target;
		Vector2D m_Position;

		SDL_Rect m_ViewBox;
		static Camera* s_Instance;


};


#endif

