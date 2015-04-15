#include "UrbanFractionField.h"
#include "UrbanDatabase.h"

UrbanFractionField::
UrbanFractionField() {

}

UrbanFractionField::
~UrbanFractionField() {

}

void UrbanFractionField::
init(int numLon, int numLat,
     double startLon, double startLat,
     double dlon, double dlat) {
    // Initialize grid coordinate arrays.
    lon.set_size(numLon);
    lat.set_size(numLat);
    for (int i = 0; i < numLon; ++i) {
        lon[i] = startLon+i*dlon;
    }
    for (int j = 0; j < numLat; ++j) {
        lat[j] = startLat+j*dlat;
    }
    // Initialize data.
    _data.set_size(numLon, numLat);
    for (int j = 0; j < numLat; ++j) {
        for (int i = 0; i < numLon; ++i) {
            double lat0 = lat[j]-0.5*dlat;
            double lat1 = lat[j]+0.5*dlat;
            _data(i, j).cellArea = RADIUS*dlon*(sin(lat1*RAD)-sin(lat0*RAD));
        }
    }
    // Initialize search tree and related objects.
    cellCoords.reshape(3, numLon*numLat);
    int l = 0;
    for (int j = 0; j < numLat; ++j) {
        for (int i = 0; i < numLon; ++i) {
            calcCartesianCoord(lon[i], lat[j],
                               cellCoords(0, l),
                               cellCoords(1, l),
                               cellCoords(2, l));
            l++;
        }
    }
    cellTree = new Tree(cellCoords, cellCoordsMap);
} // init

void UrbanFractionField::
create(const UrbanDatabase &database) {
    std::cout << "[Notice]: Create urban fraction field." << std::endl;
    for (int i = 0; i < database.size(); ++i) {
        Searcher a(cellTree, NULL, cellCoords, database.data(i).x, true);
        arma::Mat<size_t> neighbors;
        arma::mat distances;
        a.Search(1, neighbors, distances);
        UrbanFraction &data = _data(cellCoordsMap[neighbors(0, 0)]);
        data.increase();
        data.heights.tail(1) = database.data(i).floor*3;
        data.areas.tail(1) = database.data(i).area;
    }
    for (int i = 0; i < _data.size(); ++i) {
        UrbanFraction &data = _data(i);
        if (data.size() == 0) continue;
        double sumArea = arma::sum(data.areas);
        data.heightMean = arma::mean(data.heights);
        data.heightStd  = arma::stddev(data.heights);
        data.heightAW   = arma::dot(data.heights, data.areas)/sumArea;
        data.areaFrac = sumArea/data.cellArea;

    }
} // create

void UrbanFractionField::
outputInGeogrid() const {
    float *buffer = new float[_data.size()];
    int nx = lon.size();
    int ny = lat.size();
    int nz = 1;
    int isigned = 0;
    int endian = 0;
    float scalefactor = 1;
    int wordsize = 1;
    for (int i = 0; i < _data.size(); ++i) {
        const UrbanFraction &data = _data(i);
        buffer[i] = data.heightMean;
    }
    write_geogrid(buffer, &nx, &ny, &nz, &isigned, &endian, &scalefactor, &wordsize);
    delete [] buffer;
} // outputInGeogrid

void UrbanFractionField::
outputInNetcdf() const {
    std::cout << "[Notice]: Output urban fraction field." << std::endl;
    NcFile file("uff.nc", NcFile::replace);
    NcDim lonDim = file.addDim("lon", lon.size());
    NcVar lonVar = file.addVar("lon", ncDouble, lonDim);
    lonVar.putVar(lon.memptr());
    lonVar.putAtt("long_name", ncChar, size_t(9), "longitude");
    lonVar.putAtt("units", ncChar, size_t(11), "degree_east");
    NcDim latDim = file.addDim("lat", lat.size());
    NcVar latVar = file.addVar("lat", ncDouble, latDim);
    latVar.putVar(lat.memptr());
    latVar.putAtt("long_name", ncChar, size_t(8), "latitude");
    latVar.putAtt("units", ncChar, size_t(12), "degree_north");
    std::vector<NcDim> dims2d;
    dims2d.push_back(latDim); dims2d.push_back(lonDim);
    NcVar var = file.addVar("h0", ncDouble, dims2d);

    float *buffer = new float[_data.size()];
    for (int i = 0; i < _data.size(); ++i) {
        const UrbanFraction &data = _data(i);
        buffer[i] = data.heightMean;
    }
    var.putVar(buffer);
    delete [] buffer;
}