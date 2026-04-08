#include "Renderer.h"
#include "Scene.h"
#include <iomanip>

Renderer::Renderer(HANDLE handle)
    : h(handle) {}

void Renderer::gotoXY(int x, int y, const ShowMode& mode) const {
    COORD pos;
    if (mode == ShowMode::HALF) {
        pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    } else {
        pos = { static_cast<SHORT>(x * 2), static_cast<SHORT>(y) };
    }
    SetConsoleCursorPosition(h, pos);
}

void Renderer::gotoXY(const Vector2& p, const ShowMode& mode) const {
    gotoXY(p.x, p.y, mode);
}

void Renderer::drawScene(const Scene& scene, const ShowMode& mode) const {
    const auto& curCodeMap = (mode == ShowMode::HALF) ? codeMapHalf : codeMapFull;
    gotoXY(0, 0, mode);
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        for (int j = 0; j < SCENE_WIDTH; j++) {
            SetConsoleTextAttribute(h, colorMap.at(scene.GetScene()[i][j]));
            std::cout << curCodeMap.at(scene.GetScene()[i][j]);
        }
        std::cout << std::endl;
    }
    drawInfo(mode);
}

void Renderer::drawScore(int score, const ShowMode& mode) const {
    SetConsoleTextAttribute(h, colorMap.at(CodeType::TEXT));
    gotoXY(0, SCENE_HEIGHT, mode);
    std::cout << "Score:" << std::setw(5) << score;
}

void Renderer::drawInfo(const ShowMode& mode) const {
    for (int i = 0; i < INFO.size(); i++) {
        const auto& [text, code, setoff] = INFO[i];
        SetConsoleTextAttribute(h, colorMap.at(code));
        gotoXY(SCENE_WIDTH + INFOSETOFF + setoff, INFOSETOFF + i, mode);
        std::cout << text;
    }
}

void Renderer::drawPause(const ShowMode& mode) const {
    if (mode == ShowMode::HALF) {
        gotoXY(WALL_WIDTH + WIDTH / 2 - 3, WALL_WIDTH + HEIGHT / 2, mode);
    } else {
        gotoXY(WALL_WIDTH + WIDTH / 2 - 1.5, WALL_WIDTH + HEIGHT / 2, mode);
    }
    SetConsoleTextAttribute(h, colorMap.at(CodeType::TEXT));
    std::cout << "Paused";
}

void Renderer::hideCursor() const {
    HANDLE handle = h;
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(handle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &cursorInfo);
}