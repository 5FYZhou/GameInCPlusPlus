#pragma once

#include "Constants.h"
#include <fstream>

class SaveSystem {
public:
    static bool Save(const GameData& data, const std::string& filePath = SAVE_FILE_NAME) {
        //打开文件
        std::ofstream out(filePath);
        if (!out.is_open()) {
            return false;
        }
        //写入数据
        out << static_cast<int>(data.mode) << "\n"; //保存显示模式
        out << data.snakeBody.size() << "\n"; //保存蛇的长度
        for (const auto& v : data.snakeBody) { //保存蛇的身体坐标
            out << v.x << " " << v.y << "\n";
        }
        out << data.foodPos.x << " " << data.foodPos.y << "\n"; //保存食物坐标
        out << static_cast<int>(data.dir) << "\n"; //保存蛇的移动方向
        out << data.score << "\n"; //保存分数
        return true;
    }

    static bool Load(GameData& data, const std::string& filePath = SAVE_FILE_NAME) {
        //打开文件
        std::ifstream in(filePath);
        if (!in.is_open()) {
            return false;
        }
        //读取数据
        int modeInt;
        if (!(in >> modeInt)) {
            return false;
        }
        data.mode = static_cast<ShowMode>(modeInt); //读取显示模式

        int snakeSize; //读取蛇的长度
        if (!(in >> snakeSize)) {
            return false;
        }
        data.snakeBody.clear();
        for (int i = 0; i < snakeSize; i++) { //读取蛇的身体坐标
            int x, y;
            if (!(in >> x >> y)) {
                return false;
            }
            data.snakeBody.emplace_back(x, y);
        }

        int foodX, foodY; //读取食物坐标
        if (!(in >> foodX >> foodY)) {
            return false;
        }
        data.foodPos = {foodX, foodY};

        int dirInt; //读取蛇的移动方向
        if (!(in >> dirInt)) {
            return false;
        }
        data.dir = static_cast<Direction>(dirInt);

        if (!(in >> data.score)) { //读取分数
            return false;
        }

        return true;
    }

    static bool HasSaveData(const std::string& filePath = SAVE_FILE_NAME) {
        std::ifstream in(filePath);
        return in && in.peek() != std::ifstream::traits_type::eof();
    }

    static void ClearSaveData(const std::string& filePath = SAVE_FILE_NAME) {
        std::ofstream out(filePath, std::ios::trunc);
        out.close();
    }
};
