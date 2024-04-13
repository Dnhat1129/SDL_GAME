#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class FontManager {
public:
    FontManager(const std::string& fontPath, int fontSize);
    ~FontManager();

    bool loadFont();
    void closeFont();
    bool isFontLoaded() const;
    SDL_Texture* renderText(const std::string& text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer); 
    TTF_Font* getFont() const {
        return m_font;
    }
private:
    std::string m_fontPath;
    int m_fontSize;
    TTF_Font* m_font = nullptr;
};

#endif // FONTMANAGER_H
