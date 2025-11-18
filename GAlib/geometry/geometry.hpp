#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <utility>

class Geometry
{
public:
    static double pointSegmentDist(double px, double py,
                                   double ax, double ay,
                                   double bx, double by);

    static double minDistanceToPolygonBoundary(
        double cx, double cy,
        const std::vector<std::pair<double, double>> &polygon);
};

#endif // GEOMETRY_H