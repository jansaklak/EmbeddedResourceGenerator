
#include <iostream>
#include <random>
#include <functional>
#include <set>
#include <vector>
#include <cmath>

#include "COM.h"
#include "Graf.h"
#include "Hardware.h"
#include "Times.h"
#include "Cost_List.h"

extern const int SCALE = 400;

Cost_List generateRandomCostList(){
    int tasks_amount, hardware_cores_amount, processing_units_amount,channels_amount;
    int to_screen,with_cost;

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
    
    Cost_List lista_kosztow = Cost_List(tasks_amount,hardware_cores_amount,processing_units_amount,channels_amount,with_cost);
    lista_kosztow.RandALL();
    return lista_kosztow;
}

int main(){
    int menu,to_screen;
    int running = 1;
    srand (time(NULL));
    std::cout << "Czy wypisywać wyniki na ekran? 0/1: ";
    std::cin >> to_screen;
    while(running){
        std::cout << "Aby stworzyć losową tabelę wcisnij 0: \nAby wczytać z pliku wcisnij 1: \n Wcisnij 2 aby uruchomić zadania na liscie\nWcisnij 9 aby zakonczyc";
        std::cin >> menu;
        
        if(menu == 0){
            Cost_List lista = generateRandomCostList();
            lista.PrintALL("output.dat",to_screen);
            
            
        }
        else if(menu == 1){
            Cost_List lista1;
            lista1.Load_From_File("output.dat");
            lista1.PrintALL("input_test.dat",to_screen);
            int strategy;
            std::cout << "Wybierz podział zadań:\n 1-najszybciej 2-najtaniej 3-rownomiernie ";
            std::cin >> strategy;
            lista1.TaskDistribution(strategy);
            lista1.RunTasks();
            // currList = lista;
        }
        else if(menu == 2){
            
            
            
        }
        else if(menu == 9){
            running = 0;
        }
    }
    return 0;
    
}
