#include "Renderer.h"
#include "Scene.h"
#include <iomanip>

Renderer::Renderer(HANDLE handle)
    : h(handle) {}

void Renderer::hideCursor() const {
    HANDLE handle = h;
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(handle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &cursorInfo);
}

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
    // 根据显示模式选择对应的字符映射表
    const auto& curCodeMap = (mode == ShowMode::HALF) ? codeMapHalf : codeMapFull;
    gotoXY(0, 0, mode);
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        for (int j = 0; j < SCENE_WIDTH; j++) {
            // 设置颜色并输出对应的字符
            SetConsoleTextAttribute(h, colorMap.at(scene.GetScene()[i][j]));
            std::cout << curCodeMap.at(scene.GetScene()[i][j]);
        }
        std::cout << std::endl;
    }
    drawInfo(mode);
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

void Renderer::UpdateGAMEINFO(const std::string& diff, const std::string& score, const ShowMode& mode) const {
    int l = INFO.size();
    for (int i = 0; i < GAMEINFO.size(); i++) {
        const auto& [text, type, code, setoff] = GAMEINFO[i];
        SetConsoleTextAttribute(h, colorMap.at(code));
        gotoXY(SCENE_WIDTH + INFOSETOFF + setoff, INFOSETOFF + i + l, mode);
        std::cout << text;
        if(type == CodeType::DIFFICULTY){
            std::cout << diff << "              ";
        }
        else if (type == CodeType::SCORE){
            std::cout << score << "              ";
        }
    }
}

void Renderer::drawMenu(const ShowMode& mode) const{
    const auto& curCodeMap = (mode == ShowMode::HALF) ? codeMapHalf : codeMapFull;
    gotoXY(0, 0, mode);
    for (int i = 0; i < MENU.size(); i++) {
        const auto& [text, code, setoff] = MENU[i];
        SetConsoleTextAttribute(h, colorMap.at(code));
        gotoXY(INFOSETOFF + setoff, INFOSETOFF + i, mode);
        std::cout << text;
    }
}

void Renderer::drawGameOver(const ShowMode& mode) const {
    if (mode == ShowMode::HALF) {
        gotoXY(WALL_WIDTH + WIDTH / 2 - 4, WALL_WIDTH + HEIGHT / 2, mode);
    } else {
        gotoXY(WALL_WIDTH + WIDTH / 2 - 2, WALL_WIDTH + HEIGHT / 2, mode);
    }
    
    SetConsoleTextAttribute(h, colorMap.at(CodeType::TEXT));
    std::cout << "GameOver";

    gotoXY(0, SCENE_HEIGHT + 1, mode);
    std::cout << "开始新游戏？（Y/n）";
}

void Renderer::drawExit(const ShowMode& mode) const{
    gotoXY(0, SCENE_HEIGHT + 1, mode);
    SetConsoleTextAttribute(h, colorMap.at(CodeType::TEXT));
    std::cout << "游戏结束，感谢游玩！" << std::endl;
    system("pause");
}