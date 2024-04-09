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

bool intersecan_todos_articulos(const vector<Articulo>& articulos) {
    for(int i = 0; i < articulos.size(); i++){
        for(int j = i+1; j < articulos.size(); j++){
            if(!hay_interseccion_entre_pareja_articulos(articulos[i], articulos[j])){
                return false;
            }
        }
    }
    return true;
}

Articulo calcular_articulo_interseccion_pareja(Articulo a, Articulo b){

    // guardamos los puntos derecha-inferior de ambos articulos por comodidad
    int x_dcha_a = a.x + a.ancho;
    int y_inf_a = a.y + a.alto;

    int x_dcha_b = b.x + b.ancho;
    int y_inf_b = b.y + b.alto;

    int res_x = max(a.x, b.x);
    int res_y = max(a.y, b.y);
    int res_ancho = min(x_dcha_a, x_dcha_b) - res_x;
    int res_alto = min(y_inf_a, y_inf_b) - res_y;

    return Articulo(0, res_x, res_y, res_ancho, res_alto);
}

int calcular_area_interseccion_total(const vector<Articulo>& articulos) {
    if(articulos.size() == 0){
        cout << "!!! No tiene articulos" << endl;
        return 0;
    }
    if(articulos.size() == 1){
        cout << "!!! Solo 1 articulo: area " << articulos[0].area << endl;
        return articulos[0].area;
    }

    if(!intersecan_todos_articulos(articulos)){
        cout << "!!! No intersecan todos los articulos" << endl;
        return 0;
    }

    Articulo interseccion = articulos[0];
    cout << "Interseccion parcial (primer articulo): ancho=" << interseccion.ancho << ", alto=" << interseccion.alto << ", x=" << interseccion.x << ", y=" << interseccion.y << endl;
    for(int i = 1; i < articulos.size(); i++){
        interseccion = calcular_articulo_interseccion_pareja(interseccion, articulos[i]);
        cout << "Interseccion parcial: ancho=" << interseccion.ancho << ", alto=" << interseccion.alto << ", x=" << interseccion.x << ", y=" << interseccion.y << endl;
    }

    return interseccion.area;
}

void calcular_partes(const vector<Articulo>& articulos_actuales, vector<vector<Articulo>>& partes){
    for (int i = 0; i < articulos_actuales.size(); i++)
    {
        vector<Articulo> empty;
        partes.push_back( empty );

        vector< vector<Articulo> > subset_temp = partes;  //making a copy of given 2-d vector.
        
        for (int j = 0; j < subset_temp.size(); j++)
            subset_temp[j].push_back( articulos_actuales[i] );   // adding set[i] element to each subset of subsetTemp. like adding {2}(in 2nd iteration  to {{},{1}} which gives {{2},{1,2}}.
        
        for (int j = 0; j < subset_temp.size(); j++)
            partes.push_back( subset_temp[j] );  //now adding modified subsetTemp to original subset (before{{},{1}} , after{{},{1},{2},{1,2}})
    }

    sort(partes.begin(), partes.end());
    partes.erase( unique(partes.begin(), partes.end() ), partes.end());
}


int calcular_area(const vector<Articulo>& articulos) {
    int area_total = 0;

    vector<vector<Articulo>> partes;
    calcular_partes(articulos, partes);

    for(vector<Articulo> conjunto : partes){
        cout << "Calculando area conjunto ";
        for(Articulo art : conjunto) cout << art.id << " ";
        cout << endl;

        int area_interseccion = calcular_area_interseccion_total(conjunto);
        cout << "   Area interseccion: " << area_interseccion << endl;
        if(conjunto.size() % 2 == 0){
            area_total -= area_interseccion;
        } else {
            area_total += area_interseccion;
        }
        cout << "Area total actual: " << area_total << endl << endl;
    }
    cout << endl;
    cout << "------------" << endl;
    cout << "AREA TOTAL: " << area_total << endl;
    cout << "------------" << endl;

    return area_total;
}

// ================================================================================================================================================= //
// ================================================================================================================================================= //

int maximizeArea(vector<Articulo>& articulos, int i, int area_total, unordered_map<int, int>& memo, vector<Articulo>& articulos_usados) {
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
        articulos_usados.push_back(articulos.at(i));
        area_included = maximizeArea(articulos, i + 1, calcular_area(articulos_usados), memo, articulos_usados);
        articulos_usados.pop_back();
    }

    // Si hay superposición de artículos, se continua sin tener en cuenta el área del artículo actual
    int area_excluded = maximizeArea(articulos, i + 1, area_total, memo, articulos_usados);

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
    vector<Articulo> articulos_usados = {};
    unordered_map<int, int> memo1, memo2;
    int area_total = maximizeArea(articulos, 0, 0, memo1, articulos_usados);
    cout<< "---------------------" << endl;
    cout << "Area total ocupada: " << area_total << endl;
    cout<< "---------------------" << endl;
    //cout << "Area total ocupada: " << maximizeArea(articulos_prueba, 0, 0, memo2, articulos_usados) << endl;
    
    return 0;
}