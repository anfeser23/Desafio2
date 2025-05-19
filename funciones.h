#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <cstdio>
#include <cstring>

#define MAX_HUESPEDES 100
#define MAX_ANFITRIONES 50
#define MAX_ALOJAMIENTOS 100
#define MAX_RESERVAS 500
#define MAX_ANOTACIONES 1000

// Declaración de las clases
class Huesped {
    char documento[20];
    int antiguedad;
    float puntuacion;
    int reservas[50];
    int totalReservas;

public:
    Huesped();
    Huesped(const char* doc, int ant, float punt);

    const char* getDocumento() const;
    int getAntiguedad() const;
    float getPuntuacion() const;
    void agregarReserva(int cod);
    int getNumReservas() const;
    int getCodigoReserva(int i) const;
};

class Anfitrion {
    char documento[20];
    int antiguedad;
    float puntuacion;

public:
    Anfitrion();
    Anfitrion(const char* doc, int ant, float punt);

    const char* getDocumento() const;
    const char* getAntiguedad() const;
    float getPuntuacion() const;
};

class Alojamiento {
    int codigo;
    char anfitrionDocumento[20];
    char municipio[30];
    float precio;

public:
    Alojamiento();
    Alojamiento(int cod, const char* docAnfitrion, const char* muni, float pr);

    int getCodigo() const;
    const char* getAnfitrionDocumento() const;
    const char* getMunicipio() const;
    float getPrecio() const;
};

class Reservacion {
    int codigo;
    int codAlojamiento;
    char documentoHuesped[20];
    char fechaEntrada[11];
    int duracion;
    char metodoPago[10];
    char fechaPago[11];
    float monto;
    char anotaciones[MAX_ANOTACIONES];

public:
    Reservacion();
    Reservacion(int cod, int alojCod, const char* docH, const char* fEntrada, int dur, const char* mPago, const char* fPago, float montoP, const char* notas);

    int getCodigo() const;
    int getCodAlojamiento() const;
    const char* getDocumentoHuesped() const;
    const char* getFechaEntrada() const;
    int getDuracion() const;
    const char* getMetodoPago() const;
    const char* getFechaPago() const;
    float getMonto() const;
    const char* getAnotaciones() const;
    const char* getDocumentoAnfitrion();
};

// Declaración de las variables globales
extern Huesped huespedes[MAX_HUESPEDES];
extern Anfitrion anfitriones[MAX_ANFITRIONES];
extern Alojamiento alojamientos[MAX_ALOJAMIENTOS];
extern Reservacion reservaciones[MAX_RESERVAS];

extern int totalHuespedes, totalAnfitriones, totalAlojamientos, totalReservas;
extern int contadorIteraciones;

// Declaración de las funciones
Huesped* buscarHuespedPorDocumento(const char* doc);
Anfitrion* buscarAnfitrionPorDocumento(const char* doc);
Alojamiento* buscarAlojamientoPorCodigo(int codigo);


bool alojamientoOcupadoEnFecha(int, const char*);
void realizarReserva(const char* );
void menuHuesped(const char* );
void verReservasDeAnfitrion(const char*);
void menuAnfitrion(const char* );
void mostrarConsumo();
void anularReservacion(int codigo, const char* doc);
bool fechaEsAnterior(const char* f1, const char* f2);
void actualizarHistorico(const char* hoy);
void cargarHuespedesDesdeArchivo();
void cargarAnfitrionesDesdeArchivo();
void cargarAlojamientosDesdeArchivo();
void cargarReservasDesdeArchivo();
void guardarHuespedesEnArchivo();
void guardarAnfitrionesEnArchivo();
void guardarAlojamientosEnArchivo();
void guardarReservasEnArchivo();
void menuPrincipal();

#endif // FUNCIONES_H
