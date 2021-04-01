#include "algo.h"
#include "algorithm" 
#include <cstdlib>
#include <stdlib.h> 
#include <ctime>
#include <cmath>
#include <random>
using namespace std;

vector<int> Aco::calculate(int vert, vector<vector<int>> &dist) {
   
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<double> num(0.0, 1.0);
    std::uniform_int_distribution<int> node(0, vert-1);
    vector<int> best_path;
    int best_cost = -1;  
     int bez_poprawy = 10;
   double alpha = 1.0;
   double beta = 2.5;
   double evaporation = 0.5; // ro
   
   int Cnn = calc_init_Cnn(vert,dist);
   double init_pher = (double) vert / (double) Cnn; // poczatkowa ilosc pheromonow
   int iterations = 100; // zmienic na bez porpawy
   double pher = 100.0; // ilosc pheromonu zostawianego przez mrowke
   // macierz feromonow
   vector<vector<double>> pheromones;
   for(int i = 0; i < vert; i++){
   	vector<double> ph;
   	for(int k=0; k < vert; k++)
   		ph.push_back(init_pher);
   	pheromones.push_back(ph);
   }
  
   vector<vector<double>> pheromones_to_add;
   for(int i = 0; i < vert; i++){
   	vector<double> ph;
   	for(int k=0; k < vert; k++)
   		ph.push_back(0.0);
   	pheromones_to_add.push_back(ph);
   }
   int i = 0;
   int war = 1;
   int old_cur_cost,curr_cost = -1; 
   while(war < bez_poprawy){
	// znalezienie m cyklów przez m mrówek   
  	for( int m = 0; m < vert; m++){ 
  		// dla mrowki losowany jest wierzchołek startowy
  		int start_node = m;
  		vector<int> path;
  		path.push_back(start_node);   // sciezka danej mrowki
  		vector<int> left_vertices;	// lista wierzchołków mozliwych do wybrania
  		
  		for(int y = 0; y < vert; y++){ // uzupelnienie listy wszystkimi mozliwymi
  			if(y != start_node)
  				left_vertices.push_back(y);
  		}
		// szukanie sciezki
  		for(int x = 0; x < vert-1 ; x++){
  			int cn = path.back();
  			//std::cout << cn << " " << std::endl;
  			//obliczenie prawopodobienst wybrania pozostalych wierzcholkow 
  			double sum_factor = 0.0; // dolne wyrazenie w obliczaniu prawdo
  			for(vector<int>::iterator it =left_vertices.begin(); it != left_vertices.end() ; it++) 
				sum_factor += (pow(pheromones[cn][*it],alpha))*(pow((double) 1.0/(double)dist[cn][*it],beta));
			
  			vector<double> probabil;

  			for(vector<int>::iterator ir =left_vertices.begin(); ir != left_vertices.end() ; ir++) {
  				double p = ((pow(pheromones[cn][*ir],alpha))*(pow((double) 1.0/(double)dist[cn][*ir],beta)))/sum_factor;	
  				probabil.push_back(p);
  			}
	
			// Roulette-Wheel-Selection
  			partial_sum(probabil.begin(),probabil.end(),probabil.begin());

  			double p = num(g); // wylosowanie liczby z przedzialu [0,1]
  			
  			int k = 0;
  			
  			for(vector<double>::iterator ix = probabil.begin(); ix != probabil.end() ; ix++){ 
  				
  				if(p < *ix){
  					// wybrano nastepny wierzcholek
  					int wybrany = left_vertices[k];
  					path.push_back(wybrany);
  					left_vertices.erase(left_vertices.begin()+k); 
  					// usuniecie z dostepnych wierzchołków
  					break;
  				}
  				k++;
  			}

  		}
  		path.push_back(start_node);
		// Obliczenie kosztu znalezionego cyklu
  		int L = calc_cost(path,dist);
  		
  		if( L < best_cost || best_cost == -1){
  			best_cost = L;
  			best_path = path;
  		}
  		// obliczenie ile feromonow dodac
  		double ph = pher/(double) L;
  		// dodanie pheromonow tylko na sciezka przez ktore mrowka szla
  		for(int r =0; r < path.size()-1; r++)
        		pheromones_to_add[path[r]][path[r+1]] += ph;
  
  	}
  	// Uaktualnienie ilosc feromonow na kazdej krawedzi
  	for(int i = 0; i < vert; i++){
   		for(int k=0; k < vert; k++){
   			pheromones[i][k] =  (pheromones[i][k] * evaporation) + pheromones_to_add[i][k];
   			pheromones_to_add[i][k] = 0.0;
   		}

  	 }
  	curr_cost = best_cost;
  	if(old_cur_cost != -1){
	        if(old_cur_cost == curr_cost)
	            war++;
	        else
	        	war=1;    
	    } 
    	old_cur_cost = curr_cost;
  	
 }

    best_path.pop_back();
    vector<int>::iterator itt = find(best_path.begin(), best_path.end(), 0);
    std::rotate(best_path.begin(),itt,best_path.end());
    best_path.push_back(0);
    
    vector<int> solution_and_cost(best_path);
    solution_and_cost.push_back(best_cost);
       
    return solution_and_cost;
}

int Aco::calc_cost(std::vector<int> &path, vector<vector<int>> &dist){
    int cost = 0;
    for(int i =0; i < path.size()-1; i++)
        cost += dist[path[i]][path[i+1]];
    return cost;
}

Aco::Aco(){}  // konstruktor
Aco::~Aco(){} // desktruktor

int Aco::calc_init_Cnn(int vert, vector<vector<int>> &dist){
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> d(1, vert-1);

    vector<int> path; 
    for( int k=0; k < vert; k++)
	    path.push_back(k);
    path.push_back(0);
    
    int sum = 0;
    int N = 10000;
    for(int i = 0; i < N; i++){
        std::shuffle(path.begin()+1 ,path.end()-1, g);
	sum += calc_cost(path, dist);
    }
    sum = (double) sum / (double) N;
    return sum;
}


