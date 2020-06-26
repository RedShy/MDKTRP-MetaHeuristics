#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "individual.h"
using namespace std;

void read_first_line_cordeau(string file, int &depots, int &vehicles, int &customers)
{
	ifstream instance;
	instance.open(file);

	string line;
	getline(instance, line);
	stringstream ss(line);
	string s;
	int i = 0;
	while (getline(ss, s, ' '))
	{
		if (i == 1)
		{
			vehicles = stoi(s);
		}
		else if (i == 2)
		{
			customers = stoi(s);
		}
		else if (i == 3)
		{
			depots = stoi(s);
		}
		i++;
	}

	instance.close();
}

void load_coordinate_matrix(string file, int **matrix, const int depots, const int customers)
{
	ifstream instance;
	instance.open(file);

	string line;
	//skippa le prime depots+1 righe
	for (int i = 0; i < depots + 1; i++)
	{
		getline(instance, line);
	}

	for (int i = 0; i < depots + customers; i++)
	{
		getline(instance, line);
		stringstream ss(line);
		string s;
		int j = 0;
		while (getline(ss, s, ' '))
		{
			if (j == 1)
			{
				matrix[i][0] = stoi(s);
			}
			else if (j == 2)
			{
				matrix[i][1] = stoi(s);
			}
			j++;
		}
	}

	instance.close();
}

void swap_depots(int **matrix, const int depots, const int customers)
{
	for (int i = 0; i < depots; i++)
	{
		int *tmp = matrix[0 + i];
		matrix[0 + i] = matrix[customers + i];
		matrix[customers + i] = tmp;
	}
}

void print_matrix(auto **matrix, int R, int C)
{
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}
}

double euclidean_distance(const int x1, const int y1, const int x2, const int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double rad(double d)
{
	return d * M_PI / 180.0;
}

double get_distance(double lon1, double lat1, double lon2, double lat2)
{
	double radLat1 = rad(lat1);
	double radLat2 = rad(lat2);
	double a = radLat1 - radLat2;
	double b = rad(lon1) - rad(lon2);
	double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
	s = s * 6378.137;
	return s;
}

void calculate_distance_matrix(double **distance_matrix, const int *const *const coordinate_matrix, const int depots, const int customers)
{
	for (int i = 0; i < depots + customers; i++)
	{
		for (int j = 0; j < depots + customers; j++)
		{
			distance_matrix[i][j] = euclidean_distance(coordinate_matrix[i][0], coordinate_matrix[i][1],
													   coordinate_matrix[j][0], coordinate_matrix[j][1]);
			//distance_matrix[i][j] = get_distance(coordinate_matrix[i][0], coordinate_matrix[i][1],
			//										   coordinate_matrix[j][0], coordinate_matrix[j][1]);

			// if (i == 1 && j == 11)
			// {
			// 	cout << "x1 =" << coordinate_matrix[i][0] << " y1 = " << coordinate_matrix[i][1] << " x2=" << coordinate_matrix[j][0] << " y2=" << coordinate_matrix[j][1] << "\n";
			// 	cout << "distanza: " << distance_matrix[i][j] << "\n";
			// }
		}
	}
}

int **allocate_coordinate_matrix(const int rows, const int columns)
{
	int **matrix = new int *[rows];
	for (int i = 0; i < rows; i++)
	{
		matrix[i] = new int[columns];
	}
	return matrix;
}

void read_file_cordeau(string file, int &depots, int &vehicles, int &customers, int **&coordinate_matrix)
{
	read_first_line_cordeau(file, depots, vehicles, customers);

	coordinate_matrix = allocate_coordinate_matrix(depots + customers, 2);

	load_coordinate_matrix(file, coordinate_matrix, depots, customers);

	//mi interessa che i depots siano nelle prime righe della matrice e non alla fine
	swap_depots(coordinate_matrix, depots, customers);

	print_matrix(coordinate_matrix, depots + customers, 2);
}

void read_file_ruiz(string file, int &depots, int &customers, int **&coordinate_matrix)
{
	ifstream instance;
	instance.open(file);

	string line;
	getline(instance, line);
	customers = stoi(line);

	getline(instance, line);
	depots = stoi(line);

	getline(instance, line);
	getline(instance, line);

	coordinate_matrix = allocate_coordinate_matrix(depots + customers, 2);

	int **customers_matrix = allocate_coordinate_matrix(customers, 2);
	for (int c = 0; c < customers; c++)
	{
		getline(instance, line);
		stringstream ss(line);
		string s;
		int j = 0;
		while (getline(ss, s, ' '))
		{
			if (j == 0)
			{
				customers_matrix[c][0] = stoi(s);
			}
			else if (j == 1)
			{
				customers_matrix[c][1] = stoi(s);
			}
			j++;
		}
	}

	int **depots_matrix = allocate_coordinate_matrix(depots, 2);
	for (int d = 0; d < depots; d++)
	{
		getline(instance, line);
		stringstream ss(line);
		string s;
		int j = 0;
		while (getline(ss, s, ' '))
		{
			if (j == 0)
			{
				depots_matrix[d][0] = stoi(s);
			}
			else if (j == 1)
			{
				depots_matrix[d][1] = stoi(s);
			}
			j++;
		}
	}

	for (int d = 0; d < depots; d++)
	{
		coordinate_matrix[d][0] = depots_matrix[d][0];
		coordinate_matrix[d][1] = depots_matrix[d][1];
	}

	for (int c = 0; c < customers; c++)
	{
		coordinate_matrix[c + depots][0] = customers_matrix[c][0];
		coordinate_matrix[c + depots][1] = customers_matrix[c][1];
	}

	for (int d = 0; d < depots; d++)
	{
		delete[] depots_matrix[d];
	}
	delete[] depots_matrix;

	for (int c = 0; c < customers; c++)
	{
		delete[] customers_matrix[c];
	}
	delete[] customers_matrix;

	instance.close();
}

void test(int depots, int customers)
{
	// 	int N = depots + customers;
	// for (int i = 0; i < N; i++)
	// {
	// 	for (int j = 0; j < N; j++)
	// 	{
	// 		for (int k = 0; k < N; k++)
	// 		{
	// 			double d = 0;
	// 			d += 2 * euclidean_distance(coordinate_matrix[i][0], coordinate_matrix[i][1], coordinate_matrix[j][0], coordinate_matrix[j][1]);
	// 			d += euclidean_distance(coordinate_matrix[j][0], coordinate_matrix[j][1], coordinate_matrix[k][0], coordinate_matrix[k][1]);
	// 			if (d >= 23.49 && d <= 23.51)
	// 			{
	// 				cout << "TROVATA: " << d << "\n";
	// 			}
	// 		}
	// 	}
	// }
}

//RANDOM STUFF
std::random_device rd;
std::mt19937 mt(rd());

double test2(const int depots, const int customers, const double *const *const distance_matrix, const int N)
{
	Individual parent(5, depots, customers, distance_matrix, mt);
	parent.random_inizialize();
	parent.calculate_cost();
	//parent.print_tour_matrix();
	//cout << "Initial cost " << parent.get_cost() << "\n";
	const int generations = 31250 * N;
	for (int g = 0; g < generations; g++)
	{
		Individual child(parent);
		child.swap2();
		child.calculate_cost();
		if (child.get_cost() < parent.get_cost())
		{
			//cout << "Improved from " << parent.get_cost() << " to " << child.get_cost() << "\n";

			//TODO assegnamento costoso da modificare
			parent = child;
			//parent.print_tour_matrix();
		}
	}

	return parent.get_cost();
}

int main()
{
	// mi serve:
	int depots = 0;
	int vehicles = 0;
	int customers = 0;
	int **coordinate_matrix;
	string file = "./dat/25x4-1.txt";
	//read_file_cordeau(file, depots, vehicles, customers, coordinate_matrix);
	read_file_ruiz(file, depots, customers, coordinate_matrix);

	cout << "DATI INIZIALI: " << depots << " " << vehicles << " " << customers << "\n";
	print_matrix(coordinate_matrix, depots + customers, 2);

	//matrice che contiene le distanze da ogni nodo ad ogni altro nodo
	double **distance_matrix = new double *[depots + customers];
	for (int i = 0; i < depots + customers; i++)
	{
		distance_matrix[i] = new double[depots + customers];
	}

	calculate_distance_matrix(distance_matrix, coordinate_matrix, depots, customers);

	//cout<<"BRUTEFORCE\n";
	//bruteforce(5,depots,customers,distance_matrix);

	// Individual parent(5, depots, customers, distance_matrix, mt);
	// parent.random_inizialize();
	// parent.calculate_cost();
	// parent.print_tour_matrix();
	// cout << "Initial cost " << parent.get_cost() << "\n";

	// Individual best_parent(parent);
	// const int generations = 50000000;
	// for (int g = 0; g < generations; g++)
	// {
	// 	Individual child(parent);
	// 	child.mutate();
	// 	child.calculate_cost();
	// 	if (child.get_cost() < best_parent.get_cost())
	// 	{
	// 		cout << "Improved from " << best_parent.get_cost() << " to " << child.get_cost() << "\n";

	// 		//TODO assegnamento costoso da modificare
	// 		parent = child;
	// 		best_parent = child;
	// 		best_parent.print_tour_matrix();
	// 	}
	// 	if (g % 1000000 == 0)
	// 	{
	// 		cout << "generation: " << g << "\n";
	// 		//parent.print_tour_matrix();
	// 		parent.random_inizialize();
	// 		parent.calculate_cost();
	// 		if (parent.get_cost() < best_parent.get_cost())
	// 		{
	// 			cout << "Improved from " << best_parent.get_cost() << " to " << parent.get_cost() << "\n";

	// 			//TODO assegnamento costoso da modificare
	// 			best_parent = parent;
	// 			best_parent.print_tour_matrix();
	// 		}
	// 	}
	// }

	// best_parent.print_tour_matrix();
	// cout << "best cost: " << best_parent.get_cost() << "\n";
	double best = 99999;
	for (int i = 0; i < 16000; i++)
	{
		double result = test2(depots, customers, distance_matrix, 1);
		if (result < best)
		{
			best = result;
			cout << "BEST: " << best << "\n";
		}
	}
	cout << "BEST: " << best << "\n";
	return 0;
}