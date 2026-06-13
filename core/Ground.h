#ifndef GROUND_H
#define GROUND_H

#include "Objects.h"

class Ground : public Objects {
public:
    // size: 网格半边长，step: 网格间距
    Ground(float size, float step);
};

#endif // GROUND_H
