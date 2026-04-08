#pragma once

#include "Constants.h"
#include <windows.h>
#include <iostream>
#include <unordered_map>

class Renderer {
private:
    HANDLE h;
    ShowMode curShowMode;

public:
    Renderer(HANDLE handle);

    void setCurShowMode(ShowMode& mode) { curShowMode = mode; }

    void gotoXY(int x, int y) const;
    void gotoXY(const Vector2& p) const;

    void drawScene(const CodeType scene[SCENE_HEIGHT][SCENE_WIDTH]) const;
    void drawScore(int score) const;
    void drawInfo() const;
    void drawPause() const;
    void hideCursor() const;
};