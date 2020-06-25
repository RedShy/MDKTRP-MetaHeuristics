#ifndef ES_mu_comma_lambda_H
#define ES_mu_comma_lambda_H
#include "individual.h"

double ES_mu_comma_lambda(const int depots, const int customers, const int vehicles, const double *const *const distance_matrix, const int N, const int mu, const int lambda)
{
    double best_cost = std::numeric_limits<double>::max();

    //vettore che contiene i mu parents
    std::vector<Individual> parents;
    parents.assign(mu, Individual(vehicles, depots, customers, distance_matrix, mt));

    //inizializzazione dei parents
    for (Individual& p : parents)
    {
        p.random_inizialize();
        p.calculate_cost();
        if (p.get_cost() < best_cost)
        {
            best_cost = p.get_cost();
            //std::cout << "Improved: " << best_cost << "\n";
        }
    }

    std::uniform_int_distribution<int> random_parent(0, mu - 1);

    //vettore che contiene i mu migliori figli
    std::vector<Individual> children;
    children.assign(mu, Individual(vehicles, depots, customers, distance_matrix, mt));

    //puntatori che mi servono per invertire i parents con i children ad ogni generazione
    std::vector<Individual> *children_p = &children;
    std::vector<Individual> *parents_p = &parents;
    const unsigned remainder_children = lambda - mu;

    unsigned last_improved_generation = 0;
    const int generations = 14000 * N;
    unsigned g = 0;
    while(true)
    {
        const std::vector<Individual>& parents_a = *parents_p;
        std::vector<Individual>& children_a = *children_p;

        //generazione di mu figli: almeno mu figli dovranno essere parents
        for (unsigned i = 0; i < mu; i++)
        {
            //scelta casuale di un parent
            const int p = random_parent(mt);

            //genera un figlio
            children_a[i] = parents_a[p];
            children_a[i].mutate();
            children_a[i].calculate_cost();

            if (children_a[i].get_cost() < best_cost)
            {
                best_cost = children_a[i].get_cost();
                //std::cout << "Child improved: " << best_cost << "\n";

                last_improved_generation = g;
            }
        }

        //cerco il child peggiore
        unsigned worst_child = 0;
        unsigned worst_child_cost = children_a[0].get_cost();
        for (unsigned i = 1; i < mu; ++i)
        {
            if (children_a[i].get_cost() > worst_child_cost)
            {
                worst_child_cost = children_a[i].get_cost();
                worst_child = i;
            }
        }

        //generazione dei figli rimanenti
        for (int i = 0; i < remainder_children; i++)
        {
            //scelta casuale di un parent
            const int p = random_parent(mt);

            //genera un figlio
            Individual child(parents_a[p]);
            child.mutate();
            child.calculate_cost();

            //se il figlio è migliore del figlio peggiore allora lo manteniamo, altrimenti lo scartiamo direttamente
            if (child.get_cost() < worst_child_cost)
            {
                worst_child_cost = child.get_cost();
                children_a[worst_child] = child;

                //dato che abbiamo sostituito il figlio peggiore, cerchiamo il prossimo figlio peggiore
                for (unsigned i = 0; i < mu; ++i)
                {
                    if (children_a[i].get_cost() > worst_child_cost)
                    {
                        worst_child_cost = children_a[i].get_cost();
                        worst_child = i;
                    }
                }

                if (child.get_cost() < best_cost)
                {
                    best_cost = child.get_cost();
                    //std::cout << "Child improved: " << best_cost << "\n";

                    last_improved_generation = g;
                }
            }
        }

        std::vector<Individual> *tmp = parents_p;
        parents_p = children_p;
        children_p = tmp;

        g++;
        //se dopo N generazioni non c'è stato un miglioramento, esci dalla ricerca
        if(g - last_improved_generation >= N)
        {
            return best_cost;
        }
    }
}

#endif