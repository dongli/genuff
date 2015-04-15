#include "UrbanDatabase.h"

void UrbanDatabase::
read(const std::string &filePath) {
    std::cout << "[Notice]: Read urban database from \"" << filePath << "\"." << std::endl;
    GDALAllRegister();
    OGRRegisterAll();
    OGRSFDriver *driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
    OGRDataSource *dataset = driver->Open(filePath.c_str(), false);
    if (dataset == NULL) {
        std::cout << "[Error]: Failed to open " << filePath << "!" << std::endl;
        exit(1);
    }
    OGRLayer *layer = dataset->GetLayer(0);
    layer->ResetReading();
    _data.resize(layer->GetFeatureCount());
    OGRFeature *feature;
    int i = 0;
    const int numMaxPoint = 10000;
    OGRRawPoint *points = new OGRRawPoint[numMaxPoint];
    while ((feature = layer->GetNextFeature()) != NULL) {
        // Record urban data.
        _data[i].floor = feature->GetFieldAsInteger(FLOOR);
        _data[i].area = feature->GetFieldAsDouble(SHAPE_AREA);
        // Calculate centroid.
        OGRGeometry *geometry = feature->GetGeometryRef();
        if (geometry == NULL) {
            std::cout << "[Error]: Failed to get geometry!" << std::endl;
        }
        int numPoint = 0;
        if (geometry->getGeometryType() == wkbPolygon) {
            OGRPolygon *polygon = static_cast<OGRPolygon*>(geometry);
            OGRLinearRing *ring = polygon->getExteriorRing();
            if (ring->getNumPoints() > numMaxPoint) {
                std::cout << "[Error]: Exceed maximum number (" <<
                    ring->getNumPoints() << ")!" << std::endl;
                exit(1);
            }
            ring->getPoints(points, NULL);
            for (int j = 0; j < ring->getNumPoints(); ++j) {
                _data[i].clon += points[j].x;
                _data[i].clat += points[j].y;
            }
            numPoint = ring->getNumPoints();
        } else if (geometry->getGeometryType() == wkbMultiPolygon) {
            for (int k = 0; k < OGR_G_GetGeometryCount(geometry); ++k) {
                OGRPolygon *polygon = static_cast<OGRPolygon*>(OGR_G_GetGeometryRef(geometry, k));
                OGRLinearRing *ring = polygon->getExteriorRing();
                if (ring->getNumPoints() > numMaxPoint) {
                    std::cout << "[Error]: Exceed maximum number (" <<
                        ring->getNumPoints() << ")!" << std::endl;
                    exit(1);
                }
                ring->getPoints(points, NULL);
                for (int j = 0; j < ring->getNumPoints(); ++j) {
                    _data[i].clon += points[j].x;
                    _data[i].clat += points[j].y;
                }
                numPoint += ring->getNumPoints();
            }
        } else {
            std::cout << "[Error]: Unhandled geometry type " << geometry->getGeometryType() << "!" << std::endl;
        }
        _data[i].clon /= numPoint;
        _data[i].clat /= numPoint;
        calcCartesianCoord(_data[i].clon, _data[i].clat,
                           _data[i].x[0], _data[i].x[1],
                           _data[i].x[2]);
        i++;
    }
    delete [] points;
    OGRDataSource::DestroyDataSource(dataset);
    OGRCleanupAll();
} // read


// OGRFeatureDefn *fields = layer->GetLayerDefn();
// for (int i = 0; i < fields->GetFieldCount(); ++i) {
//     OGRFieldDefn *field = fields->GetFieldDefn(i);
//     std::cout << field->GetNameRef() << ": " << feature->GetFieldAsString(i) << " " << field->GetType() << std::endl;
// }
// return;