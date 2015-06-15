#ifndef __UrbanFractionField__
#define __UrbanFractionField__

#include "commons.h"
#include "UrbanFraction.h"

class UrbanDatabase;

class UrbanFractionField {
    arma::vec lon, lat;
    arma::field<UrbanFraction> _data;
    Tree *cellTree;
    arma::mat cellCoords;
    std::vector<size_t> cellCoordsMap;
public:
    UrbanFractionField();
    ~UrbanFractionField();

    void
    init(int numLon, int numLat,
         double startLon, double startLat,
         double dlon, double dlat);

    void
    create(const UrbanDatabase &database);

    void
    outputInGeogrid() const;

    void
    outputInNetcdf() const;
}; // UrbanFractionField

#endif // __UrbanFractionField__