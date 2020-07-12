#ifndef GA_H
#define GA_H

#include "individual.h"

double GA(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned population_size, const unsigned mut_rate, const unsigned mut, const unsigned cross, const unsigned known_solution = 0)
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

            if ((unsigned)best_individual.get_cost() == known_solution)
            {
                return best_individual.get_cost();
            }
        }
    }

    std::uniform_int_distribution<unsigned> random_parent(0, population_size / 2);
    std::uniform_int_distribution<unsigned> random_mut(0, mut_rate);

    std::vector<Individual> new_generation;
    new_generation.assign(population_size, Individual(vehicles, depots, customers, distance_matrix));

    std::vector<Individual> *new_generation_p = &new_generation;
    std::vector<Individual> *individuals_p = &individuals;

    unsigned g = 0;
    while (g < max_g)
    {
        //cout<<"G: "<<g<<"\n";
        const std::vector<Individual> &individuals_a = *individuals_p;
        std::vector<Individual> &new_generation_a = *new_generation_p;

        //elitismo al 10%
        const unsigned s = (10 * population_size) / 100;
        unsigned i = 0;
        for (; i < s; i++)
            new_generation_a[i] = individuals_a[i];

        //dobbiamo inserire il rimanente 90% della popolazione
        const unsigned s2 = (90 * population_size) / 100;
        const unsigned s3 = s2 + s;
        for (; i < s3; i++)
        {
            const unsigned p1 = random_parent(mt);
            unsigned p2 = random_parent(mt);
            while (p1 == p2)
            {
                p2 = random_parent(mt);
            }

            //Individual child(vehicles, depots, customers, distance_matrix);

            switch (cross)
            {
            case 0:
                new_generation_a[i].one_point_cross_over(individuals_a[p1], individuals_a[p2]);
                break;
            case 1:
                new_generation_a[i].two_point_cross_over(individuals_a[p1], individuals_a[p2]);
                break;
            case 2:
                new_generation_a[i].best_order_cross_over(individuals_a[p1], individuals_a[p2], best_individual);
                break;
            case 3:
                new_generation_a[i].position_base_cross_over(individuals_a[p1], individuals_a[p2]);
                break;
            case 4:
                new_generation_a[i].uniform_cross_over(individuals_a[p1], individuals_a[p2]);
                break;
            }

            //mutazione genetica solo con un certo rateo
            if (random_mut(mt) == 0)
            {
                switch (mut)
                {
                case 0:
                    new_generation_a[i].swap2();
                    break;
                case 1:
                    new_generation_a[i].swap3();
                    break;
                case 2:
                    new_generation_a[i].scrumble();
                    break;
                case 3:
                    new_generation_a[i].inversion();
                    break;
                case 4:
                    new_generation_a[i].insertion();
                    break;
                }
            }

            new_generation_a[i].calculate_cost();

            if (new_generation_a[i].get_cost() < best_cost)
            {
                best_cost = new_generation_a[i].get_cost();
                best_individual = new_generation_a[i];

                //std::cout << "Child improved: " << best_cost << "\n";

                if ((unsigned)best_individual.get_cost() == known_solution)
                {
                    return best_individual.get_cost();
                }
            }
        }

        std::vector<Individual> *const tmp = individuals_p;
        individuals_p = new_generation_p;
        new_generation_p = tmp;

        std::sort((*individuals_p).begin(), (*individuals_p).end());

        g++;
    }

    return best_individual.get_cost();
}

double GA_adaptive(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned population_size, const unsigned mut_rate, const unsigned known_solution = 0)
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

            if ((unsigned)best_individual.get_cost() == known_solution)
            {
                return best_individual.get_cost();
            }
        }
    }

    std::uniform_int_distribution<unsigned> random_parent(0, population_size / 2);
    std::uniform_int_distribution<unsigned> random_mut(0, mut_rate);

    std::vector<Individual> new_generation;
    new_generation.assign(population_size, Individual(vehicles, depots, customers, distance_matrix));

    std::vector<Individual> *new_generation_p = &new_generation;
    std::vector<Individual> *individuals_p = &individuals;

    unsigned score_mutators[] = {1, 1, 1, 1};
    double mprob = 1 / (double)4;
    double probabilities_mutators[] = {mprob, mprob, mprob, mprob};
    double total_good_mutation = 4;

    unsigned score_cross_overs[] = {1, 1, 1, 1, 1};
    double cprob = 1 / (double)5;
    double probabilities_cross[] = {cprob, cprob, cprob, cprob, cprob};
    double total_good_cross = 5;

    unsigned g = 0;
    while (g < max_g)
    {
        const std::vector<Individual> &individuals_a = *individuals_p;
        std::vector<Individual> &new_generation_a = *new_generation_p;

        //elitismo al 10%
        const unsigned s = (10 * population_size) / 100;
        unsigned i = 0;
        for (; i < s; i++)
            new_generation_a[i] = individuals_a[i];

        //dobbiamo inserire il rimanente 90% della popolazione
        const unsigned s2 = (90 * population_size) / 100;
        const unsigned s3 = s2 + s;
        for (; i < s3; i++)
        {
            const unsigned p1 = random_parent(mt);
            unsigned p2 = random_parent(mt);
            while (p1 == p2)
            {
                p2 = random_parent(mt);
            }

            std::discrete_distribution<unsigned> dist1{probabilities_cross[0], probabilities_cross[1], probabilities_cross[2], probabilities_cross[3], probabilities_cross[4]};
            const unsigned cr = dist1(mt);
            if (cr == 0)
            {
                new_generation_a[i].two_point_cross_over(individuals_a[p1], individuals_a[p2]);
            }
            else if (cr == 1)
            {
                new_generation_a[i].best_order_cross_over(individuals_a[p1], individuals_a[p2], best_individual);
            }
            else if (cr == 2)
            {
                new_generation_a[i].one_point_cross_over(individuals_a[p1], individuals_a[p2]);
            }
            else if (cr == 3)
            {
                new_generation_a[i].uniform_cross_over(individuals_a[p1], individuals_a[p2]);
            }
            else if (cr == 4)
            {
                new_generation_a[i].position_base_cross_over(individuals_a[p1], individuals_a[p2]);
            }

            //mutazione genetica solo con un certo rateo
            const unsigned rate = random_mut(mt);
            unsigned mut = 0;
            if (rate == 0)
            {
                std::discrete_distribution<unsigned> dist{probabilities_mutators[0], probabilities_mutators[1], probabilities_mutators[2], probabilities_mutators[3]};
                mut = dist(mt);
                if (mut == 0)
                {
                    new_generation_a[i].swap2();
                }
                else if (mut == 1)
                {
                    new_generation_a[i].swap3();
                }
                else if (mut == 2)
                {
                    new_generation_a[i].scrumble();
                }
                else if (mut == 3)
                {
                    new_generation_a[i].inversion();
                }
                else
                {
                    //new_generation_a[i].insertion();
                }
            }

            new_generation_a[i].calculate_cost();

            if (new_generation_a[i].get_cost() < best_cost)
            {
                best_cost = new_generation_a[i].get_cost();
                best_individual = new_generation_a[i];

                if ((unsigned)best_individual.get_cost() == known_solution)
                {
                    return best_individual.get_cost();
                }

                //std::cout << "Child improved: " << best_cost << "\n";

                score_cross_overs[cr]++;
                total_good_cross++;

                if (rate == 0)
                {
                    score_mutators[mut]++;
                    total_good_mutation++;
                }
            }
        }

        std::vector<Individual> *const tmp = individuals_p;
        individuals_p = new_generation_p;
        new_generation_p = tmp;

        std::sort((*individuals_p).begin(), (*individuals_p).end());

        g++;

        //aggiorna le probabilità ad ogni generazione
        for (unsigned i = 0; i < 4; i++)
        {
            probabilities_mutators[i] = score_mutators[i] / total_good_mutation;
        }

        for (unsigned i = 0; i < 5; i++)
        {
            probabilities_cross[i] = score_cross_overs[i] / total_good_cross;
        }
    }

    return best_individual.get_cost();
}

double GA_all_equal(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned population_size, const unsigned mut_rate)
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

    std::uniform_int_distribution<unsigned> random_parent(0, population_size / 2);
    std::uniform_int_distribution<unsigned> random_mut(0, mut_rate);
    std::uniform_int_distribution<unsigned> random_cross(0, 4);
    std::uniform_int_distribution<unsigned> random_mutator(0, 4);

    std::vector<Individual> new_generation;
    new_generation.assign(population_size, Individual(vehicles, depots, customers, distance_matrix));

    std::vector<Individual> *new_generation_p = &new_generation;
    std::vector<Individual> *individuals_p = &individuals;

    unsigned g = 0;
    while (g < max_g)
    {
        const std::vector<Individual> &individuals_a = *individuals_p;
        std::vector<Individual> &new_generation_a = *new_generation_p;

        //elitismo al 10%
        const unsigned s = (10 * population_size) / 100;
        unsigned i = 0;
        for (; i < s; i++)
            new_generation_a[i] = individuals_a[i];

        //dobbiamo inserire il rimanente 90% della popolazione
        const unsigned s2 = (90 * population_size) / 100;
        const unsigned s3 = s2 + s;
        for (; i < s3; i++)
        {
            const unsigned p1 = random_parent(mt);
            unsigned p2 = random_parent(mt);
            while (p1 == p2)
            {
                p2 = random_parent(mt);
            }

            Individual child(vehicles, depots, customers, distance_matrix);

            const unsigned cross = random_cross(mt);
            switch (cross)
            {
            case 0:
                new_generation_a[i].one_point_cross_over(individuals_a[p1], individuals_a[p2]);
                break;
            case 1:
                new_generation_a[i].two_point_cross_over(individuals_a[p1], individuals_a[p2]);
                break;
            case 2:
                new_generation_a[i].best_order_cross_over(individuals_a[p1], individuals_a[p2], best_individual);
                break;
            case 3:
                new_generation_a[i].position_base_cross_over(individuals_a[p1], individuals_a[p2]);
                break;
            case 4:
                new_generation_a[i].uniform_cross_over(individuals_a[p1], individuals_a[p2]);
                break;
            }

            //mutazione genetica solo con un certo rateo
            if (random_mut(mt) == 0)
            {
                const unsigned mut = random_mutator(mt);
                switch (mut)
                {
                case 0:
                    new_generation_a[i].swap2();
                    break;
                case 1:
                    new_generation_a[i].swap3();
                    break;
                case 2:
                    new_generation_a[i].scrumble();
                    break;
                case 3:
                    new_generation_a[i].inversion();
                    break;
                case 4:
                    new_generation_a[i].insertion();
                    break;
                }
            }

            new_generation_a[i].calculate_cost();

            if (new_generation_a[i].get_cost() < best_cost)
            {
                best_cost = new_generation_a[i].get_cost();
                best_individual = new_generation_a[i];

                //std::cout << "Child improved: " << best_cost << "\n";
            }
        }

        std::vector<Individual> *const tmp = individuals_p;
        individuals_p = new_generation_p;
        new_generation_p = tmp;

        std::sort((*individuals_p).begin(), (*individuals_p).end());

        g++;
    }

    return best_individual.get_cost();
}

#endif