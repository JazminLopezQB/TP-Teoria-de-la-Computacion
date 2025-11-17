cmake_minimum_required(VERSION 3.22.1)
project(Lexico)

set(CMAKE_CXX_STANDARD 23)

# Archivos fuente
set(SOURCES
        main.c
        scanner.c
        scanner.h
        )

# Crear ejecutable
add_executable(SALIDA ${SOURCES})
