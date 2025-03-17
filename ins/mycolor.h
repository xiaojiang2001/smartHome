#ifndef MYCOLOR_H
#define MYCOLOR_H

#include <QStringList>


class Mycolor
{
public:
    static int myTheme;
    static QStringList blackColorList;
    static QStringList blueColorList;
    static QStringList greenColorList;
    static QStringList pinkColorList;

    static void setThemeIndex(int);
    static void setColorList();
};



#endif


//mycolor.cpp


