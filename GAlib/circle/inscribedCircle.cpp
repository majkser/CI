#include "inscribedCircle.hpp"
#include "../geometry/geometry.hpp"
#include <cmath>

std::vector<std::pair<double, double>> InscribedCircle::polygon_;
void InscribedCircle::setPolygon(
    const std::vector<std::pair<double, double>> &poly)
{
    polygon_ = poly;
}

float InscribedCircle::fitness(GAGenome &g)
{
    GABin2DecGenome &gen = (GABin2DecGenome &)g;
    double cx = gen.phenotype(0);
    double cy = gen.phenotype(1);
    double r = gen.phenotype(2);

    if (!(isfinite(cx) && isfinite(cy) && isfinite(r)))
        return 1e-8f;

    double minD = Geometry::minDistanceToPolygonBoundary(cx, cy, polygon_);

    if (r <= minD)
    {
        return float(1.0 + r);
    }
    else
    {
        double violation = r - minD;
        return float(1e-6 / (1.0 + violation));
    }
}