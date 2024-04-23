#ifndef BUSCA_HH
#define BUSCA_HH

#include <vector>
#include <iostream>
#include <string>
#include "./librerias/pagina.hh"
#include "./librerias/articulo.hh"
#include <unordered_map>

using namespace std;
using ArticuloHash = unordered_map<string, int>;


void abrir_fichero_lectura(string nombre_fichero, ifstream& f_in);

void abrir_fichero_escritura(string nombre_fichero, ofstream& f_out);

void leer_pagina(ifstream& f_in, Pagina& pagina);

bool hay_interseccion_entre_pareja_articulos(Articulo a, Articulo b);

vector<Articulo> encontrar_no_interseccionados(vector<Articulo> articulos_anadidos, vector<Articulo> articulos_por_anadir);

void eliminar_articulo_vector(Articulo art_a_eliminar, vector<Articulo>& articulos);

void imprimir_solucion(ofstream& f_out, int num_pag, int area_solucion, vector<Articulo> art_solucion, long double tiempo_ejecucion);


// ------------------------------------------------
//              VERSIÓN ITERATIVA
// ------------------------------------------------

// Verificar si el área de un conjunto de artículos ya está en la tabla hash
bool area_en_tabla_hash(ArticuloHash& tabla, vector<Articulo>& articulos);

// Agregar el área de un conjunto de artículos a la tabla hash
void agregar_area_a_tabla_hash(ArticuloHash& tabla, vector<Articulo>& articulos, int area);

// Obtener el área de un conjunto de artículos desde la tabla hash
int obtener_area_desde_tabla_hash(ArticuloHash& tabla, vector<Articulo>& articulos);


int busca_max_area_iterativo(vector<Articulo> articulos_anadidos, vector<Articulo> articulos_por_anadir, vector<Articulo>& solucion, ArticuloHash& tabla);


// ------------------------------------------------
//              VERSIÓN RECURSIVA
// ------------------------------------------------

int buscar_max_area_recursivo(vector<Articulo> articulos_anadidos, vector<Articulo> articulos_por_anadir, vector<Articulo>& solucion);

#endif