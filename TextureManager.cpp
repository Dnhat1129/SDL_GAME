#include "TextureManager.h"
#include "Engine.h"
#include "Camera.h"

TextureManager* TextureManager::s_Instance = nullptr;

bool TextureManager::Load(std::string id, std::string filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);

    m_TextureMap[id] = texture;
    return true;
}

void TextureManager::Draw(std::string id, int x, int y, int width, int heigt, SDL_RendererFlip flip) {
    SDL_Rect srcRect = { 0, 0, width, heigt };
    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = { x-cam.X, y-cam.Y, width, heigt };
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}


void TextureManager::DrawFrame(std::string id, int x, int y, int width, int heigt, int row, int frame, SDL_RendererFlip flip) {
    SDL_Rect srcRect = { width * frame, heigt * (row - 1) , width, heigt };
    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = { x-cam.X, y-cam.Y, width, heigt };
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip) {
    SDL_Rect srcRect = { tileSize * frame, tileSize * row , tileSize, tileSize };
    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = { x - cam.X, y - cam.Y, tileSize, tileSize };
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[tilesetID], &srcRect, &dstRect, 0, 0, flip);
}

void TextureManager::Drop(std::string id) {
    SDL_DestroyTexture(m_TextureMap[id]);
    m_TextureMap.erase(id);
}

void TextureManager::Clean() {
    std::map<std::string, SDL_Texture*>::iterator it;
    for (it = m_TextureMap.begin(); it != m_TextureMap.end(); it++) {
        SDL_DestroyTexture(it->second);
    }
    m_TextureMap.clear();
    SDL_Log("texture map cleanned!");
}
