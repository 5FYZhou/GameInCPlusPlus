#pragma once

#include "Constants.h"
#include "Scene.h"
#include <windows.h>
#include <iostream>
#include <unordered_map>

class Renderer {
private:
    HANDLE h;
    
    void drawInfo(const ShowMode& mode) const;

public:
    Renderer(HANDLE handle);

    void gotoXY(int x, int y, const ShowMode& mode) const;
    void gotoXY(const Vector2& p, const ShowMode& mode) const;

    void drawScene(const Scene& scene, const ShowMode& mode) const;
    void drawScore(int score, const ShowMode& mode) const;
    void drawPause(const ShowMode& mode) const;
    void hideCursor() const;
};