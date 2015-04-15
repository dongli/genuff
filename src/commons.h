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

const double PI = M_PI;
const double RAD = PI/180.0;

static void
calcCartesianCoord(double lon, double lat,
                   double x, double y, double z) {
    const double radius = 1;
    double cosLon = cos(lon*RAD);
    double sinLon = sin(lon*RAD);
    double cosLat = cos(lat*RAD);
    double sinLat = sin(lat*RAD);
    x = radius*cosLat*cosLon;
    y = radius*cosLat*sinLon;
    z = radius*sinLat;
}

#endif // __Commons__