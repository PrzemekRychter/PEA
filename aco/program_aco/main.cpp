// UWAGA PROGRAM OCZEKUJE DANYCH BEZ PODANEJ SCIEZKI OPTYMALNEJ W FORMACIE:
// nazwa_instancji ilość_powtórzeń KOSZT [sciezka]
// Sciezka moze byc optymalnie podana ale nie musi
// nie bedzie ona i tak sprawdzana, program sprawdza w kazdym powtorzeniu tylko
// czy koszt zgadza sie z podanym
//
#include "algo.h"
#define FILE_IN "conf.ini"

// DWIE FUKNKCJE odpowiedzialne za sprawdzenie ilosci pamieci RAM 
// aktualnie uzywanej przez program
int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

int main(){
    
    std::ifstream file_in;
    file_in.open(FILE_IN);                                              // otwarcie pliku "conf.ini"
    if(!file_in.is_open()){
        std::cout << " File OPEN error - FILE_IN" << std::endl;
    }
    std::ofstream file_out("invalid_result.csv");                               // otwarcie pliku "result.csv" - tymczasowa nazwa dopóki nie zczytamy ostatniej lini z "conf.ini"
    std::string instance_info;              
    std::vector<double> bledyy;
    while(getline(file_in, instance_info) ){ 
        
        std::string instance_name, reps_s, best_cost_s, vertex_s;       // wczytanie nazwy instancji, ilości powtórzeń, kosztu najlepszej ścieżki, najlepszej ścieżki            
                                                                        // elementy powinny być rozdzielone POJEDYNCZĄ spacją
                                                                        // za ostatnim elementem w pliku "conf.ini" może być jedna spacja lub nic
        int reps, best_cost;                                            // w pliku "conf.ini" optymalna scieżka powinna zaczynać się i konczyć węzłem "0"
        std::vector<int> best_path;
        
        bool read_name, read_reps, read_cost, read_best_path;
        read_name = read_reps = read_cost = read_best_path = true;
        
        for(int i = 0; i < instance_info.size();i++ ){
            
            if(!isspace(instance_info[i])){
                if(read_name)                                               instance_name.push_back(instance_info[i]);
                else if(read_reps)                                          reps_s.push_back(instance_info[i]);
                else if(read_cost)                                          best_cost_s.push_back(instance_info[i]);
          //      else if(read_best_path)                                     vertex_s.push_back(instance_info[i]);
          //      if(i == instance_info.size()-1 && instance_info[i] != 'v')  best_path.push_back(std::stoi(vertex_s));
            }
            else if(read_name)        read_name = false;
            else if(read_reps)      { read_reps = false; 
            
            if(instance_name[instance_name.size()-1] != 'v')
            	reps = std::stoi(reps_s); }
            else if(read_cost)      { read_cost = false;}
         //   else if(read_best_path) { best_path.push_back(std::stoi(vertex_s));vertex_s.clear(); }
        }                                                               // dane instancji lub nazwa pliku csv zostały wczytane
        if(instance_name[instance_name.size()-1] == 'v'){               // ostatnia linia z "conf.ini" czyli nazwa pliku csv jest wpisywana na końcu tego pliku
            file_out << instance_name << std::endl;                     // ostatnia linia jest wtedy gdy pierwsze słowo z lini konczy się na 'v'
            if(std::rename("invalid_result.csv", instance_name.c_str()))
                    std::cout << "ERROR renaming" << std::endl;
        } else{                                                         // jesli to nie ostatnia linia to mamy instancje dla której należy znaleźć optymalną ścieżkę
            if(instance_name[instance_name.size()-1] != 'v')
            	best_cost = std::stoi(best_cost_s);             
                     
            std::ifstream instance_file("./instancje/" + instance_name);// otwarcie pliku z instancja
            if(!(instance_file.is_open())){
                std::cout << "Instance could not be opened. Check if  all instances are in \"Instancje\" file " << std::endl;         
                break;
            }
                int vertices;
            instance_file >> vertices;                                  // wczytanie ilosci miast/wierzchołków/węzłów
            
            std::vector<std::vector<int>> distances;    
            for(int i = 0; i < vertices; i++){                          // wczytanie macierzy kosztu przebycia odległości pomiędzy miastami
                std::vector<int> row;
                for(int j = 0; j < vertices; j++){
                    int k;
                    instance_file >> k;
                    row.push_back(k);
                }
                distances.push_back(row);
            }
                                                                        // Wykonanie n powtórzeń algorytmu szukania optymalnego cyklu
            int rep = 1;
            std::cout << instance_name << "                     nodes: " << vertices << ::std::endl;
            int sr = 0;
            double bledy = 0.0;
            while(rep++ <=  reps){
                std::vector<int> solution_cost;
                
                Aco aco = Aco();
                
                auto start = std::chrono::high_resolution_clock::now(); 
                
                solution_cost = aco.calculate(vertices, distances);   

                auto stop = std::chrono::high_resolution_clock::now();
                aco.~Aco();
               
               
                // wpisanie do pliku csv danych instancji w celu określenia dla jakiej instancji będą podane czasy 
                // podanie obliczonego kosztu oraz sciezki optymalnej dla 1 powtorzenia
                if( rep == 2){
                    std::string ret_path = "";
                    for(int i = 0; i < vertices+1 ; i++){
                        ret_path += (std::to_string(solution_cost[i]) + " ");
                    }
                    file_out << instance_name + " Reps: " << reps << " Nodes:  " << vertices << std::endl; 
                }
                
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                file_out << duration.count() << std::endl;              // wpisanie czasu do pliku csv
                /*
                bool path = true;                                       
                for(int i = 0; i <= vertices; i++){                     // sprawdzenie czy ścieżka zgadza się z optymalną dla instancji danej  podaną w pliku conf.ini  
                    if(solution_cost[i] != best_path[i])
                        path = false;
                }
                
                if(path == false){                                      // jeśli algorytm wyznaczył inną scieżkę podajemy ją
                    std::cout << "Attention! diffrent path for instance: " << instance_name << " at repetition: " << rep-1 <<  std::endl;
                    std::cout << "Path: ";
                    for(int i = 0 ; i < solution_cost.size()-1 ; i++)
                        std::cout << solution_cost[i] << " "; 
                    std::cout << std::endl;
                }
                */
                if(solution_cost[vertices+1] != best_cost){             // jeśli algorytm wyznaczył inny koszt podajemy go

                    std::cout << "blad: " << ((double)(solution_cost[vertices+1]-best_cost )/(double) best_cost)*100.0 << " %" << std::endl;
                } else{
                    std::cout << "koszt optymalny" << std::endl;
                }      
          
                sr++;
                bledy += ((double)(solution_cost[vertices+1]-best_cost )/(double) best_cost)*100.0;
            }
            std::cout << "sredni blad (w %) dla aktualnej instancji ponizej " << std::endl;
            std::cout << bledy/sr << std::endl;
            bledyy.push_back(bledy/sr);
            
            std::cout << "__________________________________________________" << std::endl;
            instance_file.close();
        }           
    }
    std::cout << "SREDNIE BLEDY w % DLA KOLEJNO WSZYSTKICH INSTACJI Z conf.ini" << std::endl;
    for(int z = 0; z < bledyy.size(); z++){
    	std::cout << bledyy[z] << std::endl;
    }
    file_out.close();
    file_in.close();
}
