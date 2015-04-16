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
    arma::uvec heightHist;  // building height histogram
    double areaFrac;        // plan area fraction

    UrbanFraction() {
        heightMean = 0;
        heightStd = 0;
        heightHist.set_size(HEIGHT_HIST_NUM);
    }

    void
    increase() {
        int n = heights.size()+1;
        heights.resize(n);
        areas.resize(n);
    }

    int
    size() const {
        return heights.size();
    }
}; // UrbanFraction

static const arma::vec HEIGHT_HIST_BINS = arma::linspace<arma::vec>(0, HEIGHT_HIST_MAX, HEIGHT_HIST_NUM);

#endif // __UrbanFraction__