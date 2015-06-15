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
        if (data.heights.size() > 1) {
            data.heightHist = arma::histc(data.heights, HEIGHT_HIST_BINS);
        } else {
            data.heightHist = arma::trans(arma::histc(data.heights, HEIGHT_HIST_BINS));
        }
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
    float *buffer;
    std::cout << "[Notice]: Output urban fraction field." << std::endl;
    NcFile file("uff.nc", NcFile::replace);
    // Dimensions.
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
    NcDim hhistDim = file.addDim("hhist", HEIGHT_HIST_NUM);
    NcVar hhistVar = file.addVar("hhist", ncDouble, hhistDim);
    hhistVar.putVar(HEIGHT_HIST_BINS.memptr());
    hhistVar.putAtt("long_name", ncChar, size_t(20), "height histogram bin");
    hhistVar.putAtt("units", ncChar, size_t(1), "m");
    // 2D variables.
    std::vector<NcDim> dims2d(2);
    dims2d[0] = latDim;
    dims2d[1] = lonDim;
    NcVar h0Var = file.addVar("h0", ncFloat, dims2d);
    h0Var.putAtt("long_name", ncChar, size_t(20), "mean building height");
    h0Var.putAtt("units", ncChar, size_t(1), "m");
    NcVar h1Var = file.addVar("h1", ncFloat, dims2d);
    h1Var.putAtt("long_name", ncChar, size_t(34), "building height standard deviation");
    h1Var.putAtt("units", ncChar, size_t(1), "m");
    NcVar h2Var = file.addVar("h2", ncFloat, dims2d);
    h2Var.putAtt("long_name", ncChar, size_t(39), "plan-area-weighted mean building height");
    h2Var.putAtt("units", ncChar, size_t(1), "m");
    NcVar a0Var = file.addVar("a0", ncFloat, dims2d);
    a0Var.putAtt("long_name", ncChar, size_t(18), "plan area fraction");
    a0Var.putAtt("units", ncChar, size_t(2), "m2");

    buffer = new float[_data.size()];
    for (int i = 0; i < _data.size(); ++i) {
        const UrbanFraction &data = _data(i);
        buffer[i] = data.heightMean;
    }
    h1Var.putVar(buffer);
    for (int i = 0; i < _data.size(); ++i) {
        const UrbanFraction &data = _data(i);
        buffer[i] = data.heightStd;
    }
    h2Var.putVar(buffer);
    for (int i = 0; i < _data.size(); ++i) {
        const UrbanFraction &data = _data(i);
        buffer[i] = data.heightAW;
    }
    h2Var.putVar(buffer);
    for (int i = 0; i < _data.size(); ++i) {
        const UrbanFraction &data = _data(i);
        buffer[i] = data.areaFrac;
    }
    a0Var.putVar(buffer);

    delete [] buffer;
    // 3D variables.
    std::vector<NcDim> dims3d(3);
    dims3d[0] = latDim;
    dims3d[1] = lonDim;
    dims3d[2] = hhistDim;
    NcVar h3Var = file.addVar("h3", ncFloat, dims3d);
    h3Var.putAtt("long_name", ncChar, size_t(16), "height histogram");
    h3Var.putAtt("units", ncChar, size_t(1), "1");
    buffer = new float[_data.size()*HEIGHT_HIST_NUM];
    int k = 0;
    for (int i = 0; i < _data.size(); ++i) {
        const UrbanFraction &data = _data(i);
        for (int j = 0; j < HEIGHT_HIST_NUM; ++j) {
            buffer[k++] = data.heightHist[j];
        }
    }
    h3Var.putVar(buffer);

    delete [] buffer;
} // outputInNetcdf