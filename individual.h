#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

//RANDOM STUFF
std::random_device rd;
std::mt19937 mt(rd());

class Individual
{
public:
	Individual(const int vehicles_, const int depots_, const int customers_, const double *const *const distance_matrix_, std::mt19937 &mt_) : vehicles(vehicles_), depots(depots_), customers(customers_), cost(0), distance_matrix(distance_matrix_), mt(mt_)
	{
		tours = new int[vehicles + customers];
		depot_positions = new int[vehicles];
	}

	Individual(const Individual &o) : vehicles(o.vehicles), depots(o.depots), customers(o.customers), cost(o.cost), distance_matrix(o.distance_matrix), mt(o.mt)
	{
		tours = new int[vehicles + customers];
		depot_positions = new int[vehicles];
		for (int i = 0; i < vehicles + customers; i++)
		{
			tours[i] = o.tours[i];
		}

		for (int v = 0; v < vehicles; v++)
		{
			depot_positions[v] = o.depot_positions[v];
		}
	}

	void random_inizialize()
	{
		std::uniform_int_distribution<int> dist(0, depots - 1);
		int *const tours = this->tours;

		//piazza casualmente i depots per i veicoli
		for (int v = 0; v < vehicles; v++)
		{
			tours[v] = dist(mt);
		}

		//piazza i customer in modo sequenziale
		for (int c = 0; c < customers; c++)
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
					const int tmp = tours[0];
					tours[0] = tours[i];
					tours[i] = tmp;
					break;
				}
			}
		}

		//indica le posizioni dei vari depot
		int v = 0;
		for (int i = 0; i < vehicles + customers; i++)
		{
			if (tours[i] < depots)
			{
				depot_positions[v] = i;
				v++;
			}
		}
	}

	void calculate_cost()
	{
		const int *const tours = this->tours;
		const int *const depot_positions = this->depot_positions;

		double sum = 0;
		const int N = vehicles + customers;
		for (int v = 0; v < vehicles; v++)
		{
			int first = depot_positions[v];
			int second;
			if (v == (vehicles - 1))
			{
				second = N;
			}
			else
			{
				second = depot_positions[v + 1];
			}
			int len = second - first - 1;

			int last = first;
			first++;
			for (; first < second; first++)
			{
				sum += distance_matrix[tours[last]][tours[first]] * len;

				len--;
				last = first;
			}
		}

		cost = sum;
	}

	void mutate()
	{
		std::uniform_int_distribution<int> random_cell(0, vehicles + customers - 1);
		std::uniform_int_distribution<int> random_depot(0, depots - 1);
		int *const tours = this->tours;

		//scegli la prima cella da swappare
		const int first = random_cell(mt);
		if (tours[first] < depots)
		{
			//scegli casualmente un nuovo depot
			tours[first] = random_depot(mt);
		}
		else
		{
			//scegli una seconda cella
			int second = random_cell(mt);
			while (second == 0)
			{
				second = random_cell(mt);
			}

			//se il secondo è un depot aggiorna la posizione dei depot
			if (tours[second] < depots)
			{
				//testare con std::set per vedere se migliora le prestazioni

				//ricerca lineare con sentinella
				int i = 1;
				while (depot_positions[i] != second)
					i++;
				depot_positions[i] = first;
			}

			//swap tra le due celle
			const int tmp = tours[first];
			tours[first] = tours[second];
			tours[second] = tmp;
		}
	}

	double get_cost() const
	{
		return cost;
	}

	void print_tour_matrix() const
	{
		std::cout << "TOUR ARRAY\n";
		for (int i = 0; i < vehicles + customers; i++)
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
		for (int i = 0; i < vehicles + customers; i++)
		{
			tours[i] = m.tours[i];
		}

		for (int v = 0; v < vehicles; v++)
		{
			depot_positions[v] = m.depot_positions[v];
		}

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
	const int vehicles;
	const int customers;
	const int depots;
	double cost;
	int *tours;
	const double *const *const distance_matrix;
	// è un array che indica dove si trovano i depot all'interno del cromosoma
	int *depot_positions;
	std::mt19937 &mt;
};

#endif