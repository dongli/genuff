#ifndef __UrbanData__
#define __UrbanData__


enum UrbanFieldNames {
    FLOOR,      // Type: string
    NUMB,
    IMPORTDATE,
    CODE,       // Type: integer
    GCODE,
    ONLYID,
    SHAPE_LENG, // Type: double
    SHAPE_AREA, // Type: double
    SHAPE_LEN   // Type: double
};

class UrbanData {
public:
    double floor;
    double area;
    double clon;
    double clat;
    arma::vec::fixed<3> x;

    UrbanData() {
        clon = 0;
        clat = 0;
    }
}; // UrbanData

#endif // __UrbanData__