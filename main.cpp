
#include <iostream>
#include <cstdio>
#include <cstring>

#define MAX_HUESPEDES 100
#define MAX_ANFITRIONES 50
#define MAX_ALOJAMIENTOS 100
#define MAX_RESERVAS 500
#define MAX_ANOTACIONES 1000

// ----------- CLASES BÁSICAS -----------

class Huesped {
    char documento[20];
    int antiguedad;
    float puntuacion;
    int reservas[50];
    int totalReservas;

public:
    Huesped() : antiguedad(0), puntuacion(0.0), totalReservas(0) {
        documento[0] = '\0';
    }

    Huesped(const char* doc, int ant, float punt) : antiguedad(ant), puntuacion(punt), totalReservas(0) {
        strncpy(documento, doc, 20);
    }

    const char* getDocumento() const { return documento; }
    int getAntiguedad() const { return antiguedad; }
    float getPuntuacion() const { return puntuacion; }
    void agregarReserva(int cod) { reservas[totalReservas++] = cod; }
    int getNumReservas() const { return totalReservas; }
    int getCodigoReserva(int i) const { return reservas[i]; }
};

class Anfitrion {
    char documento[20];
    int antiguedad;
    float puntuacion;

public:
    Anfitrion() : antiguedad(0), puntuacion(0.0) { documento[0] = '\0'; }
    Anfitrion(const char* doc, int ant, float punt) : antiguedad(ant), puntuacion(punt) {
        strncpy(documento, doc, 20);
    }

    const char* getDocumento() const { return documento; }
    float getPuntuacion() const { return puntuacion; }
};

class Alojamiento {
    int codigo;
    char anfitrionDocumento[20];
    char municipio[30];
    float precio;

public:
    Alojamiento() : codigo(0), precio(0.0) {
        anfitrionDocumento[0] = municipio[0] = '\0';
    }

    Alojamiento(int cod, const char* docAnfitrion, const char* muni, float pr) : codigo(cod), precio(pr) {
        strncpy(anfitrionDocumento, docAnfitrion, 20);
        strncpy(municipio, muni, 30);
    }

    int getCodigo() const { return codigo; }
    const char* getAnfitrionDocumento() const { return anfitrionDocumento; }
    const char* getMunicipio() const { return municipio; }
    float getPrecio() const { return precio; }
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
    Reservacion() : codigo(0), codAlojamiento(0), duracion(0), monto(0.0) {
        documentoHuesped[0] = fechaEntrada[0] = metodoPago[0] = fechaPago[0] = anotaciones[0] = '\0';
    }

    Reservacion(int cod, int alojCod, const char* docH, const char* fEntrada, int dur, const char* mPago, const char* fPago, float montoP, const char* notas)
        : codigo(cod), codAlojamiento(alojCod), duracion(dur), monto(montoP) {
        strncpy(documentoHuesped, docH, 20);
        strncpy(fechaEntrada, fEntrada, 11);
        strncpy(metodoPago, mPago, 10);
        strncpy(fechaPago, fPago, 11);
        strncpy(anotaciones, notas, MAX_ANOTACIONES);
    }

    int getCodigo() const { return codigo; }
    int getCodAlojamiento() const { return codAlojamiento; }
    const char* getDocumentoHuesped() const { return documentoHuesped; }
    const char* getFechaEntrada() const { return fechaEntrada; }
    int getDuracion() const { return duracion; }
    const char* getMetodoPago() const { return metodoPago; }
    const char* getFechaPago() const { return fechaPago; }
    float getMonto() const { return monto; }
    const char* getAnotaciones() const { return anotaciones; }

    const char* getDocumentoAnfitrion(); // Definida abajo
};

// ----------- VARIABLES GLOBALES -----------

Huesped huespedes[MAX_HUESPEDES];
Anfitrion anfitriones[MAX_ANFITRIONES];
Alojamiento alojamientos[MAX_ALOJAMIENTOS];
Reservacion reservaciones[MAX_RESERVAS];

int totalHuespedes = 0, totalAnfitriones = 0, totalAlojamientos = 0, totalReservas = 0;
int contadorIteraciones = 0;

// ----------- FUNCIONES AUXILIARES -----------

Huesped* buscarHuespedPorDocumento(const char* doc) {
    for (int i = 0; i < totalHuespedes; ++i) {
        contadorIteraciones++;
        if (strcmp(huespedes[i].getDocumento(), doc) == 0) return &huespedes[i];
    }
    return nullptr;
}

Anfitrion* buscarAnfitrionPorDocumento(const char* doc) {
    for (int i = 0; i < totalAnfitriones; ++i) {
        contadorIteraciones++;
        if (strcmp(anfitriones[i].getDocumento(), doc) == 0) return &anfitriones[i];
    }
    return nullptr;
}

Alojamiento* buscarAlojamientoPorCodigo(int codigo) {
    for (int i = 0; i < totalAlojamientos; ++i) {
        contadorIteraciones++;
        if (alojamientos[i].getCodigo() == codigo) return &alojamientos[i];
    }
    return nullptr;
}

// Definición fuera de clase (requiere búsqueda)
const char* Reservacion::getDocumentoAnfitrion() {
    Alojamiento* a = buscarAlojamientoPorCodigo(codAlojamiento);
    return (a) ? a->getAnfitrionDocumento() : "desconocido";
}

// ----------- FUNCIONES PRINCIPALES -----------

void mostrarConsumo() {
    std::cout << "Iteraciones: " << contadorIteraciones << "\n";
    std::cout << "Memoria: " << sizeof(Huesped) * totalHuespedes + sizeof(Anfitrion) * totalAnfitriones +
                                    sizeof(Alojamiento) * totalAlojamientos + sizeof(Reservacion) * totalReservas
              << " bytes\n";
}

void anularReservacion(int codigo, const char* doc) {
    for (int i = 0; i < totalReservas; ++i) {
        contadorIteraciones++;
        if (reservaciones[i].getCodigo() == codigo) {
            if (strcmp(reservaciones[i].getDocumentoHuesped(), doc) == 0 ||
                strcmp(reservaciones[i].getDocumentoAnfitrion(), doc) == 0) {
                for (int j = i; j < totalReservas - 1; ++j)
                    reservaciones[j] = reservaciones[j + 1];
                totalReservas--;
                std::cout << "Reservación anulada.\n";
                return;
            }
        }
    }
    std::cout << "No se encontró la reservación o no tiene permiso.\n";
}

bool fechaEsAnterior(const char* f1, const char* f2) {
    return strcmp(f1, f2) < 0;
}

void actualizarHistorico(const char* hoy) {
    FILE* archivo = fopen("historico_reservas.txt", "a");
    for (int i = 0; i < totalReservas;) {
        contadorIteraciones++;
        if (fechaEsAnterior(reservaciones[i].getFechaEntrada(), hoy)) {
            fprintf(archivo, "%d|%d|%s|%s|%d|%s|%s|%.2f|%s\n",
                    reservaciones[i].getCodigo(),
                    reservaciones[i].getCodAlojamiento(),
                    reservaciones[i].getDocumentoHuesped(),
                    reservaciones[i].getFechaEntrada(),
                    reservaciones[i].getDuracion(),
                    reservaciones[i].getMetodoPago(),
                    reservaciones[i].getFechaPago(),
                    reservaciones[i].getMonto(),
                    reservaciones[i].getAnotaciones());
            for (int j = i; j < totalReservas - 1; ++j)
                reservaciones[j] = reservaciones[j + 1];
            totalReservas--;
        } else {
            i++;
        }
    }
    fclose(archivo);
    std::cout << "Histórico actualizado.\n";
}

// ----------- MENÚ Y MAIN -----------

void menuPrincipal() {
    char documento[20];
    int opcion;

    do {
        std::cout << "\n--- UdeAStay ---\n";
        std::cout << "1. Ingresar como huésped\n";
        std::cout << "2. Ingresar como anfitrión\n";
        std::cout << "3. Salir\nOpción: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            std::cout << "Documento del huésped: ";
            std::cin >> documento;
            if (buscarHuespedPorDocumento(documento)) {
                std::cout << "Bienvenido huésped.\n";
                // aquí irían más opciones para huéspedes
            } else std::cout << "No encontrado.\n";
            break;
        case 2:
            std::cout << "Documento del anfitrión: ";
            std::cin >> documento;
            if (buscarAnfitrionPorDocumento(documento)) {
                std::cout << "Bienvenido anfitrión.\n";
                // aquí irían más opciones para anfitriones
            } else std::cout << "No encontrado.\n";
            break;
        }
    } while (opcion != 3);
}

int main() {
    // cargar desde archivos (pendiente implementar)
    menuPrincipal();
    // guardar en archivos (pendiente implementar)

    mostrarConsumo();
    return 0;
}
