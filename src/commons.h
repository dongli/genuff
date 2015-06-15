#ifndef __Commons__
#define __Commons__

#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include <vector>
#include <gdal.h>
#include <ogr_api.h>
#include <gdal_priv.h>
#include <cpl_conv.h>
#include <ogrsf_frmts.h>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <netcdf>

using namespace netCDF;

using boost::xpressive::sregex;
using boost::xpressive::smatch;
using boost::xpressive::regex_search;

// Shortcuts for MLPACK classes.
typedef mlpack::neighbor::NearestNeighborSort SortPolicy;
typedef mlpack::neighbor::NeighborSearchStat<SortPolicy> SearchStat;
typedef mlpack::tree::BinarySpaceTree<mlpack::bound::HRectBound<2>, SearchStat> Tree;
typedef mlpack::metric::EuclideanDistance Metric;
typedef mlpack::neighbor::NeighborSearch<SortPolicy, Metric, Tree> Searcher;

const double PI = M_PI;
const double RAD = PI/180.0;
const double HEIGHT_HIST_MAX = 75;
const int HEIGHT_HIST_NUM = HEIGHT_HIST_MAX/5+1;
const double RADIUS = 6.371e6;

static void
calcCartesianCoord(double lon, double lat,
                   double &x, double &y, double &z) {
    const double radius = 1;
    double cosLon = cos(lon*RAD);
    double sinLon = sin(lon*RAD);
    double cosLat = cos(lat*RAD);
    double sinLat = sin(lat*RAD);
    x = radius*cosLat*cosLon;
    y = radius*cosLat*sinLon;
    z = radius*sinLat;
}

extern "C" int write_geogrid(
        float * rarray,          /* The array to be written */
        int * nx,                /* x-dimension of the array */
        int * ny,                /* y-dimension of the array */
        int * nz,                /* z-dimension of the array */
        int * isigned,           /* 0=unsigned data, 1=signed data */
        int * endian,            /* 0=big endian, 1=little endian */
        float * scalefactor,     /* value to divide array elements by before truncation to integers */
        int * wordsize);         /* number of bytes to use for each array element */

#endif // __Commons__