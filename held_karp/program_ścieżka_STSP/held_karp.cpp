#include "algo.h"
#include "algorithm" // std::reverse
using std::vector;
Hk::Hk(){
}

Hk::~Hk(){
}
// Struktura pomocnicza w celu odnalezienia ścieżki
class Xi{
    public:
        int best;
        int xj;
        
        Xi(){
            best = -1;
            xj = -1;
        }
};
vector<int>  Hk::calculate(int vert, vector<vector<int>> &dist) {
    // (1)
    // tablica best_cost[S][xi] reprezentuje najlepszy koszt przejścia od wierzchołka 
    // x0 przez wszystkie wierzchołki w S, koncząc na xi, xi zawiera się w S
    // S to konkretny podzbiór wszystkich wierzchołków oprócz x0, podzbiory są kodowane jako
    // liczba binarna w której dany bit reprezentuje obecność danego wierzchołka
    // S = 2 ponieważ ustalamy sztywno pierwszy wierzchołek zerowy więc podzbiory nie zawierają 
    // x0 - pierwszego bitu w danym podzbiorze S
    // tablica zawiera Xi zamiast int w celu zapamietania poprzedniego wezła
    // Liczba (1<<vert)-2 reprezentuje zbiór wszystkich wierzchołków oprócz 0
    // + 1 ponieważ indeksowanie od 0
    vector<vector<Xi>> best_cost( (1<<vert)-2+1, vector<Xi>( vert, Xi()) );
    int S = 2;
    // (2)
    // Iteracja po podzbiorach zbioru wszystkich wierzchołków bez x0
    // w każdej iteracji dodajemy do S 2 = '10' bo pomijamy wierzchołek x0
    // konczymy na liczbie opisacej podzbior 2^vert-2 czyli to są wszystkie wierzchołi oprócz x0
    while(S <= (1<<vert)-2) {
        //(3)
        // Dla każdego wierzchołka znajdującego się w S znajdź podsciezke o najlepszym koszcie
        for (int xi = 1 ; xi < vert  ; xi++) {
            // (4)
            // Czy xi zawiera się w S ? Jeśli nie, weź następne xi
            if(!(S & (1<<xi))) 
                continue;
            // (5)
            // Kiedy S zawiera tylko xi nie ma możliwości wybrania podścieżki,
            //jest bezpośrednie połączenie
            if (S == (1 << xi) ){
                best_cost[S][xi].best = dist[0][xi];
                best_cost[S][xi].xj = 0;
                continue;  
            }
            // (6)
            // Znajdz wszystkie  xj - węzeł bezpośrednio przed xi,
            // x0 -> S\{xi,xj} -> xj -> xi 
            // zbiór S_minus_xi to zbór S bez wierzchołka xi - S\{xi}
            // dla takiego zbioru szukamy minimum kosztu w przejściu x0 -> S\{xi} -> xi w zależności od xj 
            int S_minus_xi = S ^ (1<<xi);
            for (int xj = 1; xj < vert ; xj++) {
                // (7)
                // xj musi się oczywiście zawierać w S_minus_xi
                if(!(S_minus_xi & 1<<xj))
                    continue;
                //(8)
                // Jeśli znaleziono lepszy wynik dla kolejnego xj, należy go zapisać
                if( best_cost[S][xi].best == -1){
                    best_cost[S][xi].best = best_cost[S_minus_xi][xj].best + dist[xj][xi];
                    best_cost[S][xi].xj = xj;
                }else{
                    int curr = best_cost[S][xi].best;
                    int next = best_cost[S_minus_xi][xj].best + dist[xj][xi];
                    if( next < curr){
                        best_cost[S][xi].best = next;
                        best_cost[S][xi].xj = xj;
                    }
                }
            }
        }
        S+=2; 
    }
    // (9)
    // Obliczenie kosztu najkrótszej ścieżki 
    // przyjeliśmy że zaczynamy w 0 co nie ma znaczenia, ale pozwoliło obliczyć koszty podzbioru 
    // maksymalnie n-1 elementowego a nie n elementowego
    // n elementowe (czyli kompletne cykle)
    // liczymy teraz i wybieramy najmiejszy
    int cost = -1;
    int xj_rem;
    for (int xj = 1; xj<vert; xj++) {
        int cur_cost = best_cost[(1<<vert)-2][xj].best + dist[xj][0];
        if (cost == -1){
            cost = cur_cost;
            xj_rem = xj;
        } else if(cur_cost < cost){
            cost = cur_cost;
            xj_rem = xj;
        }
    }
    // (10)
    // Odczytanie ścieżki
    int xj = xj_rem;
    vector<int> solution_and_cost = {0};
    S = (1<<vert)-2;
    solution_and_cost.push_back(xj);
    while(xj != 0){
        int xi = xj;
        xj = best_cost[S][xi].xj;
        solution_and_cost.push_back(xj);
        int Sn = S ^ (1 << xi);
        S = Sn;
    } 
    std::reverse(solution_and_cost.begin(), solution_and_cost.end());
    solution_and_cost.push_back(cost);
    
    return solution_and_cost;
}
/*
int  Hc::calculate(int vertices, vector<vector<int>> &dist) {
    //rozmiar 2^n - 2 - najwiekszy podzbior S czyli wszystkie wierzchołki oprócz 0 s\{x0}
    //+1 bo indeksowanie od 0 
int N = vertices;
vector< vector<int> > best_cost( (1<<N)-2+1, vector<int>( N, -1) ); 

//
// kodujemy S jako binarna liczbe - kazdy bit reprezentuje i-ty wierzchołek
// przechodzimy S - liczba reprezentujaca odwiedzane wierzchołki bez wierzchołka 0
// 0 jako wierchołek poczatkowy
//  s = 2 = "10" +2 "100" +2 "110" +2 "1000"+2 "1010"
for (int S = 2; S <= (1<<N)-2 ; S+=2) { 
    // dla kazdego xi zawierajacego sie w S
    for (int xi = 1 ;( xi < N) && (S & 1<<xi) ; ++xi) { 

        // xi musi nalezec do S
        // if (!(S & 1<<xi)) continue; DRUGI WARUNEK W FORZE

        // sprawdz wszystkie mozliwosc wyboru poprzedniego wierzchołka - xj
        // sciezka to 0 - > S\{xj,xi}-> xj -> xi 
        //  if ( zbiór S zawiera tylko xi)
        if (S == (1 << xi) )  
            best_cost[S][xi] = dist[0][xi]; 
    // xi  vertex must be N-1 vertex

    // XOR S z wierzchołiem xi zpowoduje przepisanie wszystkich
    // z S oprócz xi czyli XOR S z 1 na miejscu xi da tam 0
        int S_minus_xi = S ^ 1<<xi; 
        for (int xj = 1; (xj < N ) && (S_minus_xi & 1<<xj) ; xj++) { 
        // if (!(S_minus_xi & 1<<xj)) continue; // wierzchołek musi sie zawierac w S_minus_xi
            best_cost[S][xi] == -1 ? 
            best_cost[S_minus_xi][xj] + dist[xj][xi] :
            std::min( best_cost[S][xi]  ,  best_cost[S_minus_xi][xj] + dist[xj][xi] ); 
        } 

    } 
} 

int cost = -1; 
for (int xj=1; xj<N; xj++) { 
    int curr_xj = best_cost[(1<<N)-2+1][xj] + dist[xj][0];
    if (cost == -1)
        cost = curr_xj;
    else if (curr_xj < cost)
        cost = curr_xj;
}  
return cost;
}
*/
