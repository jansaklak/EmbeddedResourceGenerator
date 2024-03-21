
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

Cost_List losowo(){
    
    int tasks, hc, pu, costs, ch;
    bool toScreen;

    std::cout << "Podaj liczbe zadan: ";
    std::cin >> tasks;
    std::cout << "Podaj liczbe Hardware Cores: ";
    std::cin >> hc;
    std::cout << "Podaj liczbe Processing Units: ";
    std::cin >> pu;
    std::cout << "Czy z kosztem? 0/1: ";
    std::cin >> costs;
    std::cout << "Podaj liczbe szyn danych: ";
    std::cin >> ch;
    std::cout << "Czy wyswietic na ekranie? (0/1): ";
    std::cin >> toScreen;
    Cost_List lista_kosztow = Cost_List(tasks,hc,pu,ch,costs,toScreen);
    lista_kosztow.RandALL();
    lista_kosztow.PrintALL("output.dat");
    return lista_kosztow;
}



int main(){
    int menu;
    srand (time(NULL));
    std::cout << "Aby stworzyć losową tabelę wcisnij 0: ";
    std::cout << "Aby wczytać z pliku wcisnij 1: ";
    std::cin >> menu;
    if(menu == 0){
        Cost_List lista = losowo();
    }
    if(menu == 1){
        Cost_List lista;
        lista.Load_From_File("input.dat");
        lista.PrintALL("input_test.dat");
    }

    std::cout << "Zapisano do pliku tasks.dat\n";

}
