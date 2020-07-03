#include <iostream>
#include <string>
#include "utils.h"
#include "individual.h"
#include "1+1-ES.h"
#include "mu+lambda-ES.h"
#include "mu_comma_lambda-ES.h"
#include "GA.h"

void test_heuristic(const unsigned heuristic, const unsigned vehicles, string prefix, const unsigned start_number_file, const unsigned max_number_file, const double known_solution)
{
    unsigned depots = 0;
    unsigned customers = 0;
    double **coordinate_matrix;
    string dir = "./dat/";
    for (unsigned i = start_number_file; i <= max_number_file; i++)
    {
        string number_instance;
        if (i < 10)
        {
            number_instance = number_instance + "0" + to_string(i);
        }
        else
        {
            number_instance = number_instance + to_string(i);
        }
        string file = dir + prefix + number_instance + ".txt";

        read_file_ruiz(file, depots, customers, coordinate_matrix);
        double **distance_matrix = get_distance_matrix(depots, customers, coordinate_matrix);

        const unsigned max_evaluations = 148000000;
        unsigned tries = 5;

        unsigned lambda = 100;
        unsigned max_g;

        double cost;
        double global_best;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        switch (heuristic)
        {
        case 0:
            tries = 10;

            cost = ES_1_1(depots, customers, vehicles, distance_matrix, max_evaluations / tries, 1, known_solution);
            global_best = cost;
            if ((unsigned)global_best == known_solution)
            {
                break;
            }
            for (unsigned i = 1; i < tries; i++)
            {
                cost = ES_1_1(depots, customers, vehicles, distance_matrix, max_evaluations / tries , 1, known_solution);
                if (cost < global_best)
                {
                    global_best = cost;
                    if ((unsigned)global_best == known_solution)
                    {
                        break;
                    }
                }
            }
            break;
        case 1:
            lambda = 100;
            max_g = (max_evaluations / tries) / lambda;
            cost = ES_mu_plus_lambda(depots, customers, vehicles, distance_matrix, max_g, 15, lambda, 1, 5, known_solution);
            global_best = cost;
            if ((unsigned)global_best == known_solution)
            {
                break;
            }

            for (unsigned i = 1; i < tries; i++)
            {
                cost = ES_mu_plus_lambda(depots, customers, vehicles, distance_matrix, max_g, 15, lambda, 1, 5, known_solution);
                if (cost < global_best)
                {
                    global_best = cost;
                    if ((unsigned)global_best == known_solution)
                    {
                        break;
                    }
                }
            }
            break;
        case 2:
            lambda = 200;
            max_g = (max_evaluations / tries) / lambda;
            cost = ES_mu_comma_lambda(depots, customers, vehicles, distance_matrix, max_g, 15, lambda, 1, 5, known_solution);
            global_best = cost;
            if ((unsigned)global_best == known_solution)
            {
                break;
            }

            for (unsigned i = 1; i < tries; i++)
            {
                cost = ES_mu_comma_lambda(depots, customers, vehicles, distance_matrix, max_g, 15, lambda, 1, 5, known_solution);
                if (cost < global_best)
                {
                    global_best = cost;
                    if ((unsigned)global_best == known_solution)
                    {
                        break;
                    }
                }
            }
            break;
        case 3:
            tries = 1;
            lambda = 100;
            max_g = (max_evaluations / tries) / lambda;
            cost = GA(depots, customers, vehicles, distance_matrix, max_g, lambda, 4, 0, 2, known_solution);
            global_best = cost;
            if ((unsigned)global_best == known_solution)
            {
                break;
            }

            for (unsigned i = 1; i < tries; i++)
            {
                cost = GA(depots, customers, vehicles, distance_matrix, max_g, lambda, 4, 0, 1, known_solution);
                if (cost < global_best)
                {
                    global_best = cost;
                    if ((unsigned)global_best == known_solution)
                    {
                        break;
                    }
                }
            }
            break;
        case 4:
            tries = 10;

            cost = ES_1_1_adaptive(depots, customers, vehicles, distance_matrix, max_evaluations /tries, known_solution);
            global_best = cost;
            if ((unsigned)global_best == known_solution)
            {
                break;
            }

            for (unsigned i = 1; i < tries; i++)
            {
                cost = ES_1_1_adaptive(depots, customers, vehicles, distance_matrix, max_evaluations / tries, known_solution);
                if (cost < global_best)
                {
                    global_best = cost;
                    if ((unsigned)global_best == known_solution)
                    {
                        break;
                    }
                }
            }
            break;
        case 5:
            lambda = 100;
            max_g = (max_evaluations / tries) / lambda;
            cost = ES_mu_plus_lambda_adaptive(depots, customers, vehicles, distance_matrix, max_g, 15, lambda, known_solution);
            global_best = cost;
            if ((unsigned)global_best == known_solution)
            {
                break;
            }

            for (unsigned i = 1; i < tries; i++)
            {
                cost = ES_mu_plus_lambda_adaptive(depots, customers, vehicles, distance_matrix, max_g, 15, lambda, known_solution);
                if (cost < global_best)
                {
                    global_best = cost;
                    if ((unsigned)global_best == known_solution)
                    {
                        break;
                    }
                }
            }
            break;
        case 6:
            lambda = 200;
            max_g = (max_evaluations / tries) / lambda;
            cost = ES_mu_comma_lambda_adaptive(depots, customers, vehicles, distance_matrix, max_g, 15, lambda, known_solution);
            global_best = cost;
            if ((unsigned)global_best == known_solution)
            {
                break;
            }

            for (unsigned i = 1; i < tries; i++)
            {
                cost = ES_mu_comma_lambda_adaptive(depots, customers, vehicles, distance_matrix, max_g, 15, lambda, known_solution);
                if (cost < global_best)
                {
                    global_best = cost;
                    if ((unsigned)global_best == known_solution)
                    {
                        break;
                    }
                }
            }
            break;
        case 7:
            tries = 1;
            lambda = 100;
            max_g = (max_evaluations / tries) / lambda;
            cost = GA_adaptive(depots, customers, vehicles, distance_matrix, max_g, lambda, 4, known_solution);
            global_best = cost;
            if ((unsigned)global_best == known_solution)
            {
                break;
            }

            for (unsigned i = 1; i < tries; i++)
            {
                cost = GA_adaptive(depots, customers, vehicles, distance_matrix, max_g, lambda, 4, known_solution);
                if (cost < global_best)
                {
                    global_best = cost;
                    if ((unsigned)global_best == known_solution)
                    {
                        break;
                    }
                }
            }
            break;
        case 8:
            cost = ES_mu_plus_lambda_all_equal(depots, customers, vehicles, distance_matrix, 50000, 15, 100);
            break;
        case 9:
            cost = GA_all_equal(depots, customers, vehicles, distance_matrix, 100000, 100, 4);
            break;
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        cout << prefix + number_instance << ":" << cost << ":" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "\n";
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "At least one parameter for choosing the heuristic" << std::endl;
        return 1;
    }
    string h;
    switch (argv[1][0] - '0')
    {
    case 0:
        h = "(1+1)-ES";
        break;
    case 1:
        h = "(mu + lambda)-ES";
        break;
    case 2:
        h = "(mu, lambda)-ES";
        break;
    case 3:
        h = "GA";
        break;
    case 4:
        h = "(1+1)-ES_Adaptive";
        break;
    case 5:
        h = "(mu + lambda)-ES_Adaptive";
        break;
    case 6:
        h = "(mu, lambda)-ES_Adaptive";
        break;
    case 7:
        h = "GA_Adaptive";
        break;
    case 8:
        h = "(mu + lambda)-ES_All_Equal";
        break;
    case 9:
        h = "GA_All_Equal";
        break;
    }
    cout << h << "\n";

    test_heuristic(argv[1][0] - '0', 5, "Ir", 1, 1, 545);
    test_heuristic(argv[1][0] - '0', 5, "Ir", 2, 2, 832);
    test_heuristic(argv[1][0] - '0', 5, "Ir", 3, 3, 832);
    test_heuristic(argv[1][0] - '0', 10, "Ir", 4, 4, 2082);
    test_heuristic(argv[1][0] - '0', 10, "Ir", 5, 5, 1827);
    test_heuristic(argv[1][0] - '0', 10, "Ir", 6, 6, 1786);
    test_heuristic(argv[1][0] - '0', 20, "Ir", 7, 7, 5424);
    test_heuristic(argv[1][0] - '0', 20, "Ir", 8, 8, 3737);
    test_heuristic(argv[1][0] - '0', 20, "Ir", 9, 9, 3082);
    test_heuristic(argv[1][0] - '0', 20, "Ir", 10, 10, 2786);
    test_heuristic(argv[1][0] - '0', 20, "Ir", 11, 11, 2909);
    test_heuristic(argv[1][0] - '0', 20, "Ir", 12, 12, 3171);
    test_heuristic(argv[1][0] - '0', 25, "Ir", 13, 13, 8288);
    test_heuristic(argv[1][0] - '0', 25, "Ir", 14, 14, 7257);
    test_heuristic(argv[1][0] - '0', 25, "Ir", 15, 15, 8625);
    test_heuristic(argv[1][0] - '0', 25, "Ir", 16, 16, 5265);
    test_heuristic(argv[1][0] - '0', 25, "Ir", 17, 17, 6107);
    test_heuristic(argv[1][0] - '0', 25, "Ir", 18, 18, 5788);

    test_heuristic(argv[1][0] - '0', 35, "p", 1, 1, 660);
    test_heuristic(argv[1][0] - '0', 35, "p", 2, 2, 660);
    test_heuristic(argv[1][0] - '0', 35, "p", 3, 3, 906);
    test_heuristic(argv[1][0] - '0', 35, "p", 4, 4, 1881);
    test_heuristic(argv[1][0] - '0', 35, "p", 5, 5, 1871);
    test_heuristic(argv[1][0] - '0', 35, "p", 6, 6, 1460);
    test_heuristic(argv[1][0] - '0', 35, "p", 7, 7, 1453);
    test_heuristic(argv[1][0] - '0', 35, "p", 8, 8, 0);
    test_heuristic(argv[1][0] - '0', 35, "p", 9, 9, 0);
    test_heuristic(argv[1][0] - '0', 35, "p", 10, 10, 0);
    test_heuristic(argv[1][0] - '0', 35, "p", 11, 11, 0);
    test_heuristic(argv[1][0] - '0', 35, "p", 12, 12, 2769);
    test_heuristic(argv[1][0] - '0', 35, "p", 15, 15, 5618);
    test_heuristic(argv[1][0] - '0', 35, "p", 18, 18, 0);

    test_heuristic(argv[1][0] - '0', 35, "pr", 1, 1, 1167);
    test_heuristic(argv[1][0] - '0', 35, "pr", 2, 2, 2422);
    test_heuristic(argv[1][0] - '0', 35, "pr", 3, 3, 4287);
    test_heuristic(argv[1][0] - '0', 35, "pr", 4, 4, 5582);
    test_heuristic(argv[1][0] - '0', 35, "pr", 5, 5, 6782);
    test_heuristic(argv[1][0] - '0', 35, "pr", 6, 6, 0);
    test_heuristic(argv[1][0] - '0', 35, "pr", 7, 7, 1594);
    test_heuristic(argv[1][0] - '0', 35, "pr", 8, 8, 3817);
    test_heuristic(argv[1][0] - '0', 35, "pr", 9, 9, 5668);
    test_heuristic(argv[1][0] - '0', 35, "pr", 10, 10, 0);

    exit(0);

    unsigned depots = 0;
    unsigned customers = 0;
    double **coordinate_matrix;
    //string file = "./dat/Ir01.txt";
    string file = "./dat/pr10.txt";
    unsigned vehicles = 35;

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
        best = ES_1_1_adaptive(depots, customers, vehicles, distance_matrix, 250000);
        for (int i = 1; i < 20; i++)
        {
            double result = ES_1_1_adaptive(depots, customers, vehicles, distance_matrix, 250000);
            if (result < best)
            {
                best = result;
            }
        }
        break;
    case 1:
        best = ES_mu_plus_lambda_adaptive(depots, customers, vehicles, distance_matrix, 50000, 15, 100);
        break;
    case 2:
        best = ES_mu_comma_lambda_adaptive(depots, customers, vehicles, distance_matrix, 50000, 15, 200);
        break;
    case 3:
        best = GA_adaptive(depots, customers, vehicles, distance_matrix, 50000, 100, 4);
        break;
    case 4:
        //benchmark test
        best = ES_mu_plus_lambda_test(depots, customers, vehicles, distance_matrix, 320000, 15, 100);
        break;
    case 5:
        best = ES_mu_plus_lambda_adaptive(depots, customers, vehicles, distance_matrix, 50000, 15, 250);
        break;
    case 6:
        std::cout << "(mu+lambda)-ES\n";
        for (unsigned mut = 4; mut < 5; mut++)
        {
            for (unsigned cross = 0; cross < 6; cross++)
            {
                best = ES_mu_plus_lambda(depots, customers, vehicles, distance_matrix, 350000, 15, 250, mut, cross);
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
        break;
    case 7:
        std::cout << "(mu,lambda)-ES\n";
        for (unsigned mut = 4; mut < 5; mut++)
        {
            for (unsigned cross = 0; cross < 6; cross++)
            {
                best = ES_mu_comma_lambda(depots, customers, vehicles, distance_matrix, 350000, 15, 250, mut, cross);
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
        break;
    case 8:
        std::cout << "(1+1)-ES\n";
        for (unsigned mut = 0; mut < 5; mut++)
        {
            best = ES_1_1(depots, customers, vehicles, distance_matrix, 4375000, mut);
            double global_best = best;
            for (unsigned i = 1; i < 20; i++)
            {
                best = ES_1_1(depots, customers, vehicles, distance_matrix, 4375000, mut);
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
        break;
    case 9:
        std::cout << "GA\n";
        for (unsigned mut = 2; mut < 5; mut++)
        {
            for (unsigned cross = 4; cross < 5; cross++)
            {
                best = GA(depots, customers, vehicles, distance_matrix, 350000, 250, 4, mut, cross);
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
        break;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    std::cout << "Finale: " << best << endl;

    return 0;
}