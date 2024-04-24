#include "busca.hh"
#include <fstream>
#include <iomanip>
#include <chrono>
#include <stdlib.h>
#include <algorithm>

using namespace std;

// Definir un tipo para la tabla hash
using ArticuloHash = unordered_map<string, int>;

void abrir_fichero_lectura(string nombre_fichero, ifstream& f_in){
    f_in.open(nombre_fichero);
    if (!f_in.is_open()){
        cout << "Error al abrir el fichero \"" << nombre_fichero << "\" para su lectura" << endl;
        exit(1);
    }
}

void abrir_fichero_escritura(string nombre_fichero, ofstream& f_out){
    f_out.open(nombre_fichero);
    if (!f_out.is_open()){
        cout << "Error al abrir el fichero \"" << nombre_fichero << "\" para su escritura" << endl;
        exit(1);
    }
}

void leer_pagina(ifstream& f_in, Pagina& pagina){

    string s_num_articulos, s_ancho_pag, s_alto_pag;
    string x_articulo, y_articulo, ancho_articulo, alto_articulo;

    getline(f_in, s_num_articulos, ' ');
    getline(f_in, s_ancho_pag, ' ');
    getline(f_in, s_alto_pag);

    int num_articulos = stoi(s_num_articulos);
    int ancho_pag = stoi(s_ancho_pag);
    int alto_pag = stoi(s_alto_pag);

    vector<Articulo> v_articulos;

    for (int i = 0; i < num_articulos; i++){
        
        getline(f_in, ancho_articulo, ' ');
        getline(f_in, alto_articulo, ' ');
        getline(f_in, x_articulo, ' ');
        getline(f_in, y_articulo);

        v_articulos.push_back(Articulo(i, stoi(ancho_articulo), stoi(alto_articulo), stoi(x_articulo), stoi(y_articulo)));

    }

    pagina.num_articulos = num_articulos;
    pagina.ancho = ancho_pag;
    pagina.alto = alto_pag;
    pagina.actualizar_area();
    pagina.articulos = v_articulos;
}

bool hay_interseccion_entre_pareja_articulos(Articulo a, Articulo b){
    return  (a.x < b.x + b.ancho &&
            a.x + a.ancho > b.x &&
            a.y < b.y + b.alto &&
            a.y + a.alto > b.y);
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

void imprimir_solucion(ofstream& f_out, int num_pag, int area_solucion, vector<Articulo> art_solucion, long double tiempo_ejecucion){
    f_out << endl;

    f_out << "Pagina " << num_pag << endl << "\tTiempo: " << tiempo_ejecucion << " ms" << endl << "\tArea: " << area_solucion << " mm" << endl << "\tArticulos:";
    for(Articulo art : art_solucion){
        f_out << endl << "\t\t";
        f_out << art.ancho << " " << art.alto << " " << art.x << " " << art.y << " (Area: " << art.area << " mm)";
    }

    f_out << endl;
}



// ================================================
//              VERSIÓN ITERATIVA
// ------------------------------------------------

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


int busca_max_area_iterativo(vector<Articulo> articulos_anadidos, vector<Articulo> articulos_por_anadir, vector<Articulo>& solucion, ArticuloHash& tabla){
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
                area_calculada = art_anadido.area + busca_max_area_iterativo(articulos_anadidos, articulos_por_anadir, solucion, tabla);
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

// ================================================
//              VERSIÓN RECURSIVA
// ------------------------------------------------

int buscar_max_area_recursivo(vector<Articulo> articulos_anadidos, vector<Articulo> articulos_por_anadir, vector<Articulo>& solucion){
    int area_max = 0;
    vector<Articulo> articulos_no_interseccionados = encontrar_no_interseccionados(articulos_anadidos, articulos_por_anadir);

    if(articulos_no_interseccionados.empty()){
        return 0; // ya no quedan articulos validos que añadir
    } else {
        int area_calculada;

        for(Articulo art_anadido : articulos_no_interseccionados){

            articulos_anadidos.push_back(art_anadido);
            eliminar_articulo_vector(art_anadido, articulos_por_anadir);

            area_calculada = art_anadido.area + buscar_max_area_recursivo(articulos_anadidos, articulos_por_anadir, solucion);

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

// ================================================

int main(int argc, char *argv[]){
    ifstream f_in;
    ofstream f_out;


    if (argc < 4){
        cout << "ERROR: Numero de parametros invalido" << endl;
        exit(1);
    }

    abrir_fichero_lectura(argv[2], f_in);
    abrir_fichero_escritura(argv[3], f_out);

    string opcion = argv[1];
    int num_pag = 1;
    bool recursiva;

    // valoramos la opción pasada como argumento por el usuario
    if(opcion == "-r"){
        recursiva = true;

    } else if (opcion == "-i"){
        recursiva = false;

    } else {
        cout << "ERROR: No se ha reconocido la opción \"" + opcion + "\"\nOpciones válidas:\n\t\t-i\n\t\t-r" << endl;
        f_in.close();
        f_out.close();
        return 0;
    }
    
    while(!f_in.eof()){
        vector<Articulo> articulos_solucion = {};
        int area_solucion = 0;
        long double tiempo_ejecucion = 0;
        Pagina pagina;
        leer_pagina(f_in, pagina);
        
        if(recursiva) { // versión recursiva
            auto start_time = chrono::high_resolution_clock::now();
            area_solucion = buscar_max_area_recursivo({}, pagina.articulos, articulos_solucion);
            auto end_time = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time) / 1000000.0;
            tiempo_ejecucion = duracion.count();
        } else {        // versión iterativa
            ArticuloHash tabla_hash;
            auto start_time = chrono::high_resolution_clock::now();
            area_solucion = busca_max_area_iterativo({}, pagina.articulos, articulos_solucion, tabla_hash);
            auto end_time = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time) / 1000000.0;
            tiempo_ejecucion = duracion.count();
        }

        imprimir_solucion(f_out, num_pag, area_solucion, articulos_solucion, tiempo_ejecucion);
        num_pag++;
    }
    
    f_in.close();
    f_out.close();
    return 0;
}