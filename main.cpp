#include <iostream>
#include <random>
#include <functional>
#include <set>
#include <vector>
#include <cmath>
#include <filesystem>

#include "COM.h"
#include "Graf.h"
#include "Hardware.h"
#include "Times.h"
#include "Cost_List.h"

extern const int SCALE = 400;

namespace fs = std::filesystem;

Cost_List generateRandomCostList(){
    int tasks_amount, hardware_cores_amount, processing_units_amount, channels_amount;
    int to_screen, with_cost;

    std::cout << "Podaj liczbe zadan: ";
    std::cin >> tasks_amount;
    std::cout << "Podaj liczbe Hardware Cores: ";
    std::cin >> hardware_cores_amount;
    std::cout << "Podaj liczbe Processing Units: ";
    std::cin >> processing_units_amount;
    std::cout << "Podaj liczbe szyn danych: ";
    std::cin >> channels_amount;
    std::cout << "Czy z kosztem? 0/1: ";
    std::cin >> with_cost;
    if(with_cost != 0){
        with_cost = 1;
    }
    Cost_List lista_kosztow = Cost_List(tasks_amount, hardware_cores_amount, processing_units_amount, channels_amount, with_cost);
    lista_kosztow.randALL();
    return lista_kosztow;
}

int main(){
    int menu, to_screen;
    int running = 1;
    int file_loaded = 0;
    int sim;
    srand(time(NULL));
    to_screen = 0;
    std::string file_name;
    Cost_List lista = Cost_List();
    while(running){
        if(file_loaded){
            std::cout << "\tWczytano plik: " << file_name << "\n0 - Reset\n1 - Uruchom Zadania\n9 - Zakoncz program\n\t->";
            std::cin >> menu;
            switch(menu){
                case 0:
                    file_loaded = 0;
                    lista = Cost_List();
                    break;
                case 1:
                    int strategy;
                    std::cout << "Wybierz podział zadań:\n 1-najszybciej 2-najtaniej 3-rownomiernie\n\t->";
                    std::cin >> strategy;
                    lista.taskDistribution(strategy);
                    std::cout << "Czy przeprowadzić symulacje? 0/1\n\t->";
                    std::cin >> sim;
                    if(sim) lista.runTasks();
                    break;
                case 9:
                    running = 0;
                    break;
            }
        }
        else{
            std::cout << "Nacisnij 0 aby stworzyć nowy plik\nNacisnij 1 aby wczytać plik\nNacisnij 5 aby wykonać zad. 5\nNacisnij 8 aby otworzyć projekt\n\t->";
            std::cin >> menu;
            switch(menu){
                case 0:
                    lista = generateRandomCostList();
                    std::cout << "Podaj nazwe pliku do zapisu:\n\t->";
                    std::cin >> file_name;
                    file_name += ".temp";
                    lista.printALL("data/" + file_name, to_screen);
                    file_loaded = 1;
                    break;
                case 1:
                    for (const auto& entry : fs::directory_iterator("data")) {
                        std::string filename = entry.path().filename().string();
                        filename = filename.substr(0, filename.size() - 4);
                        std::cout << "   >" << filename << '\n';
                    }

                    std::cout << "Podaj nazwe pliku do wczytania:\n\t->";
                    std::cin >> file_name;
                    if( lista.Load_From_File("data/" + file_name) != 1 ||
                        lista.Load_From_File("data/" + file_name + ".temp") != 1 ||
                        lista.Load_From_File("data/" + file_name + ".dat") != 1){
                            std::cerr << "Błąd\n";
                            break;}
                    
                    
                    lista.printALL("data/input_test.dat", to_screen);
                    file_loaded = 1;
                    break;
                case 5:
                    lista.Load_From_File("data/graph20.dat");
                    lista.taskDistribution(60);
                    std::cout << "Czy przeprowadzić symulacje? 0/1\n\t->";
                    std::cin >> sim;
                    if(sim) lista.runTasks();
                    file_loaded = 1;
                    break;
                case 8:
                    lista.Load_From_File("data/projekt.dat");
                    lista.taskDistribution(8);
                    std::cout << "Czy przeprowadzić symulacje? 0/1\n\t->";
                    std::cin >> sim;
                    if(sim) lista.runTasks();
                    file_loaded = 1;
                    break;
                case 9:
                    running = 0;
                    break;
            }
        }
    }
    return 0;
}
