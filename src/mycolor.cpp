#include "mycolor.h"

int Mycolor::myTheme = 0;
QStringList Mycolor::blackColorList = {0};
QStringList Mycolor::blueColorList = {0};
QStringList Mycolor::greenColorList = {0};
QStringList Mycolor::pinkColorList = {0};

void Mycolor::setThemeIndex(int theme)
{
    Mycolor::myTheme =  theme;
}

void Mycolor::setColorList()
{
    Mycolor::blackColorList.append("background-color: rgb(44, 46, 67);");
    Mycolor::blackColorList.append("background-color: rgb(23, 27, 34);");
    Mycolor::blackColorList.append("background-color: rgb(34, 38, 46);");
    Mycolor::blackColorList.append("background-color: rgb(44, 48, 56);");

    Mycolor::blueColorList.append("background-color: rgb(41, 85, 126);");
    Mycolor::blueColorList.append("background-color: rgb(10, 55, 101);");
    Mycolor::blueColorList.append("background-color: rgb(42, 90, 133);");
    Mycolor::blueColorList.append("background-color: rgb(41, 85, 126);");

    Mycolor::greenColorList.append("background-color: rgb(53, 115, 108);");
    Mycolor::greenColorList.append("background-color: rgb(0, 44, 45);");
    Mycolor::greenColorList.append("background-color: rgb(37, 98, 91);");
    Mycolor::greenColorList.append("background-color: rgb(31, 88, 85);");

    Mycolor::pinkColorList.append("background-color: rgb(255, 140, 162);");
    Mycolor::pinkColorList.append("background-color: rgb(255, 255, 255);");
    Mycolor::pinkColorList.append("background-color: rgb(245, 126, 166);");
    Mycolor::pinkColorList.append("background-color: rgb(255, 140, 162);");

}
