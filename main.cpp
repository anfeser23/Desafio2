#include "funciones.h"
#include <iostream>

int main() {
    // Cargar desde archivos
    cargarHuespedesDesdeArchivo();
    cargarAnfitrionesDesdeArchivo();
    cargarAlojamientosDesdeArchivo();
    cargarReservasDesdeArchivo();

    menuPrincipal();

    // Guardar en archivos
    guardarHuespedesEnArchivo();
    guardarAnfitrionesEnArchivo();
    guardarAlojamientosEnArchivo();
    guardarReservasEnArchivo();

    mostrarConsumo();
    return 0;
}
