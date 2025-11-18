#ifndef INSCRIBED_CIRCLE_H
#define INSCRIBED_CIRCLE_H

#include <vector>
#include <utility>
#include <ga/GABin2DecGenome.h>

class InscribedCircle
{
public:
    static void setPolygon(const std::vector<std::pair<double, double>> &poly);
    static float fitness(GAGenome &g);

private:
    static std::vector<std::pair<double, double>> polygon_;
};

#endif // INSCRIBED_CIRCLE_H