#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

// GAlib headers includes
#include <ga/GABin2DecGenome.h>
#include <ga/GASimpleGA.h>
#include <ga/ga.h>
#include <ga/GARealGenome.h>

#include "geometry/geometry.hpp"
#include "circle/inscribedCircle.hpp"

std::vector<std::pair<double, double>> square;
std::vector<std::pair<double, double>> parseInput(int argc, char **argv)
{
    std::vector<double> v;
    if (argc > 1)
    {
        if (argc != 5) // program name + 4 arguments
        {
            throw std::runtime_error(
                "Usage: provide exactly 4 numbers (x1 y1 x2 y2) representing opposite corners of a square");
        }

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

        if (v.size() != 4)
        {
            throw std::runtime_error(
                "Input error: provide exactly 4 numbers (x1 y1 x2 y2) representing opposite corners of a square");
        }
    }

    double x1 = v[0], y1 = v[1], x2 = v[2], y2 = v[3];

    if ((std::abs(x2 - x1) - std::abs(y2 - y1)) > 1e-4)
    {
        throw std::runtime_error("The provided coordinates do not form a square.");
    }

    square.push_back({x1, y1});
    square.push_back({x1, y2});
    square.push_back({x2, y2});
    square.push_back({x2, y1});

    return square;
}

int main(int argc, char **argv)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    try
    {
        auto square = parseInput(argc, argv);
        InscribedCircle::setPolygon(square);

        double x1 = square[0].first, x2 = square[2].first;
        double y1 = square[0].second, y2 = square[2].second;
        double cx_min = std::min(x1, x2), cx_max = std::max(x1, x2);
        double cy_min = std::min(y1, y2), cy_max = std::max(y1, y2);

        double diag = hypot(cx_max - cx_min, cy_max - cy_min);
        double r_min = 0.0, r_max = diag / 2.0;

        // Define three phenotype components: cx, cy, r - each encoded with 28 bits
        GABin2DecPhenotype ph;
        const int bits = 28;
        ph.add(bits, cx_min, cx_max);
        ph.add(bits, cy_min, cy_max);
        ph.add(bits, r_min, r_max);

        // Create genome with the defined phenotype and fitness function
        GABin2DecGenome genome(ph, InscribedCircle::fitness);

        // Set up classical genetic algorithm with parameters and initialized genome
        GASimpleGA ga(genome);
        ga.populationSize(200);
        ga.pCrossover(0.8);
        ga.pMutation(0.02);
        ga.nGenerations(400);
        // ga.evolve(); // evolve whole population

        std::ofstream csv("ga_stats.csv");
        csv << "Generation,BestFitness,AvgFitness,WorstFitness,bestCx,bestCy,bestR\n";

        ga.initialize();
        GABin2DecGenome &best = (GABin2DecGenome &)ga.statistics().bestIndividual();
        csv << 0 << ","
            << ga.statistics().maxEver() << ","
            << ga.statistics().current(GAStatistics::Mean) << ","
            << ga.statistics().current(GAStatistics::Minimum) << ","
            << best.phenotype(0) << "," << best.phenotype(1) << "," << best.phenotype(2) << "\n";

        for (int i = 1; i <= ga.nGenerations(); ++i)
        {
            ga.step(); // make one generation step

            GABin2DecGenome &best_gen = (GABin2DecGenome &)ga.statistics().bestIndividual();

            csv << i << ","
                << ga.statistics().maxEver() << ","
                << ga.statistics().current(GAStatistics::Mean) << ","
                << ga.statistics().current(GAStatistics::Minimum) << ","
                << best_gen.phenotype(0) << ","
                << best_gen.phenotype(1) << ","
                << best_gen.phenotype(2) << "\n";
        }

        csv.close();

        // GABin2DecGenome &best = (GABin2DecGenome &)ga.statistics().bestIndividual();
        double best_cx = best.phenotype(0);
        double best_cy = best.phenotype(1);
        double best_r = best.phenotype(2);
        double minD = Geometry::minDistanceToPolygonBoundary(best_cx, best_cy, square);
        if (best_r > minD)
            best_r = minD;
        std::cout << std::fixed << std::setprecision(8);
        std::cout << best_cx << " " << best_cy << " " << best_r << "\n";

        std::ofstream csvBest("best_circle.csv");
        csvBest << "x0,y0,x1,y1,x2,y2,x3,y3,cx,cy,r\n";
        csvBest << square[0].first << "," << square[0].second << ","
                << square[1].first << "," << square[1].second << ","
                << square[2].first << "," << square[2].second << ","
                << square[3].first << "," << square[3].second << ","
                << best_cx << "," << best_cy << "," << best_r << "\n";
        csvBest.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}