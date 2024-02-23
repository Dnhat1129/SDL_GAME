#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"

class Warrior
{
	public:
		Warrior(Properties props);

		virtual void Draw() = 0;
		virtual void Upadate(float dt) = 0;
		virtual void Clean() = 0;
	private:
		int m_Row, m_Frame, m_FrameCount;
};


#endif 



