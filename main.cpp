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

    std::cout << "DATI INIZIALI: " << depots << " " << vehicles << " " << customers << "\n";
    //print_matrix(coordinate_matrix, depots + customers, 2);

    //matrice che contiene le distanze da ogni nodo ad ogni altro nodo
    double **distance_matrix = get_distance_matrix(depots, customers, coordinate_matrix);

    int heuristic = argv[1][0] - '0';
    double best = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    switch (heuristic)
    {
    case 0:
        // best = ES_1_1(depots, customers, vehicles, distance_matrix, 8000000);
        // cout << "BEST: " << best << "\n";
        // for (int i = 1; i < 1600000; i++)
        // {
        //     //cout << "TENTATIVO: " << i << endl;
        //     double result = ES_1_1(depots, customers, vehicles, distance_matrix, 8000000);
        //     if (result < best)
        //     {
        //         best = result;
        //         cout << "BEST: " << best << "\n";
        //     }
        // }
        break;
    case 1:
        //best = ES_mu_plus_lambda(depots, customers, vehicles, distance_matrix, 50000, 15, 250);
        break;
    case 2:
        //best = ES_mu_comma_lambda(depots, customers, vehicles, distance_matrix, 500000, 15, 300);
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
        break;
    case 6:
        std::cout<<"(mu+lambda)-ES\n";
        for (unsigned mut = 0; mut < 5; mut++)
        {
            for (unsigned cross = 0; cross < 6; cross++)
            {
                best = ES_mu_plus_lambda(depots, customers, vehicles, distance_matrix, 700000, 15, 250, mut, cross);
                string m;
                switch (mut)
                {
                case 0:
                    m = "swap2";
                    break;
                case 1:
                    m = "swap3";
                    break;
                case 2:
                    m = "scrumble";
                    break;
                case 3:
                    m = "inversion";
                    break;
                case 4:
                    m = "insertion";
                    break;
                }
                string c;
                switch (cross)
                {
                case 0:
                    c = "one_point_cross_over";
                    break;
                case 1:
                    c = "two_point_cross_over";
                    break;
                case 2:
                    c = "best_order_cross_over";
                    break;
                case 3:
                    c = "position_based_cross_over";
                    break;
                case 4:
                    c = "uniform_cross_over";
                    break;
                case 5:
                    c = "no_cross_over";
                    break;
                }
                std::cout << c << ":" << m << ":" << best << "\n";
            }
        }
    case 7:
        std::cout<<"(mu,lambda)-ES\n";
        for (unsigned mut = 0; mut < 5; mut++)
        {
            for (unsigned cross = 0; cross < 6; cross++)
            {
                best = ES_mu_comma_lambda(depots, customers, vehicles, distance_matrix, 700000, 15, 250, mut, cross);
                string m;
                switch (mut)
                {
                case 0:
                    m = "swap2";
                    break;
                case 1:
                    m = "swap3";
                    break;
                case 2:
                    m = "scrumble";
                    break;
                case 3:
                    m = "inversion";
                    break;
                case 4:
                    m = "insertion";
                    break;
                }
                string c;
                switch (cross)
                {
                case 0:
                    c = "one_point_cross_over";
                    break;
                case 1:
                    c = "two_point_cross_over";
                    break;
                case 2:
                    c = "best_order_cross_over";
                    break;
                case 3:
                    c = "position_based_cross_over";
                    break;
                case 4:
                    c = "uniform_cross_over";
                    break;
                case 5:
                    c = "no_cross_over";
                    break;
                }
                std::cout << c << ":" << m << ":" << best << "\n";
            }
        }
    case 8:
        std::cout<<"(1+1)-ES\n";
        for (unsigned mut = 0; mut < 5; mut++)
        {
            best = ES_1_1(depots, customers, vehicles, distance_matrix, 8750000, mut);
            double global_best = best;
            for (unsigned i = 1; i < 20; i++)
            {
                best = ES_1_1(depots, customers, vehicles, distance_matrix, 8750000, mut);
                if (best < global_best)
                {
                    global_best = best;
                }
            }
            string m;
            switch (mut)
            {
            case 0:
                m = "swap2";
                break;
            case 1:
                m = "swap3";
                break;
            case 2:
                m = "scrumble";
                break;
            case 3:
                m = "inversion";
                break;
            case 4:
                m = "insertion";
                break;
            }
            string c = "no_cross_over";
            std::cout << c << ":" << m << ":" << best << "\n";
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    std::cout << "Finale: " << best << endl;

    return 0;
}