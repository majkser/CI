#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <ga/GABin2DecGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/ga.h>
#include "geometry/geometry.hpp"
#include "circle/inscribedCircle.hpp"

std::vector<std::pair<double, double>> parseInput(int argc, char **argv)
{
    std::vector<double> v;
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            v.push_back(atof(argv[i]));
        }
    }
    else
    {
        double t;
        while (std::cin >> t)
            v.push_back(t);
    }

    if (!(v.size() == 4 || v.size() == 8))
    {
        throw std::runtime_error(
            "Usage: provide 4 numbers (x1 y1 x2 y2) or 8 numbers (x1 y1 ... x4 y4)");
    }

    std::vector<std::pair<double, double>> polygon;
    if (v.size() == 4)
    {
        double x1 = v[0], y1 = v[1], x2 = v[2], y2 = v[3];
        polygon.push_back({x1, y1});
        polygon.push_back({x1, y2});
        polygon.push_back({x2, y2});
        polygon.push_back({x2, y1});
    }
    else
    {
        for (int i = 0; i < 4; ++i)
            polygon.push_back({v[2 * i], v[2 * i + 1]});
    }
    return polygon;
}

int main(int argc, char **argv)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    try
    {
        auto polygon = parseInput(argc, argv);
        InscribedCircle::setPolygon(polygon);

        double minx, maxx, miny, maxy;
        Geometry::boundingBox(polygon, minx, maxx, miny, maxy);

        double cx_min = minx, cx_max = maxx;
        double cy_min = miny, cy_max = maxy;
        double diag = hypot(maxx - minx, maxy - miny);
        double r_min = 0.0, r_max = diag / 2.0;

        GABin2DecPhenotype ph;
        const int bits = 28;
        ph.add(bits, cx_min, cx_max);
        ph.add(bits, cy_min, cy_max);
        ph.add(bits, r_min, r_max);

        GABin2DecGenome genome(ph, InscribedCircle::fitness);

        GASimpleGA ga(genome);
        ga.populationSize(200);
        ga.pCrossover(0.8);
        ga.pMutation(0.02);
        ga.nGenerations(400);
        ga.evolve();

        GABin2DecGenome &best = (GABin2DecGenome &)ga.statistics().bestIndividual();
        double best_cx = best.phenotype(0);
        double best_cy = best.phenotype(1);
        double best_r = best.phenotype(2);

        double minD = Geometry::minDistanceToPolygonBoundary(best_cx, best_cy, polygon);
        if (best_r > minD)
            best_r = minD;

        std::cout << std::fixed << std::setprecision(8);
        std::cout << best_cx << " " << best_cy << " " << best_r << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}