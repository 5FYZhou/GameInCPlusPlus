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

    void hideCursor() const;
    void Clean() const { system("cls"); }
    void Out(const std::string& text) const{ std::cout << text; }

    void gotoXY(int x, int y, const ShowMode& mode) const;
    void gotoXY(const Vector2& p, const ShowMode& mode) const;

    void drawScene(const Scene& scene, const ShowMode& mode) const;
    void drawPause(const ShowMode& mode) const;

    void UpdateGAMEINFO(const std::string& diff, const std::string& score, const ShowMode& mode) const;

    void drawMenu(const ShowMode& mode) const;
    void drawGameOver(const ShowMode& mode) const;
    void drawExit(const ShowMode& mode) const;
};