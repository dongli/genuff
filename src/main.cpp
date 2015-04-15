#include "UrbanDatabase.h"
#include "UrbanFractionField.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "[Error]: A shapefile path is needed!" << std::endl;
        exit(1);
    }
    UrbanDatabase database;
    database.read(argv[1]);
    UrbanFractionField field;
    field.init(814, 753, 112.51588060, 35.5477462, 0.01, 0.01);
    field.create(database);
    field.outputInNetcdf();
}
