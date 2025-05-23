#include "funciones.h"
//#include <iostream>
using namespace std;

int main() {

    // Cargar desde archivos
    cargarHuespedesDesdeArchivo();
    cargarAnfitrionesDesdeArchivo();
    cargarAlojamientosDesdeArchivo();
    cargarReservasDesdeArchivo();

    menuPrincipal();

    // Guardar en archivos
    //guardarHuespedesEnArchivo();
    //guardarAnfitrionesEnArchivo();
    //guardarAlojamientosEnArchivo();
    //guardarReservasEnArchivo();

    /*char fecha;

    cout << endl << "Ingrese la fecha actual para guardar el historico: ";
    cin >>fecha ;

    actualizarHistorico(fecha);*/

    mostrarConsumo();
    return 0;
}
