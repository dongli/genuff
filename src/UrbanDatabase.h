#ifndef __UrbanDatabase__
#define __UrbanDatabase__

#include "commons.h"
#include "UrbanData.h"

class UrbanDatabase {
    std::vector<UrbanData> urbanData;
public:
    UrbanDatabase() {}
    ~UrbanDatabase() {}

    void
    read(const std::string &filePath);
}; // UrbanDatabase

#endif // __UrbanDatabase__