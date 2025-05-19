#include "funciones.h"

Huesped huespedes[MAX_HUESPEDES];
Anfitrion anfitriones[MAX_ANFITRIONES];
Alojamiento alojamientos[MAX_ALOJAMIENTOS];
Reservacion reservaciones[MAX_RESERVAS];

int totalHuespedes = 0, totalAnfitriones = 0, totalAlojamientos = 0, totalReservas = 0;
int contadorIteraciones = 0;

// Implementación de las clases
Huesped::Huesped() : antiguedad(0), puntuacion(0.0), totalReservas(0) {
    documento[0] = '\0';
}

Huesped::Huesped(const char* doc, int ant, float punt) : antiguedad(ant), puntuacion(punt), totalReservas(0) {
    strncpy(documento, doc, 20);
}

const char* Huesped::getDocumento() const { return documento; }
int Huesped::getAntiguedad() const { return antiguedad; }
float Huesped::getPuntuacion() const { return puntuacion; }
void Huesped::agregarReserva(int cod) { reservas[totalReservas++] = cod; }
int Huesped::getNumReservas() const { return totalReservas; }
int Huesped::getCodigoReserva(int i) const { return reservas[i]; }

Anfitrion::Anfitrion() : antiguedad(0), puntuacion(0.0) { documento[0] = '\0'; }
Anfitrion::Anfitrion(const char* doc, int ant, float punt) : antiguedad(ant), puntuacion(punt) {
    strncpy(documento, doc, 20);
}

const char* Anfitrion::getDocumento() const { return documento; }
const char* Anfitrion::getAntiguedad() const { return documento; }  // Aquí parece que quieres retornar otra cosa, revisa si es correcto
float Anfitrion::getPuntuacion() const { return puntuacion; }

Alojamiento::Alojamiento() : codigo(0), precio(0.0) {
    anfitrionDocumento[0] = municipio[0] = '\0';
}

Alojamiento::Alojamiento(int cod, const char* docAnfitrion, const char* muni, float pr) : codigo(cod), precio(pr) {
    strncpy(anfitrionDocumento, docAnfitrion, 20);
    strncpy(municipio, muni, 30);
}

int Alojamiento::getCodigo() const { return codigo; }
const char* Alojamiento::getAnfitrionDocumento() const { return anfitrionDocumento; }
const char* Alojamiento::getMunicipio() const { return municipio; }
float Alojamiento::getPrecio() const { return precio; }

Reservacion::Reservacion() : codigo(0), codAlojamiento(0), duracion(0), monto(0.0) {
    documentoHuesped[0] = fechaEntrada[0] = metodoPago[0] = fechaPago[0] = anotaciones[0] = '\0';
}

Reservacion::Reservacion(int cod, int alojCod, const char* docH, const char* fEntrada, int dur, const char* mPago, const char* fPago, float montoP, const char* notas)
    : codigo(cod), codAlojamiento(alojCod), duracion(dur), monto(montoP) {
    strncpy(documentoHuesped, docH, 20);
    strncpy(fechaEntrada, fEntrada, 11);
    strncpy(metodoPago, mPago, 10);
    strncpy(fechaPago, fPago, 11);
    strncpy(anotaciones, notas, MAX_ANOTACIONES);
}

int Reservacion::getCodigo() const { return codigo; }
int Reservacion::getCodAlojamiento() const { return codAlojamiento; }
const char* Reservacion::getDocumentoHuesped() const { return documentoHuesped; }
const char* Reservacion::getFechaEntrada() const { return fechaEntrada; }
int Reservacion::getDuracion() const { return duracion; }
const char* Reservacion::getMetodoPago() const { return metodoPago; }
const char* Reservacion::getFechaPago() const { return fechaPago; }
float Reservacion::getMonto() const { return monto; }
const char* Reservacion::getAnotaciones() const { return anotaciones; }

const char* Reservacion::getDocumentoAnfitrion() {
    Alojamiento* a = buscarAlojamientoPorCodigo(codAlojamiento);
    return (a) ? a->getAnfitrionDocumento() : "desconocido";
}

// Implementación de las funciones auxiliares
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

/* Definición fuera de clase (requiere búsqueda)
//const char* Reservacion::getDocumentoAnfitrion() {
    Alojamiento* a = buscarAlojamientoPorCodigo(codAlojamiento);
    return (a) ? a->getAnfitrionDocumento() : "desconocido";
}*/


// ----------- FUNCIONES PRINCIPALES -----------

bool alojamientoOcupadoEnFecha(int codAlojamiento, const char* fecha) {
    for (int i = 0; i < totalReservas; ++i) {
        if (reservaciones[i].getCodAlojamiento() == codAlojamiento &&
            strcmp(reservaciones[i].getFechaEntrada(), fecha) == 0) {
            return true;
        }
    }
    return false;
}

void realizarReserva(const char* documentoHuesped) {
    char fechaDeseada[11];
    std::cout << "Ingrese la fecha deseada para la reserva (YYYY-MM-DD): ";
    std::cin >> fechaDeseada;

    std::cout << "\nAlojamientos disponibles para la fecha " << fechaDeseada << ":\n";

    bool hayDisponibles = false;
    for (int i = 0; i < totalAlojamientos; ++i) {
        if (!alojamientoOcupadoEnFecha(alojamientos[i].getCodigo(), fechaDeseada)) {
            std::cout << "Codigo: " << alojamientos[i].getCodigo()
                      << " | Municipio: " << alojamientos[i].getMunicipio()
                      << " | Precio por noche: $" << alojamientos[i].getPrecio() << "\n";
            hayDisponibles = true;
        }
    }

    if (!hayDisponibles) {
        std::cout << "No hay alojamientos disponibles en esa fecha.\n";
        return;
    }

    int codAlojamiento, duracion, codigo;
    char metodoPago[10], fechaPago[11], anotaciones[MAX_ANOTACIONES];

    std::cout << "Seleccione el codigo del alojamiento que desea reservar: ";
    std::cin >> codAlojamiento;

    Alojamiento* aloj = buscarAlojamientoPorCodigo(codAlojamiento);
    if (!aloj || alojamientoOcupadoEnFecha(codAlojamiento, fechaDeseada)) {
        std::cout << "Codigo invalido o ya reservado en esa fecha.\n";
        return;
    }

    std::cout << "Codigo de reserva (unico): ";
    std::cin >> codigo;

    std::cout << "Duracion (en dias): ";
    std::cin >> duracion;

    std::cout << "Metodo de pago: ";
    std::cin >> metodoPago;

    std::cout << "Fecha de pago (YYYY-MM-DD): ";
    std::cin >> fechaPago;

    std::cin.ignore();
    std::cout << "Anotaciones: ";
    std::cin.getline(anotaciones, MAX_ANOTACIONES);

    float monto = duracion * aloj->getPrecio();

    reservaciones[totalReservas++] = Reservacion(codigo, codAlojamiento, documentoHuesped, fechaDeseada,
                                                 duracion, metodoPago, fechaPago, monto, anotaciones);

    Huesped* h = buscarHuespedPorDocumento(documentoHuesped);
    if (h) h->agregarReserva(codigo);

    std::cout << "Reserva completada. Monto total: $" << monto << "\n";

}

void menuHuesped(const char* documento) {
    int op;
    do {
        std::cout << "\n--- Menu Huesped ---\n";
        std::cout << "1. Realizar una reserva\n";
        std::cout << "2. Anular una reserva\n";
        std::cout << "3. Consultar consumo\n";
        std::cout << "4. Volver al menu principal\nOpcion: ";
        std::cin >> op;

        switch (op) {
        case 1:
            realizarReserva(documento);
            break;
        case 2: {
            int codigo;
            std::cout << "Codigo de la reserva a anular: ";
            std::cin >> codigo;
            anularReservacion(codigo, documento);
            break;
        }
        case 3:
            mostrarConsumo();
            break;
        }
    } while (op != 4);
}

void verReservasDeAnfitrion(const char* docAnfitrion) {
    bool hay = false;
    for (int i = 0; i < totalReservas; ++i) {
        Alojamiento* a = buscarAlojamientoPorCodigo(reservaciones[i].getCodAlojamiento());
        if (a && strcmp(a->getAnfitrionDocumento(), docAnfitrion) == 0) {
            hay = true;
            std::cout << "\n--- Reserva " << reservaciones[i].getCodigo() << " ---\n";
            std::cout << "Huesped: " << reservaciones[i].getDocumentoHuesped() << "\n";
            std::cout << "Alojamiento: " << reservaciones[i].getCodAlojamiento() << "\n";
            std::cout << "Fecha entrada: " << reservaciones[i].getFechaEntrada() << "\n";
            std::cout << "Duracion: " << reservaciones[i].getDuracion() << " dias\n";
            std::cout << "Pago: $" << reservaciones[i].getMonto() << " - Metodo: " << reservaciones[i].getMetodoPago() << "\n";
            std::cout << "Notas: " << reservaciones[i].getAnotaciones() << "\n";
        }
    }

    if (!hay) std::cout << "No hay reservas asociadas a sus alojamientos.\n";
}

void menuAnfitrion(const char* documento) {
    int op;
    do {
        std::cout << "\n--- Menu Anfitrion ---\n";
        std::cout << "1. Ver reservas en mis alojamientos\n";
        std::cout << "2. Anular una reserva\n";
        std::cout << "3. Consultar consumo\n";
        std::cout << "4. Volver al menu principal\nOpcion: ";
        std::cin >> op;

        switch (op) {
        case 1:
            verReservasDeAnfitrion(documento);
            break;
        case 2: {
            int codigo;
            std::cout << "Codigo de la reserva a anular: ";
            std::cin >> codigo;
            anularReservacion(codigo, documento);  // usa documento del anfitrión
            break;
        }
        case 3:
            mostrarConsumo();
            break;
        }
    } while (op != 4);
}

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
    std::cout << "No se encontro la reservación o no tiene permiso.\n";
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
    std::cout << "Historico actualizado.\n";
}

void cargarHuespedesDesdeArchivo() {
    FILE* archivo = fopen("huespedes.txt", "r");
    if (!archivo) return;

    char doc[20];
    int ant;
    float punt;

    while (fscanf(archivo, "%19[^,],%d,%f\n", doc, &ant, &punt) == 3 && totalHuespedes < MAX_HUESPEDES) {
        huespedes[totalHuespedes++] = Huesped(doc, ant, punt);
    }

    fclose(archivo);
}

void cargarAnfitrionesDesdeArchivo() {
    FILE* archivo = fopen("anfitriones.txt", "r");
    if (!archivo) return;

    char doc[20];
    int ant;
    float punt;

    while (fscanf(archivo, "%19[^,],%d,%f\n", doc, &ant, &punt) == 3 && totalAnfitriones < MAX_ANFITRIONES) {
        anfitriones[totalAnfitriones++] = Anfitrion(doc, ant, punt);
    }

    fclose(archivo);
}

void cargarAlojamientosDesdeArchivo() {
    FILE* archivo = fopen("alojamientos.txt", "r");
    if (!archivo) return;

    int cod;
    char docAnfitrion[20], municipio[30];
    float precio;

    while (fscanf(archivo, "%d,%19[^,],%29[^,],%f\n", &cod, docAnfitrion, municipio, &precio) == 4 &&
           totalAlojamientos < MAX_ALOJAMIENTOS) {
        alojamientos[totalAlojamientos++] = Alojamiento(cod, docAnfitrion, municipio, precio);
    }

    fclose(archivo);
}

void cargarReservasDesdeArchivo() {
    FILE* archivo = fopen("reservas.txt", "r");
    if (!archivo) return;

    int cod, codAloj, dur;
    float monto;
    char docH[20], fEntrada[11], metodo[10], fPago[11], notas[MAX_ANOTACIONES];

    while (fscanf(archivo, "%d,%d,%19[^,],%10[^,],%d,%9[^,],%10[^,],%f,%999[^\n]\n",
                  &cod, &codAloj, docH, fEntrada, &dur, metodo, fPago, &monto, notas) == 9 &&
           totalReservas < MAX_RESERVAS) {
        reservaciones[totalReservas++] = Reservacion(cod, codAloj, docH, fEntrada, dur, metodo, fPago, monto, notas);
    }

    fclose(archivo);
}


void guardarHuespedesEnArchivo() {
    FILE* archivo = fopen("huespedes.txt", "w");
    if (!archivo) return;

    for (int i = 0; i < totalHuespedes; ++i) {
        fprintf(archivo, "%s,%d,%.2f\n",
                huespedes[i].getDocumento(),
                huespedes[i].getAntiguedad(),
                huespedes[i].getPuntuacion());
    }

    fclose(archivo);
}

void guardarAnfitrionesEnArchivo() {
    FILE* archivo = fopen("anfitriones.txt", "w");
    if (!archivo) return;

    for (int i = 0; i < totalAnfitriones; ++i) {
        fprintf(archivo, "%s,%d,%.2f\n",
                anfitriones[i].getDocumento(),
                anfitriones[i].getAntiguedad(),
                anfitriones[i].getPuntuacion());
    }

    fclose(archivo);
}

void guardarAlojamientosEnArchivo() {
    FILE* archivo = fopen("alojamientos.txt", "w");
    if (!archivo) return;

    for (int i = 0; i < totalAlojamientos; ++i) {
        fprintf(archivo, "%d,%s,%s,%.2f\n",
                alojamientos[i].getCodigo(),
                alojamientos[i].getAnfitrionDocumento(),
                alojamientos[i].getMunicipio(),
                alojamientos[i].getPrecio());
    }

    fclose(archivo);
}

void guardarReservasEnArchivo() {
    FILE* archivo = fopen("reservas.txt", "w");
    if (!archivo) return;

    for (int i = 0; i < totalReservas; ++i) {
        fprintf(archivo, "%d,%d,%s,%s,%d,%s,%s,%.2f,%s\n",
                reservaciones[i].getCodigo(),
                reservaciones[i].getCodAlojamiento(),
                reservaciones[i].getDocumentoHuesped(),
                reservaciones[i].getFechaEntrada(),
                reservaciones[i].getDuracion(),
                reservaciones[i].getMetodoPago(),
                reservaciones[i].getFechaPago(),
                reservaciones[i].getMonto(),
                reservaciones[i].getAnotaciones());
    }

    fclose(archivo);
}


// ----------- MENÚ Y MAIN -----------

void menuPrincipal() {
    char documento[20];
    int opcion;

    do {
        std::cout << "\n--- UdeAStay ---\n";
        std::cout << "1. Ingresar como huesped\n";
        std::cout << "2. Ingresar como anfitrion\n";
        std::cout << "3. Salir\nOpcion: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            std::cout << "Documento del huesped: ";
            std::cin >> documento;
            if (buscarHuespedPorDocumento(documento)) {
                std::cout << "Bienvenido huesped.\n";
                menuHuesped(documento);
            } else std::cout << "No encontrado.\n";
            break;
        case 2:
            std::cout << "Documento del anfitrion: ";
            std::cin >> documento;
            if (buscarAnfitrionPorDocumento(documento)) {
                std::cout << "Bienvenido anfitrión.\n";
                menuAnfitrion(documento);  // llamada aquí
            } else std::cout << "No encontrado.\n";
            break;
        }
    } while (opcion != 3);
}
