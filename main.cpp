#include <iostream>
#include "utils.h"
#include "individual.h"
#include "1+1-ES.h"
#include "mu+lambda-ES.h"
#include "mu_comma_lambda-ES.h"
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
    string file = "./dat/pr10.txt";
    int vehicles = 5;

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
        best = ES_1_1(depots, customers, vehicles, distance_matrix, 80000);
        cout << "BEST: " << best << "\n";
        for (int i = 1; i < 1600000; i++)
        {
            //cout << "TENTATIVO: " << i << endl;
            double result = ES_1_1(depots, customers, vehicles, distance_matrix, 80000);
            if (result < best)
            {
                best = result;
                cout << "BEST: " << best << "\n";
            }
        }
        break;
    case 1:
        best = ES_mu_plus_lambda(depots, customers, vehicles, distance_matrix, 5000, 15, 100);
        cout << "BEST: " << best << "\n";
        for (int i = 1; i < 16000; i++)
        {
            //cout << "TENTATIVO: " << i << endl;
            double result = ES_mu_plus_lambda(depots, customers, vehicles, distance_matrix, 5000, 15, 100);
            if (result < best)
            {
                best = result;
                cout << "BEST: " << best << "\n";
            }
        }
        break;
    case 2:
        best = ES_mu_comma_lambda(depots, customers, vehicles, distance_matrix, 5000, 15, 200);
        cout << "BEST: " << best << "\n";
        for (int i = 1; i < 16000; i++)
        {
            //cout << "TENTATIVO: " << i << endl;
            double result = ES_mu_comma_lambda(depots, customers, vehicles, distance_matrix, 5000, 15, 200);
            if (result < best)
            {
                best = result;
                cout << "BEST: " << best << "\n";
            }
        }
        break;
    case 3:
        //TODO GA
        break;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    cout << "Finale: " << best << endl;

    return 0;
}