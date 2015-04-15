#ifndef __UrbanFraction__
#define __UrbanFraction__

class UrbanFraction {
public:
    double cellArea;

    arma::vec heights;
    arma::vec areas;

    double heightMean;      // mean building height
    double heightStd;       // building height standard deviation
    double heightAW;        // plan-area-weighted mean building height
    arma::vec heightHist;   // building height histogram
    double areaFrac;        // plan area fraction

    UrbanFraction() {
        heightMean = 0;
        heightStd = 0;
        heightHist = arma::zeros(10);
    }

    void
    increase() {
        int n = heights.size()+1;
        heights.resize(n+1);
        areas.resize(n+1);
    }

    int
    size() const {
        return heights.size();
    }
}; // UrbanFraction

#endif // __UrbanFraction__