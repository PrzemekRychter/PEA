#include "algo.h"
#include "algorithm" 
#include <cstdlib>
#include <stdlib.h> 
#include <ctime>
#include <cmath>
#include <random>
using std::vector;
// stale T,xo, war zatrzymania. zbadam alfa, L,
vector<int> Sa::calculate(int vert, vector<vector<int>> &dist) {
    
    double alfa = 0.99;
    int L = (double) ((vert*(vert-1))/2) * (double) 1; // (double) 0.0025 * (double)vert;/
    int bez_poprawy = 100;	//100
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<double> num(0.0, 1.0);
    std::uniform_int_distribution<int> node(1, vert-1);
   
		
    double temp = calc_init_temp(vert, dist);
    //double temp = temp0;
    //std::cout << "T0: " << temp << std::endl;	//delete
    vector<int> current_path;
    for( int k=0; k < vert; k++)
	    current_path.push_back(k);
    current_path.push_back(0);
    
    std::shuffle(current_path.begin()+1 ,current_path.end()-1 ,g);    
    int curr_cost = calc_cost(current_path, dist);
    
    vector<int> best_path(current_path);
    int best_cost = curr_cost;
    int k = 0;
    int war = 1;
    int old_cur_cost = -1;    
    //int epoch = 0;	//delete
    while( war < bez_poprawy){
     	//epoch++;		//delete
    	for(int t = 0; t < L; t++){
	        
            vector<int> sasiad(current_path);
	        int a = node(rd); int b = node(rd);
	        while (a == b){
	            a = node(rd);
	        }
	        //znalezienie sasiada
	        std::iter_swap(sasiad.begin() + a , sasiad.begin() + b);
	        //swap(sasiad, a, b);
	        int sasiad_cost = calc_cost(sasiad, dist);
	        
            if(sasiad_cost <= curr_cost){
	            curr_cost = sasiad_cost;
	            current_path = sasiad;
	            if(sasiad_cost <= best_cost){
	    	    	best_cost = sasiad_cost;
	    	   	best_path = sasiad;
	     	    }
	        } else {
                if( std::exp(((double (curr_cost - sasiad_cost))/temp)) > num(g)){
	                curr_cost = sasiad_cost;
	                current_path = sasiad;
	    	    }
	        }

    	}
	    if(old_cur_cost != -1){
	        if(old_cur_cost == curr_cost)
	            war++;
	        else
	        	war=1;    
	    } 
    	old_cur_cost = curr_cost;
    	k++;
    	//temp = alfa * temp;// temp0/(1 +log10((double) k));/
    	temp = (temp / (1 + log10(k)));
    }
    //std::cout << "Epochs: " << epoch << std::endl; //delete
    //std::cout << "Tk: " << temp << std::endl;     //delete
    vector<int> solution_and_cost(best_path);
    solution_and_cost.push_back(best_cost);
    return solution_and_cost;
}

int Sa::calc_cost(std::vector<int> &path, vector<vector<int>> &dist){
    int cost = 0;
    for(int i =0; i < path.size()-1; i++)
        cost += dist[path[i]][path[i+1]];
    return cost;
}

double Sa::calc_init_temp(int vert, vector<vector<int>> &dist){
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> d(1, vert-1);

    vector<int> path; 
    for( int k=0; k < vert; k++)
	    path.push_back(k);
    path.push_back(0);
    
    int delta = 0;
    int N = 10000;
    for(int i = 0; i < N; i++){
        std::shuffle(path.begin()+1 ,path.end()-1, g);
	    int cost = calc_cost(path, dist);
	    int a = d(rd); int b = d(rd);
	        while (a == b){
	            a = d(rd);
	        }
	    std::iter_swap(path.begin() + a, path.begin() + b);
	    int cost2 = calc_cost(path, dist);
        delta += std::abs(cost - cost2);
    }
    delta /= N;
    double temp =  -( (double) delta / (double) std::log(0.99) ); //0.99
	return temp;
}

void Sa::swap(vector<int> &sasiad, int a, int b){
	if(a>b){
		int t = b;
		b = a;
		a = t;
	}
	while(a < b){
	int te = sasiad[b];
	sasiad[b] = sasiad[a];
	sasiad[a] = te;
	b--; a++;
	}
}
Sa::Sa(){}  // konstruktor
Sa::~Sa(){} // desktruktor

