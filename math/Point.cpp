//
// Created by muyuchen on 2026/3/17.
//

#include "Point.h"

Point::Point()
{
    x=0;
    y=0;
    this->name = '.';
}

Point::Point(int x, int y, char name)
{
    this->x=x;
    this->y=y;
    this->name=name;
}

