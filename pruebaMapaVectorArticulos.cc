#include "busca.hh"
#include "./librerias/articulo.hh"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

int main() {
   
    map<set<Articulo>,int> mapa;
    
    set<Articulo> set1 = {Articulo(1, 3, 2, 2, 2), Articulo(2, 4, 3, 2, 1)};
    set<Articulo> set2 = {Articulo(1, 3, 3, 2, 2), Articulo(2, 4, 2, 2, 1)};


    mapa[set1] = 69;

    if(mapa[set2] == 69){
        cout << "OLEEEEE" << endl;
    } else {
        cout << "buuu" << endl;
    }

    return 0;

    
}



