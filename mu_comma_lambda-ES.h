#ifndef ES_mu_comma_lambda_H
#define ES_mu_comma_lambda_H
#include "individual.h"

double ES_mu_comma_lambda(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned mu, const unsigned lambda, const unsigned mut, const unsigned cross, const unsigned known_solution = 0)
{
    double best_cost = std::numeric_limits<double>::max();
    Individual best_individual(vehicles, depots, customers, distance_matrix);

    //vettore che contiene i mu parents
    std::vector<Individual> parents;
    parents.assign(mu, Individual(vehicles, depots, customers, distance_matrix));

    //inizializzazione dei parents
    for (Individual &p : parents)
    {
        p.random_inizialize();
        p.calculate_cost();
        if (p.get_cost() < best_cost)
        {
            best_cost = p.get_cost();

            best_individual = p;

            if ((unsigned)best_individual.get_cost() == known_solution)
            {
                return best_individual.get_cost();
            }
        }
    }

    //vettore che contiene i mu migliori figli
    std::vector<Individual> children;
    children.assign(mu, Individual(vehicles, depots, customers, distance_matrix));

    //puntatori che mi servono per invertire i parents con i children ad ogni generazione
    std::vector<Individual> *children_p = &children;
    std::vector<Individual> *parents_p = &parents;
    const unsigned remainder_children = lambda - mu;

    std::uniform_int_distribution<unsigned> random_mut(0, 3);
    std::uniform_int_distribution<unsigned> random_cross(0, 1);
    std::uniform_int_distribution<int> random_parent(0, mu - 1);

    unsigned g = 0;
    while (g < max_g)
    {
        const std::vector<Individual> &parents_a = *parents_p;
        std::vector<Individual> &children_a = *children_p;

        //generazione di mu figli: almeno mu figli dovranno essere parents
        for (unsigned i = 0; i < mu; i++)
        {
            //scelta casuale di un parent
            const int p = random_parent(mt);

            //genera un figlio
            children_a[i] = parents_a[p];

            unsigned p2 = random_parent(mt);
            while (p2 == p)
            {
                p2 = random_parent(mt);
            }

            switch (cross)
            {
            case 0:
                children_a[i].one_point_cross_over(parents_a[p], parents_a[p2]);
                break;
            case 1:
                children_a[i].two_point_cross_over(parents_a[p], parents_a[p2]);
                break;
            case 2:
                children_a[i].best_order_cross_over(parents_a[p], parents_a[p2], best_individual);
                break;
            case 3:
                children_a[i].position_base_cross_over(parents_a[p], parents_a[p2]);
                ;
                break;
            case 4:
                children_a[i].uniform_cross_over(parents_a[p], parents_a[p2]);
                break;
            case 5:
                //no cross over
                break;
            }

            switch (mut)
            {
            case 0:
                children_a[i].swap2();
                break;
            case 1:
                children_a[i].swap3();
                break;
            case 2:
                children_a[i].scrumble();
                break;
            case 3:
                children_a[i].inversion();
                break;
            case 4:
                children_a[i].insertion();
                break;
            }

            children_a[i].calculate_cost();

            if (children_a[i].get_cost() < best_cost)
            {
                best_cost = children_a[i].get_cost();

                best_individual = children_a[i];

                if ((unsigned)best_individual.get_cost() == known_solution)
                {
                    return best_individual.get_cost();
                }
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

            unsigned p2 = random_parent(mt);
            while (p2 == p)
            {
                p2 = random_parent(mt);
            }

            switch (cross)
            {
            case 0:
                child.one_point_cross_over(parents_a[p], parents_a[p2]);
                break;
            case 1:
                child.two_point_cross_over(parents_a[p], parents_a[p2]);
                break;
            case 2:
                child.best_order_cross_over(parents_a[p], parents_a[p2], best_individual);
                break;
            case 3:
                child.position_base_cross_over(parents_a[p], parents_a[p2]);
                ;
                break;
            case 4:
                child.uniform_cross_over(parents_a[p], parents_a[p2]);
                break;
            case 5:
                //no cross over
                break;
            }

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

                    best_individual = child;

                    if ((unsigned)best_individual.get_cost() == known_solution)
                    {
                        return best_individual.get_cost();
                    }
                }
            }
        }

        std::vector<Individual> *tmp = parents_p;
        parents_p = children_p;
        children_p = tmp;

        g++;
    }
    return best_individual.get_cost();
}

double ES_mu_comma_lambda_adaptive(const unsigned depots, const unsigned customers, const unsigned vehicles, const double *const *const distance_matrix, const unsigned max_g, const unsigned mu, const unsigned lambda, const unsigned known_solution = 0)
{
    double best_cost = std::numeric_limits<double>::max();
    Individual best_individual(vehicles, depots, customers, distance_matrix);

    //vettore che contiene i mu parents
    std::vector<Individual> parents;
    parents.assign(mu, Individual(vehicles, depots, customers, distance_matrix));

    //inizializzazione dei parents
    for (Individual &p : parents)
    {
        p.random_inizialize();
        p.calculate_cost();
        if (p.get_cost() < best_cost)
        {
            best_cost = p.get_cost();

            best_individual = p;

            if ((unsigned)best_individual.get_cost() == known_solution)
            {
                return best_individual.get_cost();
            }
        }
    }

    //vettore che contiene i mu migliori figli
    std::vector<Individual> children;
    children.assign(mu, Individual(vehicles, depots, customers, distance_matrix));

    //puntatori che mi servono per invertire i parents con i children ad ogni generazione
    std::vector<Individual> *children_p = &children;
    std::vector<Individual> *parents_p = &parents;
    const unsigned remainder_children = lambda - mu;

    std::uniform_int_distribution<unsigned> random_mut(0, 3);
    std::uniform_int_distribution<unsigned> random_cross(0, 1);
    std::uniform_int_distribution<int> random_parent(0, mu - 1);

    unsigned score_mutators[] = {1, 1, 1, 1};
    double mprob = 1 / (double)4;
    double probabilities_mutators[] = {mprob, mprob, mprob, mprob};
    double total_good_mutation = 4;

    unsigned score_cross_overs[] = {1, 1, 1, 1, 1, 1};
    double cprob = 1 / (double)6;
    double probabilities_cross[] = {cprob, cprob, cprob, cprob, cprob, cprob};
    double total_good_cross = 6;

    unsigned g = 0;
    while (g < max_g)
    {
        const std::vector<Individual> &parents_a = *parents_p;
        std::vector<Individual> &children_a = *children_p;

        //generazione di mu figli: almeno mu figli dovranno essere parents
        for (unsigned i = 0; i < mu; i++)
        {
            //scelta casuale di un parent
            const int p = random_parent(mt);

            //genera un figlio
            children_a[i] = parents_a[p];

            unsigned p2 = random_parent(mt);
            while (p2 == p)
            {
                p2 = random_parent(mt);
            }

            std::discrete_distribution<unsigned> dist1{probabilities_cross[0], probabilities_cross[1], probabilities_cross[2], probabilities_cross[3], probabilities_cross[4], probabilities_cross[5]};
            const unsigned cr = dist1(mt);
            if (cr == 0)
            {
                //children_a[i].two_point_cross_over(parents_a[p], parents_a[p2]);
            }
            else if (cr == 1)
            {
                //children_a[i].best_order_cross_over(parents_a[p], parents_a[p2], best_individual);
            }
            else if (cr == 2)
            {
                //children_a[i].one_point_cross_over(parents_a[p], parents_a[p2]);
            }
            else if (cr == 3)
            {
                //children_a[i].uniform_cross_over(parents_a[p], parents_a[p2]);
            }
            else if (cr == 4)
            {
                //children_a[i].position_base_cross_over(parents_a[p], parents_a[p2]);
            }
            else if (cr == 5)
            {
                //no cross over
            }

            std::discrete_distribution<unsigned> dist{probabilities_mutators[0], probabilities_mutators[1], probabilities_mutators[2], probabilities_mutators[3]};
            const unsigned mut = dist(mt);
            if (mut == 0)
            {
                children_a[i].swap2();
            }
            else if (mut == 1)
            {
                children_a[i].swap3();
            }
            else if (mut == 2)
            {
                children_a[i].scrumble();
            }
            else if (mut == 3)
            {
                children_a[i].inversion();
            }
            else
            {
                //children_a[i].insertion();
            }

            children_a[i].calculate_cost();

            if (children_a[i].get_cost() < best_cost)
            {
                best_cost = children_a[i].get_cost();

                best_individual = children_a[i];

                score_cross_overs[cr]++;
                score_mutators[mut]++;
                total_good_mutation++;
                total_good_cross++;

                if ((unsigned)best_individual.get_cost() == known_solution)
                {
                    return best_individual.get_cost();
                }
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

            unsigned p2 = random_parent(mt);
            while (p2 == p)
            {
                p2 = random_parent(mt);
            }

            std::discrete_distribution<unsigned> dist1{probabilities_cross[0], probabilities_cross[1], probabilities_cross[2], probabilities_cross[3], probabilities_cross[4], probabilities_cross[5]};
            const unsigned cr = dist1(mt);
            if (cr == 0)
            {
                //child.two_point_cross_over(parents_a[p], parents_a[p2]);
            }
            else if (cr == 1)
            {
                //child.best_order_cross_over(parents_a[p], parents_a[p2], best_individual);
            }
            else if (cr == 2)
            {
                //child.one_point_cross_over(parents_a[p], parents_a[p2]);
            }
            else if (cr == 3)
            {
                //child.uniform_cross_over(parents_a[p], parents_a[p2]);
            }
            else if (cr == 4)
            {
                //child.position_base_cross_over(parents_a[p], parents_a[p2]);
            }
            else if (cr == 5)
            {
                //no cross over
            }

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

                    best_individual = child;

                    score_cross_overs[cr]++;
                    score_mutators[mut]++;
                    total_good_mutation++;
                    total_good_cross++;

                    if ((unsigned)best_individual.get_cost() == known_solution)
                    {
                        return best_individual.get_cost();
                    }
                }
            }
        }

        std::vector<Individual> *tmp = parents_p;
        parents_p = children_p;
        children_p = tmp;

        g++;

        //aggiorna le probabilità ad ogni generazione
        for (unsigned i = 0; i < 4; i++)
        {
            probabilities_mutators[i] = score_mutators[i] / total_good_mutation;
        }

        for (unsigned i = 0; i < 6; i++)
        {
            probabilities_cross[i] = score_cross_overs[i] / total_good_cross;
        }
    }
    return best_individual.get_cost();
}

#endif