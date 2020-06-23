#ifndef ES_mu_lambda_H
#define ES_mu_lambda_H

#include "individual.h"

double ES_mu_lambda(const int depots, const int customers, const int vehicles, const double *const *const distance_matrix, const int N, const int mu, const int lambda)
{
    double best_cost = std::numeric_limits<double>::max();

    //vettore che contiene la popolazione di mu parents + lambda children
    std::vector<Individual> individuals;
    individuals.assign(mu + lambda, Individual(vehicles, depots, customers, distance_matrix, mt));

    //inizializzazione dei parents
    for (int p = 0; p < mu; p++)
    {
        individuals[p].random_inizialize();
        individuals[p].calculate_cost();

        if (individuals[p].get_cost() < best_cost)
        {
            best_cost = individuals[p].get_cost();
            //std::cout << "Improved: " << best_cost << "\n";
        }
    }

    //Prendiamo il parent peggiore
    double worst_parent_cost = individuals[0].get_cost();
    for (int i = 1; i < mu; ++i)
    {
        if (individuals[i].get_cost() > worst_parent_cost)
        {
            worst_parent_cost = individuals[i].get_cost();
        }
    }

    std::uniform_int_distribution<int> random_parent(0, mu - 1);

    const int generations = 14000 * N;
    for (int g = 0; g < generations; g++)
    {
        int added_children = 0;

        //generazione di lambda figli
        for (int i = 0; i < lambda; i++)
        {
            //scelta casuale di un parent
            const int p = random_parent(mt);

            //genera un figlio
            Individual child(individuals[p]);
            child.mutate();
            child.calculate_cost();

            //se il figlio è migliore del parent peggiore allora lo manteniamo, altrimenti lo scartiamo direttamente
            if (child.get_cost() < worst_parent_cost)
            {
                //TODO è possibile evitare questo assegnamento profondo, magari usando puntatori o indici?
                individuals[mu + added_children] = child;
                added_children++;

                if (child.get_cost() < best_cost)
                {
                    best_cost = child.get_cost();
                    //std::cout << "Child improved: " << best_cost << "\n";
                }
            }
        }

        //facciamo un sort per selezionare i miglior mu individui
        std::sort(individuals.begin(), individuals.begin() + mu + added_children);

        //prendiamo il valore del parent peggiore
        worst_parent_cost = individuals[mu - 1].get_cost();
        // if(added_children > 0)
        // {
        //     std::cout<<"G: "<<g<<" Child added in generation: "<<added_children<<"\n";
        //     // std::cout<<"INDIVIDUALS COSTS\n";
        //     // for(int i=0; i<mu; i++)
        //     // {
        //     //     //cout<<individuals[i].get_cost()<<" ";
        //     //     individuals[i].print_tour_matrix();
        //     // }
        //     // std::cout<<"\n";
        // }
    }
}

#endif