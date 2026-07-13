#ifndef GROUND_H
#define GROUND_H

#include "Object.h"

class Ground : public Object {
public:
    // size: 网格半边长，step: 网格间距
    Ground(float size, float step);
};

#endif // GROUND_H
