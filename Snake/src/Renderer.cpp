#include "Renderer.h"
#include <iomanip>

Renderer::Renderer(HANDLE handle)
    : h(handle), curShowMode(ShowMode::FULL) {
}

void Renderer::gotoXY(int x, int y) const {
    COORD pos;
    if (curShowMode == ShowMode::HALF) {
        pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    } else {
        pos = { static_cast<SHORT>(x * 2), static_cast<SHORT>(y) };
    }
    SetConsoleCursorPosition(h, pos);
}

void Renderer::gotoXY(const Vector2& p) const {
    gotoXY(p.x, p.y);
}

void Renderer::drawScene(const CodeType scene[SCENE_HEIGHT][SCENE_WIDTH]) const {
    const auto& curCodeMap = (curShowMode == ShowMode::HALF) ? codeMapHalf : codeMapFull;
    gotoXY(0, 0);
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        for (int j = 0; j < SCENE_WIDTH; j++) {
            SetConsoleTextAttribute(h, colorMap.at(scene[i][j]));
            std::cout << curCodeMap.at(scene[i][j]);
        }
        std::cout << std::endl;
    }
    drawInfo();
}

void Renderer::drawScore(int score) const {
    SetConsoleTextAttribute(h, colorMap.at(CodeType::TEXT));
    gotoXY(0, SCENE_HEIGHT);
    std::cout << "Score:" << std::setw(5) << score;
}

void Renderer::drawInfo() const {
    for (int i = 0; i < INFO.size(); i++) {
        const auto& [text, code, setoff] = INFO[i];
        SetConsoleTextAttribute(h, colorMap.at(code));
        gotoXY(SCENE_WIDTH + INFOSETOFF + setoff, INFOSETOFF + i);
        std::cout << text;
    }
}

void Renderer::drawPause() const {
    if (curShowMode == ShowMode::HALF) {
        gotoXY(WALL_WIDTH + WIDTH / 2 - 3, WALL_WIDTH + HEIGHT / 2);
    } else {
        gotoXY(WALL_WIDTH + WIDTH / 2 - 1.5, WALL_WIDTH + HEIGHT / 2);
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