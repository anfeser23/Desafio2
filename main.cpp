#include "funciones.h"
#include <iostream>
#include <cstring>
using namespace std;

int main() {

    // Cargar desde archivos
    cargarHuespedesDesdeArchivo();
    cargarAnfitrionesDesdeArchivo();
    cargarAlojamientosDesdeArchivo();
    cargarReservasDesdeArchivo();

    menuPrincipal();

    // Pedir fecha para actualizar histórico
    char fechaActual[11];
    tm fechaTm;

    cout << endl << "Ingrese la fecha actual para actualizar el historico (YYYY-MM-DD): ";
    cin >> fechaActual;

    while (!convertirFecha(fechaActual, fechaTm)) {
        cout << "Formato de fecha inválido. Intente nuevamente (YYYY-MM-DD): ";
        cin >> fechaActual;
    }

    actualizarHistorico(fechaActual);

    mostrarConsumo();

    return 0;
}
