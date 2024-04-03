#include "busca.hh"
#include "./librerias/articulo.hh"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

bool hay_interseccion_entre_pareja_articulos(Articulo a, Articulo b){
    return  (a.x < b.x + b.ancho &&
            a.x + a.ancho > b.x &&
            a.y < b.y + b.alto &&
            a.y + a.alto > b.y);
}

// ================================================================================================================================================= //
// ================================================================================================================================================= //

int maximizeArea(vector<Articulo>& articulos, int i, int area_total, unordered_map<int, int>& memo) {
    if (i == articulos.size()) return area_total;
    int area_included = 0;
    bool overlap = false;

    // Itera sobre todos los artículos almacenados en la memoria (aquellos que ya se han añadido a la página)
    for (const auto& [idx, area] : memo) {
        // Para no comparar el mismo artículo
        if (i == idx) continue; 

        // Se comprueba si hay intersección entre el articulo a añadir y los que están ya en la página
        cout << "Compruebo entre: Articulo " << i << " y el Articulo " << idx << endl;
        if (hay_interseccion_entre_pareja_articulos(articulos[i-1], articulos[idx-1])) {
            overlap = true;
            cout << "Hay interseccion entre: Articulo " << i << " y el Articulo " << idx << endl;
            break;
        }
    }

    // Si no hay superposición de artículos, se añade el area a area_total
    if (!overlap) {
        area_included = maximizeArea(articulos, i + 1, area_total + articulos[i].area, memo);
    }

    // Si hay superposición de artículos, se continua sin tener en cuenta el área del artículo actual
    int area_excluded = maximizeArea(articulos, i + 1, area_total, memo);

    // Se guarda el máximo resultante de haber añadido el artículo o sin haberlo añadido.
    memo[i] = max(area_included, area_excluded);
    
    return memo[i];
}

int main() {
    // Articulo = {id, x, y, anchura, altura}
    vector<Articulo> articulos = {Articulo(1, 3, 2, 2, 2), Articulo(2, 4, 3, 2, 1), Articulo(3, 2, 2, 1, 1), Articulo(4, 3, 4, 1, 3)};
    vector<Articulo> articulos_prueba = {Articulo(1, 10, 10, 0, 0), Articulo(2, 10, 10, 15, 15), Articulo(3, 10, 10, 10, 10), Articulo(4, 20, 10, 20, 20), Articulo(5, 20, 10, 25, 15)};
    int anchura_pagina = 7;
    int altura_pagina = 7;
    
    unordered_map<int, int> memo1, memo2;
    cout << "Area total ocupada: " << maximizeArea(articulos, 0, 0, memo1) << endl;
    //cout << "Area total ocupada: " << maximizeArea(articulos_prueba, 0, 0, memo2) << endl;
    
    return 0;
}