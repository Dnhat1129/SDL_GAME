#include "Animation.h"
#include "TextureManager.h"
#include <iostream>
#include "Warrior.h"


void Animation::Update() {
	m_CurrentFrame = (SDL_GetTicks() / m_Speed) % m_FrameCount;
}

void Animation::Draw(float x, float y, int width, int height, SDL_RendererFlip flip ) {
	TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, width, height, m_SpriteRow, m_CurrentFrame, flip);
}
void Animation::SetProps(std::string textureID, int spriteRow, int frameCount, int speed) {
	m_TextureID = textureID;
	m_SpriteRow = spriteRow;
	m_FrameCount = frameCount;
	m_Speed = speed;
}

void Animation::DrawKame(float x, float y, int width, int height, int currentframe, SDL_RendererFlip flip) {
	TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, width, height, m_SpriteRow, currentframe, flip);
}

void Animation::Reset() {
	m_CurrentFrame = 0;
}