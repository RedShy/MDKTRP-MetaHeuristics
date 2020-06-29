#ifndef ES_1_1_H
#define ES_1_1_H

#include "individual.h"

double ES_1_1(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned mut)
{
    Individual parent(vehicles, depots, customers, distance_matrix);
    parent.random_inizialize();
    parent.calculate_cost();

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
        }
        g++;
    }

    return parent.get_cost();
}

#endif