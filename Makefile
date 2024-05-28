#*****************************************************************
# File:   Makefile
# Author: Manel Jorda Puig Rubio
# Date:   febrero 2024
# Coms:   Ejecutar "make" para compilar
#         Ejecutar "make clean" para limpiar (borrar lo que genere)
#*****************************************************************
#---------------------------------------------------------
# macros
RM = /bin/rm -f # para limpiar
CC = g++         # compilador
#---------------------------------------------------------
LIB=./librerias
ARBOL=${LIB}/arbol
PAG=${LIB}/pagina
ART=${LIB}/articulo
#---------------------------------------------------------
# opciones compilación y enlazado ("linkado")
CPPFLAGS = -g -I. -I${LIB} -O2 -std=c++11 -lstdc++
#---------------------------------------------------------
# vars
EJEC = busca
#
all: ${EJEC}
#---------------------------------------------------------
# compilar
${EJEC}.o: ${EJEC}.hh ${ART}.hh ${PAG}.hh ${EJEC}.cc
	${CC} -c ${EJEC}.cc -o ${EJEC}.o ${CPPFLAGS}
#
${ARBOL}.o: ${ARBOL}.hh ${ARBOL}.cc
	${CC} -c ${ARBOL}.cc -o ${ARBOL}.o ${CPPFLAGS}
#
# "linkar"
#---------------------------------------------------------
${EJEC}: ${EJEC}.o ${ARBOL}.o
	${CC} ${EJEC}.o ${ARBOL}.o -o ${EJEC} ${CPPFLAGS}
#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	${RM} ${ARBOL}.o ${EJEC}.o ${EJEC}
