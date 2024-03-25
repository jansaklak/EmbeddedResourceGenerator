
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
    // std::cout << "Czy wypisywać wyniki na ekran? 0/1: ";
    // std::cin >> to_screen;
    to_screen = 0;
    while(running){
        std::cout << "Podaj liczbe:\n0 - utworz losowa liste:\n1 - Wczytaj z pliku: \n2 - uruchom zadania:\n5 - wykonaj zad. 5:\n9 - zakoncz program:\n";
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
        else if(menu == 5){
            Cost_List lista20;
            lista20.Load_From_File("graph20.dat");
            lista20.TaskDistribution(1);
            lista20.RunTasks();
            
            
        }
        else if(menu == 9){
            running = 0;
        }
    }
    return 0;
    
}
