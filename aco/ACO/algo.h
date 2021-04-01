#ifndef ALGO_H
#define ALGO_H

#include<bits/stdc++.h>
using std::vector;
class Bf
{
    private:
        int best;                  // koszt aktualnie najlepszej scieżki
        std::vector<int> solution_and_cost;
        std::vector<std::vector<int>> distances; 
        void tsp_start(std::vector<int> choose, std::vector<int> path);
    public:
        Bf();
        std::vector<int> calculate(int vertices, std::vector<std::vector<int>> &dist);
        ~Bf();
};

class Hk
{
    public: 
        Hk();
        ~Hk();
        std::vector<int> calculate(int vert, std::vector<std::vector<int>> &dist);
};

class Sa
{
    public:
        Sa();
        ~Sa();
        std::vector<int> calculate(int vert, std::vector<std::vector<int>> &dist);
    private:  
    	double calc_init_temp(int vert, vector<vector<int>> &dist);
    	int calc_cost(std::vector<int>& path, vector<vector<int>> &dist);
};

class Aco
{
	public:
		Aco();
		~Aco();
		 std::vector<int> calculate(int vert, std::vector<std::vector<int>> &dist);
	private:
		int calc_cost(std::vector<int>& path, vector<vector<int>> &dist);
		int calc_init_Cnn(int vert, vector<vector<int>> &dist);
};

#endif


