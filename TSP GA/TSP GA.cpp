// TSP GA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <math.h>

struct Individ
{
	double id;
	double x;
	double y;
};

struct Path
{
	std::vector<Individ> single_path;
};


std::vector<Individ> read_input_file() {
	std::ifstream input_file;
	input_file.open("tsp.in", std::ios::in);
	double amout_of_numebrs;
	input_file >> amout_of_numebrs;
	std::vector<Individ> Points(amout_of_numebrs);
	for (double index = 0; index < Points.size(); ++index) {
		Points[index].id = index;
		double q;
		input_file >> q;
		input_file >> Points[index].x;
		input_file >> Points[index].y;
	}
	input_file.close();
	return Points;
}


double Pop_Size() {
	std::cout << "Populationsize (Integer): ";
	double val;
	std::cin >> val;
	return val;
}
double Number_Of_Iterations() {
	std::cout << "Numer of Iterations: ";
	double iterations;
	std::cin >> iterations;
	return iterations;
}
double Mutation() {
	std::cout << "Chance to mutate: ";
	double mutation_chance;
	std::cin >> mutation_chance;
	return mutation_chance;
}
double Mew_Individ() {
	std::cout << "Probability to add a new individ: ";
	double Individ_Prob;
	std::cin >> Individ_Prob;
	return Individ_Prob;
}
double Breed() {
	std::cout << "Chance to breed: ";
	double Chance_To_Breed;
	std::cin >> Chance_To_Breed;
	return Chance_To_Breed;
}



std::vector<Individ> Only_Intermediate_Nodes(std::vector<Individ>&Input_data) {
	std::vector<Individ>Intermediate;
	for (double index = 0; index < Input_data.size(); index++) {
		Intermediate.push_back(Input_data[index]);
	}
	return Intermediate;
}

Path Generate_path(std::vector<Individ>&Input_data) {
	Path p;
	std::vector<Individ>temp = Input_data;
	while (temp.size()) {
		double rand_val;
		rand_val = rand()%temp.size();
		p.single_path.push_back(temp[rand_val]);
		temp.erase(temp.begin() + rand_val );
	}
	return p;
	
}

std::vector<Path> Generate_First_Population(std::vector<Individ>&Input_data, double &Pop_Size) {
	std::vector<Path> Population(Pop_Size);
	for (double index = 0; index < Population.size(); ++index) {
		Population[index] = Generate_path(Input_data);
	}
	return Population;
}


double cost(Path p1, std::vector<std::vector<double>>cost_matrix) {
	double cost = 0;
	for (double index = 0; index < p1.single_path.size()-1; ++index) {
		cost += cost_matrix[p1.single_path[index].id][p1.single_path[index + 1].id];
	}
	return cost;
}


double find_index(Path &p1, double &id) {
	for (double index = 0; index < p1.single_path.size(); ++index) {
		if (id == p1.single_path[index].id)return index;
	}
}

bool inprove(Path &p1, std::vector<std::vector<double>>cost_matrix, std::vector<Individ>&Input_data) {
	double act_cost = 0;
	double new_cost = 0;
	std::vector<double>best_individs_save(2);
	std::vector<double>index_switch(2);
	act_cost = cost(p1, cost_matrix);
	for (double index = 0; index < p1.single_path.size(); ++index) {
		for (double index1 = 0; index1 < Input_data.size(); ++index1) {
			double found_index=find_index(p1, Input_data[index1].id);
			Individ save_ind = p1.single_path[index];
			p1.single_path[index] = Input_data[index1];
			p1.single_path[found_index] = save_ind;
			new_cost = cost(p1, cost_matrix);
			if (act_cost > new_cost) {
				return true;
			}
			else {
				save_ind = p1.single_path[found_index];
				p1.single_path[found_index] = p1.single_path[index];
				p1.single_path[index] = save_ind;
			}
		}
	}
	return false;
}

void breeed(Path &p1, Path &p2, std::vector<Individ>&Input_data) {
	double pos1 = rand() % Input_data.size();
	double pos2 = rand() % Input_data.size();
	if (pos1 > pos2) {
		pos1 = pos1 + pos2;
		pos2 = pos1 - pos2;
		pos1 = pos1 - pos2;
	}
	Path p1_1;
	Path p2_1;
	bool q = true;
	for (double index = 0; index < p1.single_path.size(); ++index) {
		if (index == pos1 || index == pos2)q = !q;
		if (q == true) {
			p1_1.single_path.push_back(p1.single_path[index]);
			p2_1.single_path.push_back(p2.single_path[index]);
		}
		else
		{
			p1_1.single_path.push_back(p2.single_path[index]);
			p2_1.single_path.push_back(p1.single_path[index]);
		}
	}
	p1 = p1_1;
	p2 = p2_1;
}
double rand_number(double l, double r) {
	double returned_number = rand() % (int)(r - l) + l;
	return returned_number;
}


double rand_real() {
	return((double)rand() / (RAND_MAX));
}


double find_dupe(Path p1, double id) {
	short counter = 0;
	for (double index = 0; index < p1.single_path.size();++index) {
		if (p1.single_path[index].id == id)counter++;
		if (counter == 2)return index;
	}
}
double find_unused(std::vector<double> vctr) {
	for (double index = 0; index < vctr.size(); ++index) {
		if (vctr[index] == 0)return index;
	}
}
std::vector<Path> breed2(Path p1, Path p2, std::vector<Individ>&Input_data) {
	std::vector<Path>s1(2);
	double point1 = rand() % Input_data.size();
	double point2 = rand() % Input_data.size();
	while (point1 == point2) {
		point2 = rand() % Input_data.size();
	}
	if (point2 < point1) {
		point1 = point1 + point2;
		point2 = point1 - point2;
		point1 = point1 - point2;
	}
	bool t = true; 
	for (double index = 0; index < p1.single_path.size(); index++) {
		if (point1 == index || point2 == index) {
			t = !t;
		}
		if (t == true) {
			s1[0].single_path.push_back(p1.single_path[index]);
			s1[1].single_path.push_back(p2.single_path[index]);
		}
		else
		{
			s1[0].single_path.push_back(p2.single_path[index]);
			s1[1].single_path.push_back(p1.single_path[index]);
		}
	}
	std::vector<double> c1(s1[0].single_path.size());
	std::vector<double> c2(s1[0].single_path.size());
	std::vector<Individ>c1_1 = Input_data;
	std::vector<Individ>c2_1 = Input_data;

	for (double index = 0; index < s1[0].single_path.size(); ++index) {
		c1[s1[0].single_path[index].id]++;
		c2[s1[1].single_path[index].id]++;
	}
	for (double index = 0; index < s1[0].single_path.size(); ++index) {
		if (c1[s1[0].single_path[index].id] == 2) {
			double unused_variable=find_unused(c1);
			c1[s1[0].single_path[index].id]--;
			s1[0].single_path[index] = Input_data[unused_variable];
			c1[unused_variable]++;
		}
		if (c2[s1[1].single_path[index].id] == 2) {
			double unused_variable = find_unused(c2);
			c2[s1[1].single_path[index].id]--;
			s1[1].single_path[index] = Input_data[unused_variable];
			c2[unused_variable]++;

		}
	}
	std::vector<double>o1(Input_data.size()	);
	std::vector<double>o2(Input_data.size() );
	for (double index = 0; index < o1.size(); index++) {
		o1[s1[0].single_path[index].id]++;
		o2[s1[1].single_path[index].id]++;
	}
	for (double index = 0; index < o1.size(); ++index) {
		if (o1[s1[0].single_path[index].id] != 1) { 
			std::cout << "ERROR HERE  in first"<<std::endl;
		}
		if (o2[s1[1].single_path[index].id] != 1) {
			std::cout << "ERROR HERE  in second"<<std::endl;
		}
	}
	return s1;
}
void evolve(std::vector<Path>&Population, double Iterations, double Chance_To_Mutate, double Chance_For_New_Individ, double Chance_To_Breed, std::vector<std::vector<double>>&cost_matrix, std::vector<Individ>&Input_data) {
	std::vector<double> found_indexes(Input_data.size());
	std::vector<Path> new_generation;
	std::vector<double>path_costs(Population.size());
	double total_cost = 0;
	for (double index = 0; index < path_costs.size(); ++index) {
		path_costs[index] = cost(Population[index], cost_matrix);
		total_cost += path_costs[index];
	}

	bool ok;
	for (double index = 0; index < Population.size()-1; index++) {
		ok = false;
		for (double index1 = 0; index1 < Population.size()-index-1; index1++) {
			if(path_costs[index1]>path_costs[index1+1]){
				double a;
				a = path_costs[index1];
				path_costs[index1] = path_costs[index1 + 1];
				path_costs[index1+ 1] = a;
				Path s;
				s = Population[index1];
				Population[index1] = Population[index1 + 1];
				Population[index1 + 1] = s;
				ok = true;
			}
		}
		if (ok == false)break;
	}

	std::vector<double>fitness(Population.size()+1);
	fitness[0] = 0;
	for (double index = 0; index < fitness.size()-1; ++index) {
		fitness[index+1] =fitness[index]+ path_costs[index] / total_cost;
	}
	std::vector<Path> new_population;
	while (new_population.size() < Population.size()) {
		if (new_population.size() == (Population.size() - 1)) {
			new_population.push_back(Generate_path(Input_data));
		}
		else {
			double random_nr = rand_real();
			if (random_nr < Chance_To_Breed) {
				double rand1 = rand_real();
				double rand2 = rand_real();
				int index1 = 0;
				int index2 = 0;
				while (fitness[index1] < rand1) {
					index1++;
				}
				while (fitness[index2] < rand2) {
					index2++;
				}
				index1--;
				index2--;
				std::vector<Path> o=breed2(Population[index1], Population[index2], Input_data);
				new_population.push_back(o[0]);
				new_population.push_back(o[1]);
			}
			else
			{
				new_population.push_back(Generate_path(Input_data));
			}
		}
	}
	Population = new_population;
	
}
void mutate(std::vector<Path>Population, double chance_to_mutate) {
	for (double index = 0; index < Population.size(); ++index) {
		double random_value =rand_real();
		if (random_value < chance_to_mutate) {
			double pos1 = rand() % Population[index].single_path.size();
			double pos2 = rand() % Population[index].single_path.size();
			while (pos1 == pos2) {
				pos2 = rand() % Population[index].single_path.size();
			}
			Path p1 = Population[pos1];
			Population[pos1] = Population[pos2];
			Population[pos2] = p1;
		}
	}
}
void Genetic_Alg(std::vector<Path>Population, double Iterations, double Chance_To_Mutate, double Chance_For_New_Individ, double Chance_To_Breed, std::vector<std::vector<double>>cost_matrix, std::vector<Individ>&Input_data) {
	double min_value = 0;
	double copy_generations = Iterations;
	Path save_best;
	std::ofstream output_file;
	output_file.open("tsp.out", std::ios::out | std::ios::trunc);
	output_file << "First Random Generation:" << std::endl;
	for (double index = 0; index < Population.size(); ++index) {
		double p_cost = cost(Population[index], cost_matrix);
		if (min_value == 0) { min_value = p_cost; }
		if (p_cost < min_value) {
			min_value == p_cost;
		}
		output_file << "Individ: " << index << std::endl;
		for (double index1 = 0; index1 < Population[index].single_path.size(); ++index1) {
			output_file << Population[index].single_path[index1].id << " ";
		}
		output_file << std::endl;
	}
	output_file << std::endl << "Min Value: " << min_value << std::endl << std::endl << std::endl;
	while (Iterations) {
		for (double index = 0; index < Population.size(); ++index) {
			double counter = 0;
			bool t = true;
			while (t&&counter<=15) {
				t = inprove(Population[index], cost_matrix, Input_data);
				counter++;
				
			}
		}
		evolve(Population, Iterations, Chance_To_Mutate, Chance_For_New_Individ, Chance_To_Breed, cost_matrix, Input_data);
		mutate(Population, Chance_To_Mutate);

		output_file << "Generation: " << copy_generations - Iterations << std::endl;
		for (double index1 = 0; index1 < Population.size(); ++index1) {
			output_file << "Individ: " << index1 << std::endl;
			double p_cost = cost(Population[index1], cost_matrix);
			if (p_cost < min_value) {
				min_value = p_cost;
				save_best = Population[index1];
			}
			for (double index2 = 0; index2 < Population[index1].single_path.size(); ++index2) {
				output_file << Population[index1].single_path[index2].id << " ";
			}
			output_file << std::endl;
		}

		output_file << "Min value: " << min_value << std::endl << std::endl;
		output_file << std::endl << std::endl;
		--Iterations;
	}
	output_file << "Best individ found:";
	for (double index = 0; index < save_best.single_path.size(); ++index) {
		output_file << save_best.single_path[index].id << " ";
	}
	output_file << std::endl;
	output_file << std::endl << std::endl << "Minimum value found: " << min_value;
	std::cout << "DONE";
	output_file.close();
}


std::vector<std::vector<double>> generate_matrix(std::vector<Individ>&Input_data) {
	std::vector<std::vector<double>>matrix(Input_data.size());
	for (double index1 = 0; index1 < Input_data.size(); index1++) {
		for (double index2 = 0; index2 < Input_data.size(); index2++) {
			matrix[index1].push_back(sqrt(pow( (Input_data[index2].x - Input_data[index1].x),2) + pow( (Input_data[index2].y - Input_data[index1].y),2)));
		}
	}
	return matrix;
}


// main to replace for keyboard input
int main2() {
	srand(time(0));
	std::vector<Individ>Input_data = read_input_file();
	double pop_size = Pop_Size();
	double iterations = Number_Of_Iterations();
	double Chance_To_Mutate = Mutation();
	double Chance_To_Breed = Breed();
	//double Chance_New_Individ = Mew_Individ();
	std::vector<Path>Population=Generate_First_Population(Input_data, pop_size);
	std::vector<std::vector<double>>cost_matrix = generate_matrix(Input_data);
	Genetic_Alg(Population, iterations, Chance_To_Mutate, 1-Chance_To_Breed, Chance_To_Breed, cost_matrix, Input_data);
	return 0;
}


int main3()
{
	srand(time(0));
	std::vector<Individ>Input_data=read_input_file();
	//double pop_size = Pop_Size();
	//double iterations = Number_Of_Iterations();
	//double Chance_To_Mutate = Mutation();
	//double Chance_To_Breed = Breed();
	//double Chance_New_Individ = Mew_Individ();
	//std::vector<Path>Population=Generate_First_Population(Input_data, pop_size);
	double q = 10;
	std::vector<Path>Population = Generate_First_Population(Input_data,q);
	std::vector<std::vector<double>>cost_matrix = generate_matrix(Input_data);
//	Genetic_Alg(Population, iterations, Chance_To_Mutate, Chance_New_Individ, Chance_To_Breed, cost_matrix, Input_data);
	Genetic_Alg(Population, 10, 0.3, 0.2, 0.8, cost_matrix, Input_data);
    return 0;
}

int main() {
	std::cout << "Chose 1 for your imput  or 2 for debugg/test input: ";
	double a;
	std::cin >> a;
	if ((int)a == 2) { main3; }
	if ((int)a == 1) { main2; }
}