#ifndef ES_1_1_H
#define ES_1_1_H

#include "individual.h"

double ES_1_1(const int depots, const int customers, const int vehicles, const double *const *const distance_matrix, const int N)
{
    Individual parent(vehicles, depots, customers, distance_matrix, mt);
    parent.random_inizialize();
    parent.calculate_cost();
    //parent.print_tour_matrix();
    //cout << "Initial cost " << parent.get_cost() << "\n";
    const int generations = 2000000 * N;
    //int max_g=0;
    for (int g = 0; g < generations; g++)
    {
        Individual child(parent);
        child.mutate();
        child.calculate_cost();
        if (child.get_cost() < parent.get_cost())
        {

            //max_g = g;
            //cout << "Improved from " << parent.get_cost() << " to " << child.get_cost() << "\n";

            //TODO assegnamento costoso da modificare
            parent = child;
            //parent.print_tour_matrix();
        }
    }

   //cout<<"Last improved generation: "<<max_g<<"\n";

    return parent.get_cost();
}

#endif