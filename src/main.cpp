#include "UrbanDatabase.h"
#include "UrbanFraction.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "[Error]: A shapefile path is needed!" << std::endl;
        exit(1);
    }
    UrbanDatabase database;
    database.read(argv[1]);
}
