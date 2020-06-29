#ifndef ES_mu_lambda_H
#define ES_mu_lambda_H

#include "individual.h"

double ES_mu_plus_lambda(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned mu, const unsigned lambda)
{
    double best_cost = std::numeric_limits<double>::max();

    //vettore che contiene la popolazione di mu parents + lambda children
    std::vector<Individual> individuals;
    individuals.assign(mu + lambda, Individual(vehicles, depots, customers, distance_matrix));

    Individual best_individual(vehicles, depots, customers, distance_matrix);

    //inizializzazione dei parents
    for (Individual &i : individuals)
    {
        i.random_inizialize();
        i.calculate_cost();
        if (i.get_cost() < best_cost)
        {
            best_cost = i.get_cost();
            //std::cout << "Improved: " << best_cost << "\n";

            best_individual = i;
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

    std::uniform_int_distribution<unsigned> random_parent(0, mu - 1);
    std::uniform_int_distribution<unsigned> random_element(0, 4);
    std::uniform_int_distribution<unsigned> random_cross(0, 1);
    std::uniform_int_distribution<unsigned> random_mut(0, 3);

    unsigned g = 0;
    while (g < max_g)
    {
        unsigned added_children = 0;

        //generazione di lambda figli
        for (unsigned i = 0; i < lambda; i++)
        {
            //scelta casuale di un parent
            const unsigned p = random_parent(mt);

            //scelta casuale di un altro parent diverso dal primo
            unsigned p2 = random_parent(mt);
            while (p2 == p)
            {
                p2 = random_parent(mt);
            }

            //crea un figlio
            Individual child(individuals[p]);
            //Individual child(vehicles, depots, customers, distance_matrix);

            //ricombina i genitori
            // const unsigned cr = random_cross(mt);
            // if (cr == 0)
            // {
            child.two_point_cross_over(individuals[p], individuals[p2]);
            // }
            // else if (cr == 1)
            // {
            //     //child.best_order_cross_over(individuals[p], individuals[p2], best_individual);
            // }
            // else if (cr == 2)
            // {
            //     child.one_point_cross_over(individuals[p], individuals[p2]);
            // }
            // else if (cr == 3)
            // {
            //     child.uniform_cross_over(individuals[p], individuals[p2]);
            // }
            // else if (cr == 4)
            // {
            //     child.position_base_cross_over(individuals[p], individuals[p2]);
            // }

            //child.one_point_cross_over(individuals[p], individuals[p2]);
            //child.two_point_cross_over(individuals[p], individuals[p2]);
            //child.best_order_cross_over(individuals[p], individuals[p2], best_individual);

            //facciamo mutazione solo con una certa probabilita
            //const unsigned mut = random_mut(mt);
            //const unsigned rate = random_element(mt);
            // if (rate == 0)
            // {
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
            // }

            child.swap2();
            //child.swap3();
            //child.scrumble();
            //child.inversion();
            //child.insertion_prova();
            //child.insertion_repeated();

            child.calculate_cost();

            //se il figlio è migliore del parent peggiore allora lo manteniamo, altrimenti lo scartiamo direttamente
            if (child.get_cost() < worst_parent_cost)
            {
                individuals[mu + added_children] = child;
                added_children++;

                if (child.get_cost() < best_cost)
                {
                    best_cost = child.get_cost();
                    std::cout << "Child improved: " << best_cost << "\n";

                    best_individual = child;
                }
            }
        }

        //facciamo un sort per selezionare i miglior mu individui
        std::sort(individuals.begin(), individuals.begin() + mu + added_children);

        //prendiamo il valore del parent peggiore
        worst_parent_cost = individuals[mu - 1].get_cost();

        g++;
    }

    return best_cost;
}

double ES_mu_plus_lambda_adaptive(const int depots, const int customers, const int vehicles, const double *const *const distance_matrix, const int N, const int mu, const int lambda)
{
    double best_cost = std::numeric_limits<double>::max();

    std::vector<Individual> individuals;
    individuals.assign(mu + lambda, Individual(vehicles, depots, customers, distance_matrix));

    Individual best_individual(vehicles, depots, customers, distance_matrix);

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

    double worst_parent_cost = individuals[0].get_cost();
    for (int i = 1; i < mu; ++i)
    {
        if (individuals[i].get_cost() > worst_parent_cost)
        {
            worst_parent_cost = individuals[i].get_cost();
        }
    }

    std::uniform_int_distribution<unsigned> random_parent(0, mu - 1);
    std::uniform_int_distribution<unsigned> random_element(0, 4);
    std::uniform_int_distribution<unsigned> random_cross(0, 1);
    std::uniform_int_distribution<unsigned> random_mut(0, 3);

    unsigned score_cross_overs[] = {0, 0, 0, 0, 0};
    unsigned score_mutators[] = {0, 0, 0, 0, 0};

    double mprob = 1 / (double)5;
    double probabilities_mutators[] = {mprob, mprob, mprob, mprob, mprob};
    double total_good_mutation = 0;

    double cprob = 1 / (double)5;
    double probabilities_cross[] = {cprob, cprob, cprob, cprob, cprob};
    double total_good_cross = 0;

    unsigned last_improved_generation = 0;
    const unsigned generations = 14000 * N;
    unsigned g = 0;
    while (true)
    {
        unsigned added_children = 0;

        for (unsigned i = 0; i < lambda; i++)
        {
            const unsigned p = random_parent(mt);

            unsigned p2 = random_parent(mt);
            while (p2 == p)
            {
                p2 = random_parent(mt);
            }

            Individual child(individuals[p]);
            //Individual child(vehicles, depots, customers, distance_matrix);

            std::discrete_distribution<unsigned> dist1{probabilities_cross[0], probabilities_cross[1], probabilities_cross[2], probabilities_cross[3], probabilities_cross[4]};
            const unsigned cr = dist1(mt);
            if (cr == 0)
            {
                child.two_point_cross_over(individuals[p], individuals[p2]);
            }
            else if (cr == 1)
            {
                child.best_order_cross_over(individuals[p], individuals[p2], best_individual);
            }
            else if (cr == 2)
            {
                child.one_point_cross_over(individuals[p], individuals[p2]);
            }
            else if (cr == 3)
            {
                child.uniform_cross_over(individuals[p], individuals[p2]);
            }
            else if (cr == 4)
            {
                child.position_base_cross_over(individuals[p], individuals[p2]);
            }

            std::discrete_distribution<unsigned> dist{probabilities_mutators[0], probabilities_mutators[1], probabilities_mutators[2], probabilities_mutators[3], probabilities_mutators[4]};
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
                child.insertion();
            }

            child.calculate_cost();

            if (child.get_cost() < worst_parent_cost)
            {
                individuals[mu + added_children] = child;
                added_children++;

                if (child.get_cost() < best_cost)
                {
                    best_cost = child.get_cost();
                    std::cout << "Child improved: " << best_cost << "\n";
                    last_improved_generation = g;

                    best_individual = child;
                }

                score_cross_overs[cr]++;
                score_mutators[mut]++;
                total_good_mutation++;
                total_good_cross++;
            }
        }

        std::sort(individuals.begin(), individuals.begin() + mu + added_children);

        worst_parent_cost = individuals[mu - 1].get_cost();

        g++;
        if (g % 1000 == 0)
        {
            for (unsigned i = 0; i < 4; i++)
            {
                probabilities_mutators[i] = score_mutators[i] / total_good_mutation;
            }

            for (unsigned i = 0; i < 5; i++)
            {
                probabilities_cross[i] = score_cross_overs[i] / total_good_cross;
            }
        }

        if (g % 10000 == 0)
        {
            cout << "SCORE CROSS-OVERS\n";
            for (unsigned i : score_cross_overs)
            {
                cout << i << " ";
            }
            cout << "\n";

            cout << "SCORE MUTATORS\n";
            for (unsigned i : score_mutators)
            {
                cout << i << " ";
            }
            cout << "\n";
        }
    }

    return best_cost;
}

double ES_mu_plus_lambda_test(const int depots, const int customers, const int vehicles, const double *const *const distance_matrix, const int N, const int mu, const int lambda)
{
    double best_cost = std::numeric_limits<double>::max();

    std::vector<Individual> individuals;
    individuals.assign(mu + lambda, Individual(vehicles, depots, customers, distance_matrix));

    Individual best_individual(vehicles, depots, customers, distance_matrix);

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

    double worst_parent_cost = individuals[0].get_cost();
    for (int i = 1; i < mu; ++i)
    {
        if (individuals[i].get_cost() > worst_parent_cost)
        {
            worst_parent_cost = individuals[i].get_cost();
        }
    }

    std::uniform_int_distribution<int> random_parent(0, mu - 1);
    std::uniform_int_distribution<unsigned> random_element(0, 4);
    for (unsigned g = 0; g < N; g++)
    {
        //cout<<"G: "<<g<<"\n";
        unsigned added_children = 0;

        for (unsigned i = 0; i < lambda; i++)
        {
            const unsigned p = random_parent(mt);

            unsigned p2 = random_parent(mt);
            while (p2 == p)
            {
                p2 = random_parent(mt);
            }

            Individual child(individuals[p]);
            //Individual child(vehicles, depots, customers, distance_matrix);

            //child.best_order_cross_over(individuals[p], individuals[p2], best_individual);
            //child.one_point_cross_over(individuals[p], individuals[p2]);
            //child.two_point_cross_over(individuals[p], individuals[p2]);
            //child.position_base_cross_over(individuals[p], individuals[p2]);
            //child.uniform_cross_over(individuals[p], individuals[p2]);
            //child.partially_mapped_cross_over(individuals[p], individuals[p2]);

            //child.scrumble();
            //child.inversion();
            //child.swap3();
            //child.swap2();
            //child.insertion_prova();
            // if(random_element(mt) == 0)
            //     child.swap2();
            //child.random_inizialize();

            //child.calculate_cost();

            if (child.get_cost() < worst_parent_cost)
            {
                individuals[mu + added_children] = child;
                added_children++;

                if (child.get_cost() < best_cost)
                {
                    best_cost = child.get_cost();

                    best_individual = child;
                }
            }
        }

        std::sort(individuals.begin(), individuals.begin() + mu + added_children);

        worst_parent_cost = individuals[mu - 1].get_cost();
    }
    return best_cost;
}

#endif