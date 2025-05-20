// archivo funciones.h

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <cstring>

#define MAX_HUESPEDES 100
#define MAX_ANFITRIONES 50
#define MAX_ALOJAMIENTOS 100
#define MAX_RESERVAS 200
#define MAX_ANOTACIONES 500

class Huesped;
class Anfitrion;
class Alojamiento;
class Reservacion;

struct Amenidades {
    bool ascensor;
    bool piscina;
    bool aireAcondicionado;
    bool cajaFuerte;
    bool parqueadero;
    bool patio;
};

class Huesped {
private:
    char documento[20];
    int antiguedad;
    float puntuacion;
    int totalReservas;
    int reservas[MAX_RESERVAS];

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
private:
    char documento[20];
    int antiguedad;
    float puntuacion;

public:
    Anfitrion();
    Anfitrion(const char* doc, int ant, float punt);

    const char* getDocumento() const;
    int getAntiguedad() const;
    float getPuntuacion() const;
};

class Alojamiento {
private:
    char nombre[100];
    int codigo;
    char anfitrionDocumento[20];
    char departamento[30];
    char municipio[30];
    char tipo[10]; // Casa o Apartamento
    char direccion[100];
    float precio;
    Amenidades amenidades;
    char fechasReservadas[365][11]; // Hasta 365 fechas por año (YYYY-MM-DD)

public:
    Alojamiento();
    Alojamiento(int cod, const char* nom, const char* docAnfitrion, const char* dep, const char* muni, const char* t, const char* dir, float pr);

    int getCodigo() const;
    const char* getAnfitrionDocumento() const;
    const char* getNombre() const;
    const char* getMunicipio() const;
    const char* getTipo() const;
    float getPrecio() const;
    bool estaDisponible(const char* fechaEntrada, int duracion) const;
    void agregarReserva(const char* fechaEntrada, int duracion);
    void mostrarInfo() const;
    Amenidades getAmenidades() const;
    void setAmenidades(const Amenidades& amen);

    const char* getDepartamento() const { return departamento; }
    const char* getDireccion() const { return direccion; }


    // Método para obtener la puntuación del anfitrión
    float getPuntuacionAnfitrion() const;

    const char* getFechaReservada(int index) const {
        return (index >= 0 && index < 365) ? fechasReservadas[index] : "";
    }

    void setFechaReservada(int index, const char* fecha) {
        if (index >= 0 && index < 365) {
            strncpy(fechasReservadas[index], fecha, 11);
            fechasReservadas[index][10] = '\0';
        }
    }

    /*bool getAmenidad(int index) const {
        if (index >= 0 && index < 6) return amenidades[index];
        return false;
    }

    void setAmenidad(int index, bool valor) {
        if (index >= 0 && index < 6) amenidades[index] = valor;
    }*/

};


class Reservacion {
private:
    int codigo;
    int codAlojamiento;
    char documentoHuesped[20];
    char fechaEntrada[11];
    int duracion;
    float monto;
    char metodoPago[10];
    char fechaPago[11];
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

// Funciones auxiliares
Huesped* buscarHuespedPorDocumento(const char* doc);
Anfitrion* buscarAnfitrionPorDocumento(const char* doc);
Alojamiento* buscarAlojamientoPorCodigo(int codigo);
bool alojamientoOcupadoEnFecha(int codAlojamiento, const char* fecha);
bool fechaEsAnterior(const char* f1, const char* f2);
void realizarReserva(const char* documentoHuesped);
void menuHuesped(const char* documento);
void menuAnfitrion(const char* documento);
void verReservasDeAnfitrion(const char* docAnfitrion);
void mostrarConsumo();
void anularReservacion(int codigo, const char* doc);
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
bool convertirFecha(const char* , tm& );

// Funciones para reserva según filtros
void buscarAlojamientosPorFiltros(const char* fechaEntrada, const char* municipio, int duracion, float precioMaximo, float puntuacionMinima);
void buscarAlojamientosPorCodigo(int codigo);
void reservarAlojamientoPorCodigo(int, const char* , int );
void realizarReservaPorFiltros(const char* );
void realizarReservaPorCodigo(const char* );
void reservarAlojamientoPorCodigo(int , const char* , int , const char* );
void buscarAlojamientosPorFiltros(const char* , const char* , int , float);
bool codigoReservaExiste(int );

#endif
