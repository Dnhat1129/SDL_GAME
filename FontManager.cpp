#include "FontManager.h"
#include <iostream>
#include <string>

FontManager::FontManager(const std::string& fontPath, int fontSize)
    : m_fontPath(fontPath), m_fontSize(fontSize) {}

FontManager::~FontManager() {
    closeFont();
}

bool FontManager::loadFont() {
    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    m_font = TTF_OpenFont(m_fontPath.c_str(), m_fontSize);
    if (m_font == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void FontManager::closeFont() {
    if (m_font) {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
}

bool FontManager::isFontLoaded() const {
    return m_font != nullptr;
}

SDL_Texture* FontManager::renderText(const std::string& text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_FreeSurface(textSurface);
    return textTexture;
}
