#ifndef ALGO_H
#define ALGO_H

#include<bits/stdc++.h>

class Bf
{
    private:
        int best;                                                       // koszt aktualnie najlepszej scieżki
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

#endif


