#include "algo.h"
#include "algorithm" 
#include <cstdlib>
#include <stdlib.h> 
#include <ctime>
#include <cmath>
#include <random>
using std::vector;

vector<int> Sa::calculate(int vert, vector<vector<int>> &dist) {
   
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<double> num(0.0, 1.0);
    std::uniform_int_distribution<int> node(1, vert-1);
    
    int L = vert*vert;	
    double temp = calc_init_temp(vert, dist);
    
    vector<int> current_path;
    for( int k=0; k < vert; k++)
	    current_path.push_back(k);
    current_path.push_back(0);
    
    std::shuffle(current_path.begin()+1 ,current_path.end()-1 ,g);    
    int curr_cost = calc_cost(current_path, dist);
    
    vector<int> best_path(current_path);
    int best_cost = curr_cost;
    
    int war = 0;
    int old_cur_cost = -1;
    
    while(war < 9){
    
    	for(int t = 0; t < L; t++){
	        
            vector<int> sasiad(current_path);
	        int a = node(rd); int b = node(rd);
	        while (a == b){
	            a = node(rd);
	        }
	        std::iter_swap(sasiad.begin() + a , sasiad.begin() + b);
	        int sasiad_cost = calc_cost(sasiad, dist);
	        
            if(sasiad_cost <= best_cost){
	    	    best_cost = sasiad_cost;
	    	    best_path = sasiad;
	        }
	        
            if(sasiad_cost <= curr_cost){
	            curr_cost = sasiad_cost;
	            current_path = sasiad;
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
	        	war=0;    
	    } 
    	old_cur_cost = curr_cost;
    	temp = 0.98 * temp;
    }
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
	    std::iter_swap(path.begin() + d(rd), path.begin() + d(rd));
	    int cost2 = calc_cost(path, dist);
        delta += std::abs(cost - cost2);
    }
    delta /= N;
    double temp =  -( (double) delta / (double) std::log(0.99) );
	return temp;
}

Sa::Sa(){}  // konstruktor
Sa::~Sa(){} // desktruktor


