#include "algo.h"

Bf::Bf(){
}

// funkcja pierwsza
std::vector<int> Bf::calculate(int vertices, std::vector<std::vector<int>> &dist){
            // (1)
            // Wywołanie pierwszej funkcji z argumentami: vertices – ilość wierchołków, dist[][] macierz odległości między miastami. 
            // Zmienna best, wspólna dla wszystki wywołań rekurencyjnych jest inicjalizowana wartością 1, będzie ona przechowywać koszt aktualnie najlepszej drogi. 
            // Inicjalizacja zmiennej path[] czyli aktualnie tworzona droga, wybieramy wierzchołek 0-owy jako pierwszy. 
            // Inicjalizacja zmiennej choose[] jest to tablica z której możemy wybrać następny wierchołek w celu stworzenia drogi.
            
            best = -1;                                                  
            distances = dist;
            
            std::vector<int> path{0};                                          
            std::vector<int> choose;                                        
                                                                
            for(int i = 1; i <vertices; i++)   
                choose.push_back(i);
            // (2)
 	    // Kopie zmiennych path[] oraz choose[]  czyli c_path[] oraz c_choose[] 
 	    // są przekazywane do drugiej funkcji – rekurencyjnej. c_path – current_path, c_choose – current_choose
            tsp_start(choose, path);                                 
            // (7)
            //Jest to moment w którym druga funkcja rekurencyjna( i wszystkie jej podwywołania) została/zostały zakończone i dalej będzie wykonywana  funkcja pierwsza
            
            // (8)
            // Funkcja pierwsza zwraca  znalezioną optymalną ścieżkę w postaci 0, a, b, … , 0 i na koniec do tablicy zapisuje jej koszt.
            solution_and_cost.push_back(0);                          
            solution_and_cost.push_back(best);                       
            return solution_and_cost;
}
// funckja druga
void Bf::tsp_start(std::vector<int> c_choose, std::vector<int> c_path){ // Funkcja szukająca potymalnej ścieżki rekurene
	    // (3)
	    // Jesli tablica c_choose[] jest pusta oznacza to że cykl jest kompetny i można przejść do obliczenia kosztu i sprawdzenia czy jest optymalny.	
            if(c_choose.size() == 0){
            	 // (5)
                //Cykl jest kompletny zostały wykorzystane wszystkie wierzchołki, więc liczony jest koszt cyklu, 
                // czyli w pętli są sumowane koszty połączeń między kolejnymi wierchołkami w cyklu.
                // Operator % został zastosowany w celu obliczenia kosztu z ostatniego węzła do 0-owego. 
                int cost = 0;
                for(int i = 0; i < c_path.size() ; i++)
                    cost += distances[c_path[i] ][ c_path[(i+1)%c_path.size()] ];   // modulo w celu znalezienia ceny z ostatniego wezla do 0
                // (6)
                // Jeśli aktualnie najlepszy koszt jest < 0 (czyli jest to pierwszy znaleziony cykl) lub wyliczony koszt jest mniejszy od aktualnie najlepszego, 
                // to obliczony koszt zapisujemy/nadpisujemy jako najlepszy we wspólnej dla wszystkich wywołań rekurencyjnych zmiennej best, 
                // a  aktualną scieżkę zapisujemy/nadpisujemy (również we wspólnej dla wszystkich wywołań rekurencyjnych) zmiennej solution&cost.
                if(best < 0 || cost < best){
                    best = cost;
                    solution_and_cost = c_path;
                }
            }
 
           //chose.size() != 0
           // (4)
           // Jeśli tablica c_choose[] zawiera wierchołki do stworzenia cyklu, to w pętli dla każdego wierchołka z c_choose jest tworzona kopia aktualnej trasy (path), 
           // dodawany jest do niej koleny wierzchołek z c_choose, a następnie funkcja wywołuje samą siebie z nową trasą (path) oraz nową tablicą c_choose (choose) 
           // bez wierchołka który został dodany (skoro został dodany już do trasy to nie chcemy żeby się powtarzał)
           for(int i = 0; i < c_choose.size(); i++){
                
                std::vector<int> path = c_path;                       // new_path - scieżka z jednym z kolejnych węzłów
                path.push_back(c_choose[i]);                      // dodanie kolejnego wezla do sciezki z mozliwych do wyboru
                
                std::vector<int> choose = c_choose;                   // new_chose - węzły możliwe do wyboru - węzeł własnie wybrany
                choose.erase(choose.begin()+i);         // usuniecie tego wezla z tablicy wyboru
                
                tsp_start(choose, path);
            }
            return;
}

// Destruktor
Bf::~Bf(){
   // std::cout << "Obiekt zniszczony" << std::endl;
}

