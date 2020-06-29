#include <iostream>
#include "utils.h"
#include "individual.h"
#include "1+1-ES.h"
#include "mu+lambda-ES.h"
#include "mu_comma_lambda-ES.h"
#include "GA.h"
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "At least one parameter for choosing the heuristic" << std::endl;
        return 1;
    }

    int depots = 0;
    int customers = 0;
    double **coordinate_matrix;
    //string file = "./dat/10x4-1.txt";
    string file = "./dat/pr10.txt";
    int vehicles = 35;

    //read_file_cordeau(file, depots, vehicles, customers, coordinate_matrix);
    read_file_ruiz(file, depots, customers, coordinate_matrix);

    cout << "DATI INIZIALI: " << depots << " " << vehicles << " " << customers << "\n";
    //print_matrix(coordinate_matrix, depots + customers, 2);

    //matrice che contiene le distanze da ogni nodo ad ogni altro nodo
    double **distance_matrix = get_distance_matrix(depots, customers, coordinate_matrix);

    int heuristic = argv[1][0] - '0';
    double best = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    switch (heuristic)
    {
    case 0:
        best = ES_1_1(depots, customers, vehicles, distance_matrix, 8000000);
        cout << "BEST: " << best << "\n";
        for (int i = 1; i < 1600000; i++)
        {
            //cout << "TENTATIVO: " << i << endl;
            double result = ES_1_1(depots, customers, vehicles, distance_matrix, 8000000);
            if (result < best)
            {
                best = result;
                cout << "BEST: " << best << "\n";
            }
        }
        break;
    case 1:
        best = ES_mu_plus_lambda(depots, customers, vehicles, distance_matrix, 50000, 15, 250);
        break;
    case 2:
        best = ES_mu_comma_lambda(depots, customers, vehicles, distance_matrix, 500000, 15, 300);
        break;
    case 3:
        GA(depots, customers, vehicles, distance_matrix, 500000, 100, 4);
        break;
    case 4:
        //benchmark test
        best = ES_mu_plus_lambda_test(depots, customers, vehicles, distance_matrix, 3000, 15, 300);
        break;
    case 5:
        best = ES_mu_plus_lambda_adaptive(depots, customers, vehicles, distance_matrix, 50000, 15, 250);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    cout << "Finale: " << best << endl;

    Individual a(vehicles, depots, customers, distance_matrix);
    // Individual b(vehicles, depots, customers, distance_matrix);
    // Individual c(vehicles, depots, customers, distance_matrix);
     a.random_inizialize();
    // b.random_inizialize();
    // c.partially_mapped_cross_over(a, b);

    a.print_tour_matrix();
    a.insertion_repeated();
    a.print_tour_matrix();

    return 0;
}