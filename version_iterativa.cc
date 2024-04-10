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
#include <unordered_map> // Agregar la librería para el hash map

using namespace std;

// Definir un tipo para la tabla hash
using ArticuloHash = unordered_map<string, int>;

bool hay_interseccion_entre_pareja_articulos(Articulo a, Articulo b){
    return  (a.x < b.x + b.ancho &&
            a.x + a.ancho > b.x &&
            a.y < b.y + b.alto &&
            a.y + a.alto > b.y);
}

// Verificar si el área de un conjunto de artículos ya está en la tabla hash
bool area_en_tabla_hash(ArticuloHash& tabla, vector<Articulo>& articulos) {
    // Crear una clave única para este conjunto de artículos
    string clave = "";
    for (Articulo& art : articulos) {
        clave += to_string(art.id) + ",";
    }

    // Buscar en la tabla hash
    if (tabla.find(clave) != tabla.end()) {
        return true; // Si la clave existe en la tabla, retorna verdadero
    } else {
        return false; // Si no, retorna falso
    }
}

// Agregar el área de un conjunto de artículos a la tabla hash
void agregar_area_a_tabla_hash(ArticuloHash& tabla, vector<Articulo>& articulos, int area) {
    // Crear una clave única para este conjunto de artículos
    string clave = "";
    for (Articulo& art : articulos) {
        clave += to_string(art.id) + ",";
    }

    // Agregar el área asociada a esta clave en la tabla hash
    tabla[clave] = area;
}

// Obtener el área de un conjunto de artículos desde la tabla hash
int obtener_area_desde_tabla_hash(ArticuloHash& tabla, vector<Articulo>& articulos) {
    // Crear una clave única para este conjunto de artículos
    string clave = "";
    for (Articulo& art : articulos) {
        clave += to_string(art.id) + ",";
    }

    // Retornar el área asociada a esta clave en la tabla hash
    return tabla[clave];
}

vector<Articulo> encontrar_no_interseccionados(vector<Articulo> articulos_anadidos, vector<Articulo> articulos_por_anadir){
    vector<Articulo> articulos_no_interseccionados;

    // si no hay articulos añadidos con los que comparar, ninguno intersecciona
    if(articulos_anadidos.size() == 0) return articulos_por_anadir;

    // si no quedan articulos que añadir
    if(articulos_por_anadir.size() == 0) return {};

    for(Articulo por_anadir : articulos_por_anadir){
        bool intersecciona = false;

        for(Articulo anadido : articulos_anadidos){
            if(hay_interseccion_entre_pareja_articulos(por_anadir, anadido)){
                intersecciona = true;
                break;
            }
        }

        if(!intersecciona){
            articulos_no_interseccionados.push_back(por_anadir);
        }
    }

    return articulos_no_interseccionados;
}

void eliminar_articulo_vector(Articulo art_a_eliminar, vector<Articulo>& articulos){
    vector<Articulo>::iterator encontrado = find(articulos.begin(), articulos.end(), art_a_eliminar);
    articulos.erase(encontrado);
}



int maximizeArea(vector<Articulo> articulos_anadidos, vector<Articulo> articulos_por_anadir, vector<Articulo>& solucion, ArticuloHash& tabla){
    int area_max = 0;
    vector<Articulo> articulos_no_interseccionados = encontrar_no_interseccionados(articulos_anadidos, articulos_por_anadir);

    if(articulos_no_interseccionados.empty()){
        return 0; // ya no quedan articulos validos que añadir
    } else {
        int area_calculada;

        for(Articulo art_anadido : articulos_no_interseccionados){

            articulos_anadidos.push_back(art_anadido);
            eliminar_articulo_vector(art_anadido, articulos_por_anadir);

            // si para la combinacion de articulos que tenemos ya hemos calculado el area entonces la obtenemos del hashmap
            if (area_en_tabla_hash(tabla, articulos_anadidos)) {
                area_calculada = obtener_area_desde_tabla_hash(tabla, articulos_anadidos);
            } else {
                // sino, la calculamos recursivamente y después la agregamos 
                area_calculada = art_anadido.area + maximizeArea(articulos_anadidos, articulos_por_anadir, solucion, tabla);
                agregar_area_a_tabla_hash(tabla, articulos_anadidos, area_calculada);
            }

            if(area_calculada > area_max){
                area_max = area_calculada;
                solucion = articulos_anadidos;
            }

            // volvemos a los valores originales
            articulos_anadidos.pop_back();
            articulos_por_anadir.push_back(art_anadido);
        }
    }

    return area_max;
}

void imprimir_solucion(int num_test, int area_solucion, vector<Articulo> art_solucion){
    cout << "Test " << num_test << " | Area: " << area_solucion << ", Articulos:";
    for(Articulo art : art_solucion){
        cout << " " << art.id;
    }
    cout << endl << endl;
}


int main() {

    vector<Articulo> solucion;
    ArticuloHash tabla1, tabla2, tabla3, tabla4;

    // vector<Articulo> todos_articulos = {Articulo(1, 2, 2, 3, 2), Articulo(2, 2, 1, 4, 3), Articulo(3, 1, 1, 2, 2), Articulo(4, 1, 3, 3, 4)}; // solucion 8
    vector<Articulo> test_1 = {Articulo(1, 3, 3, 3, 3), Articulo(2, 3, 3, 1, 1), Articulo(3, 3, 3, 5, 1), Articulo(4, 3, 3, 1, 5), Articulo(5, 3, 3, 5, 5)};
    vector<Articulo> test_2 = {Articulo(1, 1, 1, 1, 1), Articulo(2, 2, 3, 1, 3), Articulo(3, 8, 4, 1, 3), Articulo(4, 9, 5, 3, 1)};
    vector<Articulo> test_3 = {Articulo(1, 1, 12, 1, 0), Articulo(2, 12, 3, 0, 1), Articulo(3, 12, 1, 0, 5), Articulo(4, 6, 3, 5, 1)};
    vector<Articulo> test_4 = {Articulo(1, 3, 7, 0, 0), Articulo(2, 6, 7, 3, 0), Articulo(3, 3, 7, 9, 0), Articulo(4, 12, 7, 0, 0)};

    int area_solucion = maximizeArea({}, test_1, solucion, tabla1);
    imprimir_solucion(1, area_solucion, solucion);
    
    area_solucion = maximizeArea({}, test_2, solucion, tabla2);
    imprimir_solucion(2, area_solucion, solucion);

    area_solucion = maximizeArea({}, test_3, solucion, tabla3);
    imprimir_solucion(3, area_solucion, solucion);

    area_solucion = maximizeArea({}, test_4, solucion, tabla4);
    imprimir_solucion(4, area_solucion, solucion);

   return 0;
}
