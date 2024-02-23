#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.h"
#include <string>

class Character: public GameObject
{
	public:
		Character(Properties props) : GameObject(pros);
};
#endif

