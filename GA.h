#ifndef GA_H
#define GA_H

#include "individual.h"

double GA(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned population_size, const unsigned mut_rate, const unsigned mut, const unsigned cross)
{
    double best_cost = std::numeric_limits<double>::max();

    //vettore che contiene la popolazione
    std::vector<Individual> individuals;
    individuals.assign(population_size, Individual(vehicles, depots, customers, distance_matrix));

    Individual best_individual(vehicles, depots, customers, distance_matrix);

    //inizializzazione della popolazione
    for (Individual &i : individuals)
    {
        i.random_inizialize();
        i.calculate_cost();
        if (i.get_cost() < best_cost)
        {
            best_cost = i.get_cost();

            best_individual = i;
        }
    }

    std::uniform_int_distribution<unsigned> random_parent(0, 50);
    std::uniform_int_distribution<unsigned> random_mut(0, mut_rate);

    unsigned g = 0;
    while (g < max_g)
    {
        std::sort(individuals.begin(), individuals.end());

        std::vector<Individual> new_generation;

        //elitismo al 10%
        const unsigned s = (10 * population_size) / 100;
        for (unsigned i = 0; i < s; i++)
            new_generation.push_back(individuals[i]);

        //dobbiamo inserire il rimanente 90% della popolazione
        const unsigned s2 = (90 * population_size) / 100;
        for (unsigned i = 0; i < s2; i++)
        {
            const unsigned p1 = random_parent(mt);
            unsigned p2 = random_parent(mt);
            while (p1 == p2)
            {
                p2 = random_parent(mt);
            }

            Individual child(vehicles, depots, customers, distance_matrix);

            switch (cross)
            {
            case 0:
                child.one_point_cross_over(individuals[p1], individuals[p2]);
                break;
            case 1:
                child.two_point_cross_over(individuals[p1], individuals[p2]);
                break;
            case 2:
                child.best_order_cross_over(individuals[p1], individuals[p2], best_individual);
                break;
            case 3:
                child.position_base_cross_over(individuals[p1], individuals[p2]);
                break;
            case 4:
                child.uniform_cross_over(individuals[p1], individuals[p2]);
                break;
            }

            //mutazione genetica solo con un certo rateo
            if (random_mut(mt) == 0)
            {
                switch (mut)
                {
                case 0:
                    child.swap2();
                    break;
                case 1:
                    child.swap3();
                    break;
                case 2:
                    child.scrumble();
                    break;
                case 3:
                    child.inversion();
                    break;
                case 4:
                    child.insertion();
                    break;
                }
            }

            child.calculate_cost();
            new_generation.push_back(child);

            if (child.get_cost() < best_cost)
            {
                best_cost = child.get_cost();
                best_individual = child;

                //std::cout << "Child improved: " << best_cost << "\n";
            }
        }

        individuals = new_generation;

        g++;
    }

    return best_individual.get_cost();
}

#endif