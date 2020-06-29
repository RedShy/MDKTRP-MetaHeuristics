#ifndef ES_1_1_H
#define ES_1_1_H

#include "individual.h"

double ES_1_1(const int depots, const int customers, const int vehicles, const double *const *const distance_matrix, const int N)
{
    Individual parent(vehicles, depots, customers, distance_matrix);
    parent.random_inizialize();
    parent.calculate_cost();
    //parent.print_tour_matrix();
    //cout << "Initial cost " << parent.get_cost() << "\n";
    const int generations = 2000000 * N;
    //int max_g=0;
    unsigned last_improved_generation = 0;

    std::uniform_int_distribution<unsigned> random_mut(0, 3);
    unsigned g = 0;
    while (true)
    {
        Individual child(parent);

        //child.swap2();
        //child.swap3();
        //child.scrumble();
        //child.inversion();
        child.insertion_repeated();
        const unsigned mut = random_mut(mt);
        // if (mut == 0)
        // {
        //     child.swap2();
        // }
        // else if (mut == 1)
        // {
        //     child.swap3();
        // }
        // else if (mut == 2)
        // {
        //     child.scrumble();
        // }
        // else
        // {
        //     child.inversion();
        // }

        child.calculate_cost();
        if (child.get_cost() < parent.get_cost())
        {

            //max_g = g;
            //cout << "Improved from " << parent.get_cost() << " to " << child.get_cost() << "\n";

            //TODO assegnamento costoso da modificare
            parent = child;
            //parent.print_tour_matrix();

            last_improved_generation = g;
        }
        g++;

        //se dopo N generazioni non c'è stato un miglioramento, esci dalla ricerca
        if (g - last_improved_generation >= N)
        {
            //cout<<"LAST IMPROVED GENERATION: "<<last_improved_generation<<"\n";
            return parent.get_cost();
        }
    }

    //cout<<"Last improved generation: "<<max_g<<"\n";

    return parent.get_cost();
}

#endif