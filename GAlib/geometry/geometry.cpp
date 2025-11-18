#include "geometry.hpp"
#include <cmath>
#include <limits>

double Geometry::pointSegmentDist(double px, double py,
                                  double ax, double ay,
                                  double bx, double by)
{
    double vx = bx - ax;
    double vy = by - ay;
    double wx = px - ax;
    double wy = py - ay;
    double c1 = vx * wx + vy * wy;
    if (c1 <= 0.0)
    {
        return hypot(px - ax, py - ay);
    }
    double c2 = vx * vx + vy * vy;
    if (c2 <= c1)
    {
        return hypot(px - bx, py - by);
    }
    double t = c1 / c2;
    double projx = ax + t * vx;
    double projy = ay + t * vy;
    return hypot(px - projx, py - projy);
}

double Geometry::minDistanceToPolygonBoundary(
    double cx, double cy,
    const std::vector<std::pair<double, double>> &polygon)
{
    size_t n = polygon.size();
    if (n < 2)
        return 0.0;

    double minD = std::numeric_limits<double>::infinity();
    for (size_t i = 0; i < n; ++i)
    {
        size_t j = (i + 1) % n;
        double d = pointSegmentDist(cx, cy,
                                    polygon[i].first, polygon[i].second,
                                    polygon[j].first, polygon[j].second);
        if (d < minD)
            minD = d;
    }
    return minD;
}