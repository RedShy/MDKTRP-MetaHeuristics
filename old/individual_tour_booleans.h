#include <iostream>

bool ended_bruteforce(const int vehicles, const int depots, const int customers, const bool *const *const matrix)
{
	for (int v = 0; v < vehicles; v++)
	{
		if (!matrix[v][depots - 1])
		{
			return false;
		}
	}

	for (int c = 0; c < customers; c++)
	{
		if (!matrix[vehicles - 1][c+depots])
		{
			return false;
		}
	}

	return true;
}

void next_tick(bool **matrix, const int vehicles, const int depots, const int customers)
{
	bool next = false;
	for (int c = depots + customers - 1; c >= depots; c--)
	{
		next = false;
		if (matrix[vehicles - 1][c])
		{
			next = true;
		}

		for (int v = 0; v < vehicles; v++)
		{
			if (matrix[v][c])
			{
				matrix[v][c] = false;
				matrix[(v + 1) % vehicles][c] = true;
				break;
			}
		}

		if (!next)
			break;
	}

	if (next)
	{
		next = false;
		for (int v = vehicles - 1; v >= 0; v--)
		{
			next = false;
			if (matrix[v][depots - 1])
			{
				next = true;
			}

			for (int d = 0; d < depots; d++)
			{
				if (matrix[v][d])
				{
					matrix[v][d] = false;
					matrix[v][(d + 1) % depots] = true;
					break;
				}
			}

			if (!next)
				break;
		}
	}
}

int get_number_customers(const int v, const int columns, const bool *const *const tours)
{
	int vertices = 0;
	for (int i = 0; i < columns; i++)
	{
		if (tours[v][i])
			vertices++;
	}
	return vertices - 1;
}

double calculate_cost(const int vehicles, const int depots, const int customers, const bool *const *const tours, const double *const *const distances)
{
	double total_cost = 0;
	const int columns = depots + customers;
	//per ogni veicolo calcola il costo del suo tour
	for (int v = 0; v < vehicles; v++)
	{
		int n_customers = get_number_customers(v, columns, tours);

		//trova la posizione del primo depot
		int last = 0;
		for (int i = 0; i < columns; i++)
		{
			if (tours[v][i])
			{
				last = i;
				break;
			}
		}

		//calcola la latenza del tour
		for (int i = last + 1; i < columns; i++)
		{
			if (tours[v][i])
			{
				total_cost += n_customers * distances[last][i];

				last = i;
				n_customers--;
			}
		}
	}

	return total_cost;
}

void bruteforce(const int vehicles, const int depots, const int customers, const double *const *const distances)
{
	const int columns = depots + customers;
	bool **matrix = new bool *[vehicles];
	for (int v = 0; v < vehicles; v++)
	{
		matrix[v] = new bool[columns];
	}

	for (int v = 0; v < vehicles; v++)
	{
		for (int c = 0; c < columns; c++)
		{
			matrix[v][c] = false;
		}
	}

	for (int v = 0; v < vehicles; v++)
	{
		matrix[v][0] = true;
	}

	for (int c = 0; c < customers; c++)
	{
		matrix[0][c + depots] = true;
	}

	int i = 0;
	double cost = calculate_cost(vehicles, depots, customers, matrix, distances);
	while (!ended_bruteforce(vehicles, depots, customers, matrix))
	{
		next_tick(matrix, vehicles, depots, customers);

		double new_cost = calculate_cost(vehicles, depots, customers, matrix, distances);
		if (new_cost < cost)
		{
			cout << "Improved from " << cost << " to " << new_cost << "\n";
			cost = new_cost;
		}

		i++;
		if(i % 2000000 == 0)
		{
			cout<<i<<"\n";
			print_matrix(matrix,vehicles,depots+customers);
		}
	}

	cout<<"BEST BRUTEFORCE: "<<cost<<"\n";
}

class Individual
{
public:
	Individual(const int vehicles_, const int depots_, const int customers_, const double *const *const distance_matrix_) : vehicles(vehicles_), depots(depots_), customers(customers_), cost(0), distance_matrix(distance_matrix_)
	{
		tours = new bool *[vehicles];
		for (int i = 0; i < vehicles; i++)
		{
			tours[i] = new bool[depots + customers];
		}

		for (int i = 0; i < vehicles; i++)
		{
			for (int j = 0; j < depots + customers; j++)
			{
				tours[i][j] = false;
			}
		}
	}

	void random_inizialize()
	{
		for (int v = 0; v < vehicles; v++)
		{
			//seleziona un depot casualmente
			int depot = rand() % depots;
			tours[v][depot] = true;
		}

		//per ogni colonna seleziona casualmente una riga
		for (int c = depots; c < customers + depots; c++)
		{
			tours[rand() % vehicles][c] = true;
		}

		// tours[0][1] = true;
		// tours[0][depots+11] = true;
		// tours[0][depots+46] = true;
	}

	double calculate_cost()
	{
		const bool *const *const tours = this->tours;
		double total_cost = 0;
		const int columns = depots + customers;
		//per ogni veicolo calcola il costo del suo tour
		for (int v = 0; v < vehicles; v++)
		{
			int n_customers = get_number_customers(v);
			//std::cout<<"CUSTOMR: "<<n_customers<<"\n";

			//trova la posizione del primo depot
			int last = 0;
			for (int i = 0; i < columns; i++)
			{
				if (tours[v][i])
				{
					//std::cout<<"i="<<i<<"\n";
					last = i;
					break;
				}
			}

			//calcola la latenza del tour
			for (int i = last + 1; i < columns; i++)
			{
				//std::cout<<"cella: "<<tours[v][i]<<" ";
				if (tours[v][i])
				{
					//std::cout<<"SOMMO: "<<distance_matrix[last][i]<<" i="<<i<<"\n";
					total_cost += n_customers * distance_matrix[last][i];
					//total_cost += distance_matrix[last][i];

					last = i;
					n_customers--;
				}
			}

			//std::cout<<"COST: "<<total_cost<<"\n";
		}

		cost = total_cost;
	}

	void mutate()
	{
		for (int k = 0; k < 3; k++)
		{
			//seleziona un veicolo casualmente
			int v = rand() % vehicles;

			//seleziona un customer casualmente
			//int c = (rand() % customers) + depots;

			//seleziona una colonna casualmente
			int c = rand() % (customers + depots);

			//se hai beccato un 1, ripeti finché non trovi uno 0
			while (tours[v][c])
			{
				v = rand() % vehicles;
				c = rand() % (customers + depots);
			}

			tours[v][c] = true;
			//controlla se la colonna è un depot oppure un customer
			if (c < depots) //depot
			{
				//metti 0 l'uno sulla stessa riga
				for (int i = 0; i < depots; i++)
				{
					if (i != c && tours[v][i])
					{
						tours[v][i] = false;
						break;
					}
				}
			}
			else //customer
			{
				//metti 0 l'uno sulla stessa colonna
				for (int r = 0; r < vehicles; r++)
				{
					if (r != v && tours[r][c])
					{
						tours[r][c] = false;
						break;
					}
				}
			}
		}
	}

	double get_cost() const
	{
		return cost;
	}

	void print_tour_matrix() const
	{
		std::cout << "TOUR MATRIX\n";
		for (int i = 0; i < vehicles; i++)
		{
			for (int j = 0; j < depots + customers; j++)
			{
				std::cout << tours[i][j] << " ";
			}
			std::cout << "\n";
		}
	}

	Individual &operator=(const Individual &m)
	{
		if (this == &m)
			return *this;

		//TODO da migliorare molto
		vehicles = m.vehicles;
		customers = m.customers;
		depots = m.depots;
		cost = m.cost;
		tours = new bool *[vehicles];
		for (int i = 0; i < vehicles; i++)
		{
			tours[i] = new bool[depots + customers];
		}
		for (int i = 0; i < vehicles; i++)
		{
			for (int j = 0; j < depots + customers; j++)
			{
				tours[i][j] = m.tours[i][j];
			}
		}

		return *this;
	}

private:
	int vehicles;
	int customers;
	int depots;
	double cost;
	bool **tours;
	const double *const *const distance_matrix;

	int get_number_customers(const int v) const
	{
		int vertices = 0;
		const int columns = depots + customers;
		for (int i = 0; i < columns; i++)
		{
			if (tours[v][i])
				vertices++;
		}
		return vertices - 1;
	}
};