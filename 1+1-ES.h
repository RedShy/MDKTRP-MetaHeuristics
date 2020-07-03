#ifndef ES_1_1_H
#define ES_1_1_H

#include "individual.h"
double ES_1_1(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned mut, const unsigned known_solution = 0)
{
    Individual parent(vehicles, depots, customers, distance_matrix);
    parent.random_inizialize();
    parent.calculate_cost();

    if ((unsigned)parent.get_cost() == known_solution)
    {
        return parent.get_cost();
    }

    unsigned g = 0;
    while (g < max_g)
    {
        Individual child(parent);

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

        child.calculate_cost();
        if (child.get_cost() < parent.get_cost())
        {
            parent = child;
            //cout<<"Improved:"<<child.get_cost()<<"\n";

            if ((unsigned)parent.get_cost() == known_solution)
            {
                return parent.get_cost();
            }
        }
        g++;
    }

    return parent.get_cost();
}

double ES_1_1_adaptive(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned known_solution = 0)
{
    Individual parent(vehicles, depots, customers, distance_matrix);
    parent.random_inizialize();
    parent.calculate_cost();

    if ((unsigned)parent.get_cost() == known_solution)
    {
        return parent.get_cost();
    }

    unsigned score_mutators[] = {1, 1, 1, 1};

    double mprob = 1 / (double)4;
    double probabilities_mutators[] = {mprob, mprob, mprob, mprob};
    double total_good_mutation = 4;

    unsigned g = 0;
    while (g < max_g)
    {
        Individual child(parent);

        std::discrete_distribution<unsigned> dist{probabilities_mutators[0], probabilities_mutators[1], probabilities_mutators[2], probabilities_mutators[3]};
        const unsigned mut = dist(mt);
        if (mut == 0)
        {
            child.swap2();
        }
        else if (mut == 1)
        {
            child.swap3();
        }
        else if (mut == 2)
        {
            child.scrumble();
        }
        else if (mut == 3)
        {
            child.inversion();
        }
        else
        {
            //child.insertion();
        }

        child.calculate_cost();
        if (child.get_cost() < parent.get_cost())
        {
            parent = child;
            //cout<<"Improved:"<<child.get_cost()<<"\n";

            score_mutators[mut]++;
            total_good_mutation++;

            if ((unsigned)parent.get_cost() == known_solution)
            {
                return parent.get_cost();
            }
        }
        g++;

        //aggiorna le probabilità ad ogni generazione
        for (unsigned i = 0; i < 4; i++)
        {
            probabilities_mutators[i] = score_mutators[i] / total_good_mutation;
        }
    }

    return parent.get_cost();
}

#endif