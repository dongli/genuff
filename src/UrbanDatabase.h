#ifndef __UrbanDatabase__
#define __UrbanDatabase__

#include "commons.h"
#include "UrbanData.h"

class UrbanDatabase {
    std::vector<UrbanData> _data;
public:
    UrbanDatabase() {}
    ~UrbanDatabase() {}

    void
    read(const std::string &filePath);

    int
    size() const {
        return _data.size();
    }

    const UrbanData&
    data(int i) const {
        return _data[i];
    }
}; // UrbanDatabase

#endif // __UrbanDatabase__