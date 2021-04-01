#ifndef ALGO_H
#define ALGO_H

#include<bits/stdc++.h>
void tsp_bf(int vertices, std::vector<std::vector<int>> &distances, std::vector<int> &solution_and_cost);

class Bf
{
    private:
        int best;                                                       // koszt aktualnie najlepszej scieżki
        std::vector<int> solution_and_cost;
        std::vector<std::vector<int>> distances; 
    public:
        Bf();
        std::vector<int> calculate(int vertices, std::vector<std::vector<int>> &dist);
        void tsp_start(std::vector<int> choose, std::vector<int> path);
        ~Bf();
};

#endif
