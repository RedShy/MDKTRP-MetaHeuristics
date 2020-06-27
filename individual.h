#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>
#include <algorithm>
#include <random>
#include <set>
#include <chrono>

//RANDOM STUFF
std::random_device rd;
std::mt19937 mt(rd());

class Individual
{
public:
	Individual(const unsigned vehicles_, const unsigned depots_, const unsigned customers_, const double *const *const distance_matrix_, std::mt19937 &mt_) : vehicles(vehicles_), depots(depots_), customers(customers_), cost(0), distance_matrix(distance_matrix_), mt(mt_)
	{
		tours = new unsigned[vehicles + customers];
		depot_positions = new unsigned[vehicles];
	}

	Individual(const Individual &o) : vehicles(o.vehicles), depots(o.depots), customers(o.customers), cost(o.cost), distance_matrix(o.distance_matrix), mt(o.mt)
	{
		tours = new unsigned[vehicles + customers];
		depot_positions = new unsigned[vehicles];
		for (unsigned i = 0; i < vehicles + customers; i++)
		{
			tours[i] = o.tours[i];
		}

		for (unsigned v = 0; v < vehicles; v++)
		{
			depot_positions[v] = o.depot_positions[v];
		}
	}

	void random_inizialize()
	{
		std::uniform_int_distribution<unsigned> dist(0, depots - 1);
		unsigned *const tours = this->tours;

		//piazza casualmente i depots per i veicoli
		for (unsigned v = 0; v < vehicles; v++)
		{
			tours[v] = dist(mt);
		}

		//piazza i customer in modo sequenziale
		for (unsigned c = 0; c < customers; c++)
		{
			tours[c + vehicles] = c + depots;
		}

		std::shuffle(&tours[0], &tours[vehicles + customers], mt);

		//se il primo elemento è un customer swappalo con il primo depot che trovi
		if (tours[0] >= depots)
		{
			for (int i = 0; i < vehicles + customers; i++)
			{
				if (tours[i] < depots)
				{
					const unsigned tmp = tours[0];
					tours[0] = tours[i];
					tours[i] = tmp;
					break;
				}
			}
		}

		//indica le posizioni dei vari depot
		unsigned v = 0;
		for (unsigned i = 0; i < vehicles + customers; i++)
		{
			if (tours[i] < depots)
			{
				//start_routes_positions.insert(i);
				depot_positions[v] = i;
				v++;
			}
		}
	}

	void swap2()
	{
		std::uniform_int_distribution<unsigned> random_cell(0, vehicles + customers - 1);
		std::uniform_int_distribution<unsigned> random_depot(0, depots - 1);
		unsigned *const tours = this->tours;

		//scegli la prima cella da swappare
		const unsigned first = random_cell(mt);
		if (tours[first] < depots)
		{
			//scegli casualmente un nuovo depot
			tours[first] = random_depot(mt);
		}
		else
		{
			//scegli una seconda cella
			unsigned second = random_cell(mt);
			//la seconda cella non può essere uguale 0 e né uguale a first
			while (second == 0 || second == first)
			{
				second = random_cell(mt);
			}

			//se il secondo è un depot aggiorna la posizione dei depot
			if (tours[second] < depots)
			{
				unsigned i = 1;
				while (depot_positions[i] != second)
					i++;
				depot_positions[i] = first;

				//elimino la posizione precedente
				//start_routes_positions.erase(second);

				//inserisco la nuova posizione
				//start_routes_positions.insert(first);
			}

			//swap tra le due celle
			const unsigned tmp = tours[first];
			tours[first] = tours[second];
			tours[second] = tmp;
		}
	}

	void swap3()
	{
		std::uniform_int_distribution<unsigned> random_cell(0, vehicles + customers - 1);
		std::uniform_int_distribution<unsigned> random_depot(0, depots - 1);
		unsigned *const tours = this->tours;

		// std::shuffle(positions,positions+vehicles+customers-1,mt);
		// const unsigned first = positions[0];
		// const unsigned second = positions[1];
		// const unsigned third = positions[2];

		//scegli la prima cella da swappare
		const unsigned first = random_cell(mt);
		if (tours[first] < depots)
		{
			//scegli casualmente un nuovo depot
			tours[first] = random_depot(mt);
		}
		else
		{
			//scegli una seconda cella
			unsigned second = random_cell(mt);
			//la seconda cella non può essere uguale 0 e né uguale a first
			while (second == 0 || second == first)
			{
				second = random_cell(mt);
			}

			//scegli una terza cella
			unsigned third = random_cell(mt);
			//la terza cella non può essere uguale 0, né uguale a first, né a second
			while (third == 0 || third == first || third == second)
			{
				third = random_cell(mt);
			}

			//se il secondo è un depot aggiorna la posizione dei depot
			if (tours[second] < depots)
			{
				unsigned i = 1;
				while (depot_positions[i] != second)
				{
					i++;
				}
				depot_positions[i] = first;
			}

			//se il terzo è un depot aggiorna la posizione dei depot
			if (tours[third] < depots)
			{
				unsigned i = 1;
				while (depot_positions[i] != third)
					i++;
				depot_positions[i] = second;
			}

			//swap tra le tre celle
			const unsigned tmp3 = tours[third];
			tours[third]=tours[first];
			tours[first] = tours[second];
			tours[second] = tmp3;
		}
	}

	void scrumble()
	{
		std::uniform_int_distribution<unsigned> random_cell(0, vehicles + customers - 1);
		std::uniform_int_distribution<unsigned> random_depot(0, depots - 1);
		unsigned *const tours = this->tours;

		//scegli la prima cella
		unsigned first = random_cell(mt);
		if (tours[first] < depots)
		{
			//scegli casualmente un nuovo depot
			tours[first] = random_depot(mt);
		}
		else
		{
			while (first == vehicles + customers - 1 || first == 0)
			{
				first = random_cell(mt);
			}

			//scegli una seconda cella maggiore della prima
			unsigned second = random_cell(mt);
			while (second <= first)
			{
				second = random_cell(mt);
			}
			std::shuffle(&tours[first], &tours[second + 1], mt);

			//aggiorno la posizione degli altri depot
			unsigned i = first;
			for (unsigned v = 1; v < vehicles; v++)
			{
				//controllo se il depot è nel range dello scrumble
				if (depot_positions[v] >= first && depot_positions[v] <= second)
				{
					//cerco il depot all'interno dello scrumble
					while (tours[i] >= depots)
					{
						i++;
					}
					depot_positions[v] = i;
					i++;
				}
			}
		}
	}

	void calculate_cost()
	{
		const unsigned *const tours = this->tours;
		//const int *const depot_positions = this->depot_positions;
		// const std::set<unsigned> &start_routes_positions_c = start_routes_positions;

		// //cout << "ARRAY DEPOTS\n";

		// double sum = 0;

		// auto it = start_routes_positions_c.cbegin();
		// auto next_it = it;
		// ++next_it;
		// auto end = start_routes_positions_c.cend();
		// --end;
		// //calcola la latenza per le rotte tranne l'ultima
		// for (; it != end; ++it, ++next_it)
		// {
		// 	//cout<<*it<<" ";
		// 	unsigned first = *it;
		// 	unsigned second = *next_it;

		// 	int len = second - first - 1;
		// 	int last = first;
		// 	first++;
		// 	for (; first < second; first++)
		// 	{
		// 		sum += distance_matrix[tours[last]][tours[first]] * len;

		// 		len--;
		// 		last = first;
		// 	}
		// }
		// //cout<<"\n";

		// //calcolo la latenza per l'ultima rotta
		// const int N = vehicles + customers;
		// unsigned first = *it;
		// unsigned second = N;
		// int len = second - first - 1;
		// int last = first;
		// first++;
		// for (; first < second; first++)
		// {
		// 	sum += distance_matrix[tours[last]][tours[first]] * len;

		// 	len--;
		// 	last = first;
		// }

		std::sort(depot_positions, depot_positions + vehicles);

		const unsigned *const depot_positions = this->depot_positions;

		double sum = 0;
		const unsigned N = vehicles + customers;
		const unsigned vehicles_n = vehicles - 1;
		for (unsigned v = 0; v < vehicles_n; v++)
		{
			unsigned first = depot_positions[v];
			const unsigned second = depot_positions[v + 1];

			unsigned len = second - first - 1;

			unsigned last = first;
			first++;
			for (; first < second; first++)
			{
				sum += distance_matrix[tours[last]][tours[first]] * len;

				len--;
				last = first;
			}
		}

		unsigned first = depot_positions[vehicles_n];
		unsigned len = N - first - 1;
		unsigned last = first;
		first++;
		for (; first < N; first++)
		{
			sum += distance_matrix[tours[last]][tours[first]] * len;

			len--;
			last = first;
		}

		cost = sum;
		//cout << "COSTO: " << cost << "\n";
	}

	double get_cost() const
	{
		return cost;
	}

	void print_tour_matrix() const
	{
		std::cout << "TOUR ARRAY\n";
		for (unsigned i = 0; i < vehicles + customers; i++)
		{
			std::cout << tours[i] << " ";
		}
		std::cout << "\n";
	}

	Individual &operator=(const Individual &m)
	{
		if (this == &m)
			return *this;

		cost = m.cost;
		for (unsigned i = 0; i < vehicles + customers; i++)
		{
			tours[i] = m.tours[i];
		}

		for (unsigned v = 0; v < vehicles; v++)
		{
			depot_positions[v] = m.depot_positions[v];
		}

		//start_routes_positions = m.start_routes_positions;

		return *this;
	}

	bool operator<(const Individual &o) const
	{
		return cost < o.cost;
	}

	~Individual()
	{
		delete[] tours;
		delete[] depot_positions;
	}

private:
	const unsigned vehicles;
	const unsigned customers;
	const unsigned depots;
	double cost;
	unsigned *tours;
	const double *const *const distance_matrix;
	// è un array di n_veicoli elementi che indica dove si trovano i depot associati ai veicoli all'interno del cromosoma
	unsigned *depot_positions;
	//set che indica le posizioni di partenza in ordine crescente delle rotte all'interno del cromosoma
	//std::set<unsigned> start_routes_positions;
	std::mt19937 &mt;
};

#endif