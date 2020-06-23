#include <iostream>
#include "utils.h"
#include "individual.h"
#include "1+1-ES.h"
#include "mu+lambda-ES.h"
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
    int vehicles = 35;

    //read_file_cordeau(file, depots, vehicles, customers, coordinate_matrix);
    read_file_ruiz(file, depots, customers, coordinate_matrix);

    cout << "DATI INIZIALI: " << depots << " " << vehicles << " " << customers << "\n";
    //print_matrix(coordinate_matrix, depots + customers, 2);

    //matrice che contiene le distanze da ogni nodo ad ogni altro nodo
    double **distance_matrix = get_distance_matrix(depots, customers, coordinate_matrix);

    int heuristic = argv[1][0] - '0';
    double best = 0;
    switch (heuristic)
    {
    case 0:
        best = ES_1_1(depots, customers, vehicles, distance_matrix, 1);
        for (int i = 0; i < 1600000; i++)
        {
            double result = ES_1_1(depots, customers, vehicles, distance_matrix, 1);
            if (result < best)
            {
                best = result;
                cout << "BEST: " << best << "\n";
            }
        }
        break;
    case 1:
        const int mu = 5;
        const int lambda = 250;
        best = ES_mu_lambda(depots, customers, vehicles, distance_matrix, 1, mu, lambda);
        for (int i = 0; i < 16000; i++)
        {
            double result = ES_mu_lambda(depots, customers, vehicles, distance_matrix, 1, mu, lambda);
            if (result < best)
            {
                best = result;
                cout << "BEST: " << best << "\n";
            }
        }
        break;
    }

    cout << "Finale: " << best << endl;

    return 0;
}