#include "UrbanDatabase.h"

void UrbanDatabase::
read(const std::string &filePath) {
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
    urbanData.resize(layer->GetFeatureCount());
    OGRFeature *feature;
    int i = 0;
    const int numMaxPoint = 100;
    OGRRawPoint *points = new OGRRawPoint[numMaxPoint];
    while ((feature = layer->GetNextFeature()) != NULL) {
        // Record urban data.
        urbanData[i].floor = feature->GetFieldAsInteger(FLOOR);
        urbanData[i].area = feature->GetFieldAsDouble(SHAPE_AREA);
        // Calculate centroid.
        OGRGeometry *geometry = feature->GetGeometryRef();
        if (geometry == NULL) {
            std::cout << "[Error]: Failed to get geometry!" << std::endl;
        }
        if (geometry->getGeometryType() == wkbPolygon) {
            OGRPolygon *polygon = static_cast<OGRPolygon*>(geometry);
            OGRLinearRing *ring = polygon->getExteriorRing();
            if (ring->getNumPoints() > numMaxPoint) {
                std::cout << "[Error]: Exceed maximum number!" << std::endl;
                exit(1);
            }
            ring->getPoints(points, NULL);
            for (int j = 0; j < ring->getNumPoints(); ++j) {
                urbanData[i].clon += points[j].x;
                urbanData[i].clat += points[j].y;
            }
            urbanData[i].clon /= ring->getNumPoints();
            urbanData[i].clat /= ring->getNumPoints();
        } else if (geometry->getGeometryType() == wkbMultiPolygon) {

        } else {
            std::cout << "[Error]: Unhandled geometry type " << geometry->getGeometryType() << "!" << std::endl;
        }
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