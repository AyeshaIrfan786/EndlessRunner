#pragma once
#include <string>

class FileHandler {
public:
    static void saveHighScore(int score);
    static int  loadHighScore();
private:
    static const std::string filename;
};