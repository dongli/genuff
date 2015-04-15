#ifndef __UrbanFractionField__
#define __UrbanFractionField__

#include "commons.h"
#include "UrbanFraction.h"

class UrbanFractionField {
public:
    UrbanFractionField();
    ~UrbanFractionField();

    void
    init(int numLon, int numLat);
};

#endif // __UrbanFractionField__