#include "funciones.h"

// ------------- Variables globales ---------------

Huesped huespedes[MAX_HUESPEDES];
Anfitrion anfitriones[MAX_ANFITRIONES];
Alojamiento alojamientos[MAX_ALOJAMIENTOS];
Reservacion reservaciones[MAX_RESERVAS];

int totalHuespedes = 0, totalAnfitriones = 0, totalAlojamientos = 0, totalReservas = 0;
int contadorIteraciones = 0;

// -------------------------- Implementación de las clases --------------------------------

// Clase Huesped

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

// Clase Anfitrion

Anfitrion::Anfitrion() : antiguedad(0), puntuacion(0.0) { documento[0] = '\0'; }
Anfitrion::Anfitrion(const char* doc, int ant, float punt) : antiguedad(ant), puntuacion(punt) {
    strncpy(documento, doc, 20);
}

//const char* Anfitrion::getDocumento() const { return documento; }
//const char* Anfitrion::getAntiguedad() const { return documento; }
float Anfitrion::getPuntuacion() const { return puntuacion; }

int Anfitrion::getAntiguedad() const {
    return antiguedad;
}

const char* Anfitrion::getDocumento() const {
    return documento;
}

// Clase alojamiento

Alojamiento::Alojamiento() : codigo(0), precio(0.0), amenidades({false, false, false, false, false, false}) {
    nombre[0] = departamento[0] = municipio[0] = tipo[0] = direccion[0] = '\0';
}

Alojamiento::Alojamiento(int cod, const char* nom, const char* docAnfitrion, const char* dep, const char* muni, const char* t, const char* dir, float pr)
    : codigo(cod), precio(pr), amenidades({false, false, false, false, false, false}) {
    strncpy(nombre, nom, 100);
    strncpy(anfitrionDocumento, docAnfitrion, 20);
    strncpy(departamento, dep, 30);
    strncpy(municipio, muni, 30);
    strncpy(tipo, t, 10);
    strncpy(direccion, dir, 100);
}

int Alojamiento::getCodigo() const { return codigo; }
const char* Alojamiento::getAnfitrionDocumento() const { return anfitrionDocumento; }
const char* Alojamiento::getNombre() const { return nombre; }
const char* Alojamiento::getTipo() const { return tipo; }
const char* Alojamiento::getMunicipio() const { return municipio; }
float Alojamiento::getPrecio() const { return precio; }

bool Alojamiento::estaDisponible(const char* fechaEntrada, int duracion) const {
    tm entradaTm = {};
    if (!convertirFecha(fechaEntrada, entradaTm)) {
        return false;  // Si no se puede convertir la fecha, no está disponible
    }

    for (int i = 0; i < 365; ++i) {  // Comprobamos las 365 fechas reservadas
        if (fechasReservadas[i][0] == '\0') continue;  // Si no hay reserva en esa fecha

        tm fechaReserva = {};
        if (convertirFecha(fechasReservadas[i], fechaReserva)) {
            // Si hay solapamiento de fechas
            tm fechaFin = fechaReserva;
            fechaFin.tm_mday += duracion;  // Se suman los días de duración de la reserva
            if (entradaTm.tm_year == fechaReserva.tm_year && entradaTm.tm_mon == fechaReserva.tm_mon &&
                entradaTm.tm_mday >= fechaReserva.tm_mday && entradaTm.tm_mday <= fechaFin.tm_mday) {
                return false;  // Está reservado durante ese periodo
            }
        }
    }
    return true;
}

void Alojamiento::agregarReserva(const char* fechaEntrada, int duracion) {
    tm entradaTm = {};
    if (!convertirFecha(fechaEntrada, entradaTm)) {
        std::cerr << "Fecha inválida.\n";
        return;
    }

    // Agregar la fecha reservada
    for (int i = 0; i < 365; ++i) {
        if (fechasReservadas[i][0] == '\0') {
            snprintf(fechasReservadas[i], sizeof(fechasReservadas[i]), "%04d-%02d-%02d",
                     entradaTm.tm_year + 1900, entradaTm.tm_mon + 1, entradaTm.tm_mday);
            return;
        }
    }
    std::cerr << "No se pudo agregar la reserva, capacidad máxima alcanzada.\n";
}

void Alojamiento::mostrarInfo() const {
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Codigo: " << codigo << std::endl;
    std::cout << "Departamento: " << departamento << std::endl;
    std::cout << "Municipio: " << municipio << std::endl;
    std::cout << "Tipo: " << tipo << std::endl;
    std::cout << "Direccion: " << direccion << std::endl;
    std::cout << "Precio por noche: $" << precio << std::endl;
    std::cout << "Amenidades: ";
    std::cout << "Ascensor: " << (amenidades.ascensor ? "Sí" : "No") << ", ";
    std::cout << "Piscina: " << (amenidades.piscina ? "Sí" : "No") << ", ";
    std::cout << "Aire acondicionado: " << (amenidades.aireAcondicionado ? "Sí" : "No") << ", ";
    std::cout << "Caja fuerte: " << (amenidades.cajaFuerte ? "Sí" : "No") << ", ";
    std::cout << "Parqueadero: " << (amenidades.parqueadero ? "Sí" : "No") << ", ";
    std::cout << "Patio: " << (amenidades.patio ? "Sí" : "No") << std::endl;
}

float Alojamiento::getPuntuacionAnfitrion() const {
    // Suponiendo que `anfitrionDocumento` es el documento del anfitrión,
    // buscamos al anfitrión correspondiente en el array de anfitriones.
    for (int i = 0; i < totalAnfitriones; ++i) {
        if (strcmp(anfitrionDocumento, anfitriones[i].getDocumento()) == 0) {
            return anfitriones[i].getPuntuacion();  // Retorna la puntuación del anfitrión
        }
    }
    return 0.0f;  // Si no se encuentra, retornamos 0 (o algún valor predeterminado)
}

// Clase reservacion

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
const char* Reservacion::getEstado() const { return estado; }

void Reservacion::setEstado(const char* nuevoEstado) {
    strncpy(estado, nuevoEstado, sizeof(estado));
    estado[sizeof(estado) - 1] = '\0';  // Asegura terminación nula
}

const char* Reservacion::getDocumentoAnfitrion() {
    Alojamiento* a = buscarAlojamientoPorCodigo(codAlojamiento);
    return (a) ? a->getAnfitrionDocumento() : "desconocido";
}

// ------------------- Implementación de las funciones auxiliares --------------------------

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


// ----------------------------- Implementacion funciones principales ----------------------------

bool convertirFecha(const char* fecha, tm& fechaTm) {
    return sscanf(fecha, "%4d-%2d-%2d", &fechaTm.tm_year, &fechaTm.tm_mon, &fechaTm.tm_mday) == 3;
}

bool alojamientoOcupadoEnFecha(int codAlojamiento, const char* fecha) {
    for (int i = 0; i < totalReservas; ++i) {
        if (reservaciones[i].getCodAlojamiento() == codAlojamiento &&
            strcmp(reservaciones[i].getFechaEntrada(), fecha) == 0) {
            return true;
        }
    }
    return false;
}

bool codigoReservaExiste(int codigo) {
    for (int i = 0; i < totalReservas; ++i) {
        if (reservaciones[i].getCodigo() == codigo) {
            return true;
        }
    }
    return false;
}

bool fechaEsAnterior(const char* f1, const char* f2) {
    return strcmp(f1, f2) < 0;
}

// Reserva por codigo

void reservarAlojamientoPorCodigo(int codigo, const char* fechaEntrada, int duracion, const char* documentoHuesped) {
    Alojamiento* aloj = buscarAlojamientoPorCodigo(codigo);
    if (!aloj || !aloj->estaDisponible(fechaEntrada, duracion)) {
        std::cout << "Alojamiento no encontrado o no disponible.\n";
        return;
    }

    int codReserva;
    char metodoPago[10], fechaPago[11], anotaciones[MAX_ANOTACIONES];

    std::cout << "Codigo unico para la reserva: ";
    std::cin >> codReserva;

    std::cout << "Metodo de pago: ";
    std::cin >> metodoPago;

    std::cout << "Fecha de pago (YYYY-MM-DD): ";
    std::cin >> fechaPago;

    std::cin.ignore();  // limpiar buffer antes de leer anotaciones
    std::cout << "Anotaciones: ";
    std::cin.getline(anotaciones, MAX_ANOTACIONES);

    float monto = aloj->getPrecio() * duracion;

    reservaciones[totalReservas++] = Reservacion(
        codReserva, codigo, documentoHuesped, fechaEntrada, duracion,
        metodoPago, fechaPago, monto, anotaciones
        );

    aloj->agregarReserva(fechaEntrada, duracion);

    Huesped* h = buscarHuespedPorDocumento(documentoHuesped);
    if (h) h->agregarReserva(codReserva);

    guardarReservasEnArchivo();

    std::cout << "Reserva realizada con exito. Monto total: $" << monto << "\n";
}

void realizarReservaPorCodigo(const char* documentoHuesped) {
    int codigoAloj, duracion, codReserva;
    char fecha[11], metodoPago[10], fechaPago[11], anotaciones[MAX_ANOTACIONES];

    std::cout << "Ingrese el codigo del alojamiento a reservar: ";
    std::cin >> codigoAloj;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Entrada invalida. Cancelando reserva.\n";
        return;
    }

    Alojamiento* alojamiento = buscarAlojamientoPorCodigo(codigoAloj);
    if (!alojamiento) {
        std::cout << "Codigo de alojamiento no encontrado.\n";
        return;
    }

    std::cout << "Ingrese la fecha de entrada (YYYY-MM-DD): ";
    std::cin >> fecha;

    std::cout << "Duracion de la estancia (en dias): ";
    std::cin >> duracion;

    if (!alojamiento->estaDisponible(fecha, duracion)) {
        std::cout << "El alojamiento no esta disponible en esa fecha.\n";
        return;
    }

    // Validación de código único
    while (true) {
        std::cout << "Codigo unico para la reserva: ";
        std::cin >> codReserva;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Codigo invalido. Intente de nuevo.\n";
            continue;
        }

        if (!codigoReservaExiste(codReserva)) break;
        std::cout << "Ese codigo ya existe. Ingrese otro.\n";
    }

    std::cout << "Metodo de pago: ";
    std::cin >> metodoPago;

    std::cout << "Fecha de pago (YYYY-MM-DD): ";
    std::cin >> fechaPago;

    std::cin.ignore(); // Limpia el buffer antes de getline
    std::cout << "Anotaciones: ";
    std::cin.getline(anotaciones, MAX_ANOTACIONES);

    float monto = alojamiento->getPrecio() * duracion;

    reservaciones[totalReservas++] = Reservacion(
        codReserva, codigoAloj, documentoHuesped,
        fecha, duracion, metodoPago, fechaPago,
        monto, anotaciones
        );

    alojamiento->agregarReserva(fecha, duracion);

    Huesped* h = buscarHuespedPorDocumento(documentoHuesped);
    if (h) h->agregarReserva(codReserva);

    guardarReservasEnArchivo();

    std::cout << "Reserva completada. Monto total: $" << monto << "\n";
}


// Reserva por filtro

void buscarAlojamientosPorFiltros(const char* fechaEntrada, const char* municipio, int duracion, float precioMaximo) {
    bool encontrado = false;

    for (int i = 0; i < totalAlojamientos; ++i) {
        Alojamiento& alojamiento = alojamientos[i];

        if (strcmp(alojamiento.getMunicipio(), municipio) == 0 &&
            alojamiento.estaDisponible(fechaEntrada, duracion)) {

            if ((precioMaximo < 0 || alojamiento.getPrecio() <= precioMaximo)) {
                alojamiento.mostrarInfo();
                encontrado = true;
            }
        }
    }

    if (!encontrado) {
        std::cout << "No se encontraron alojamientos disponibles con esos filtros.\n";
    }
}


void realizarReservaPorFiltros(const char* documentoHuesped) {
    char fechaDeseada[11], municipio[30];
    int duracion;
    float precioMaximo = -1;

    std::cout << "Fecha deseada (YYYY-MM-DD): ";
    std::cin >> fechaDeseada;

    std::cout << "Municipio: ";
    std::cin >> municipio;

    std::cout << "Duracion de la estancia (en dias): ";
    std::cin >> duracion;

    char deseaFiltrar;
    std::cout << "¿Desea filtrar por precio maximo? (s/n): ";
    std::cin >> deseaFiltrar;

    if (deseaFiltrar == 's' || deseaFiltrar == 'S') {
        std::cout << "Precio maximo por noche: ";
        std::cin >> precioMaximo;
    }

    std::cout << "\nAlojamientos disponibles:\n";
    buscarAlojamientosPorFiltros(fechaDeseada, municipio, duracion, precioMaximo);

    int codigo;
    std::cout << "Ingrese el codigo del alojamiento que desea reservar (0 para cancelar): ";
    std::cin >> codigo;

    if (codigo != 0) {
        reservarAlojamientoPorCodigo(codigo, fechaDeseada, duracion, documentoHuesped);
    }
}

// Reservas del anfitrion

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

// Consumo

void mostrarConsumo() {
    std::cout << "Iteraciones: " << contadorIteraciones << "\n";
    std::cout << "Memoria: " << sizeof(Huesped) * totalHuespedes + sizeof(Anfitrion) * totalAnfitriones +
                                    sizeof(Alojamiento) * totalAlojamientos + sizeof(Reservacion) * totalReservas
              << " bytes\n";
}

// Anular reservacion

/*void anularReservacion(int codigo, const char* doc) {
    for (int i = 0; i < totalReservas; ++i) {
        contadorIteraciones++;
        if (reservaciones[i].getCodigo() == codigo) {
            if (strcmp(reservaciones[i].getDocumentoHuesped(), doc) == 0 ||
                strcmp(reservaciones[i].getDocumentoAnfitrion(), doc) == 0) {
                for (int j = i; j < totalReservas - 1; ++j)
                    reservaciones[j] = reservaciones[j + 1];
                totalReservas--;
                std::cout << "Reservacion anulada.\n";
                return;
            }
        }
    }
    std::cout << "No se encontro la reservacion o no tiene permiso.\n";
}*/

void anularReservacion(int codigo, const char* doc) {
    bool encontrada = false;
    for (int i = 0; i < totalReservas; ++i) {
        contadorIteraciones++;
        if (reservaciones[i].getCodigo() == codigo) {
            if (strcmp(reservaciones[i].getDocumentoHuesped(), doc) == 0 ||
                strcmp(reservaciones[i].getDocumentoAnfitrion(), doc) == 0) {

                reservaciones[i].setEstado("anulada");
                encontrada = true;
                break;
            }
        }
    }

    if (encontrada) {
        std::cout << "Reservacion marcada como anulada.\n";
        guardarReservasEnArchivo();  // actualiza el archivo con el nuevo estado
    } else {
        std::cout << "No se encontro la reservacion o no tiene permiso.\n";
    }
}

// Historico

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

// Funciones para carga

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

    char linea[5000];

    while (fgets(linea, sizeof(linea), archivo) && totalAlojamientos < MAX_ALOJAMIENTOS) {
        int cod;
        char nombre[100], docAnfitrion[20], departamento[30], municipio[30], tipo[10], direccion[100];
        float precio;
        int ascensor, piscina, aire, caja, parqueadero, patio;
        char fechasStr[4000];

        int leidos = sscanf(linea, "%d,%99[^,],%19[^,],%29[^,],%29[^,],%9[^,],%99[^,],%f,%d,%d,%d,%d,%d,%d,%3999[^\n]",
                            &cod, nombre, docAnfitrion, departamento, municipio, tipo, direccion, &precio,
                            &ascensor, &piscina, &aire, &caja, &parqueadero, &patio, fechasStr);

        if (leidos != 15) {
            printf("Error en el formato de la linea: %s\n", linea);
            continue;  // o break, dependiendo de qué tan crítico sea
        }

        Alojamiento& a = alojamientos[totalAlojamientos++];
        a = Alojamiento(cod, nombre, docAnfitrion, departamento, municipio, tipo, direccion, precio);

        // Aquí podrías tener un método setAmenidades si tienes un arreglo interno
        // a.setAmenidades({ascensor, piscina, aire, caja, parqueadero, patio});

        int index = 0;
        char* token = strtok(fechasStr, ";");
        while (token && index < 365) {
            a.setFechaReservada(index, token);
            token = strtok(nullptr, ";");
            ++index;
        }
    }

    fclose(archivo);
}

/*void cargarReservasDesdeArchivo() {
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
}*/

void cargarReservasDesdeArchivo() {
    FILE* archivo = fopen("reservas.txt", "r");
    if (!archivo) return;

    int cod, codAloj, dur;
    float monto;
    char docH[20], fEntrada[11], metodo[20], fPago[11], notas[MAX_ANOTACIONES], estado[10];

    while (fscanf(archivo, "%d,%d,%19[^,],%10[^,],%d,%19[^,],%10[^,],%f,%199[^,],%9[^\n]\n",
                  &cod, &codAloj, docH, fEntrada, &dur, metodo, fPago, &monto, notas, estado) == 10 &&
           totalReservas < MAX_RESERVAS) {

        Reservacion r(cod, codAloj, docH, fEntrada, dur, metodo, fPago, monto, notas);
        r.setEstado(estado);  // <--- Guardamos el estado leído
        reservaciones[totalReservas++] = r;
    }

    fclose(archivo);
}

// Funciones para escritura

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
        Alojamiento& a = alojamientos[i];

        fprintf(archivo, "%d,%s,%s,%s,%s,%s,%s,%.2f,%d,%d,%d,%d,%d,%d,",
                a.getCodigo(),
                a.getNombre(),
                a.getAnfitrionDocumento(),
                a.getDepartamento(),
                a.getMunicipio(),
                a.getTipo(),
                a.getDireccion(),
                a.getPrecio());

                bool amenidades[6];
                    for (int j = 0; j < 6; ++j) {
                        int val;
                        fscanf(archivo, ",%d", &val);
                        amenidades[j] = (val != 0);
                    }
                //a.setAmenidades(amenities);


        // Guardar fechas usando el getter
        bool first = true;
        for (int j = 0; j < 365; ++j) {
            if (a.getFechaReservada(j)[0] != '\0') {
                if (!first) fprintf(archivo, ";");
                fprintf(archivo, "%s", a.getFechaReservada(j));
                first = false;
            }
        }


        fprintf(archivo, "\n");
    }

    fclose(archivo);
}

/*void guardarReservasEnArchivo() {
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
}*/

void guardarReservasEnArchivo() {
    FILE* archivo = fopen("reservas.txt", "w");
    if (!archivo) return;

    char anotacionesLimpias[200];
    for (int i = 0; i < totalReservas; ++i) {
        // Copiar anotaciones y reemplazar comas por punto y coma
        strncpy(anotacionesLimpias, reservaciones[i].getAnotaciones(), sizeof(anotacionesLimpias));
        anotacionesLimpias[sizeof(anotacionesLimpias) - 1] = '\0';
        for (int j = 0; anotacionesLimpias[j] != '\0'; ++j) {
            if (anotacionesLimpias[j] == ',') {
                anotacionesLimpias[j] = ';';
            }
        }

        fprintf(archivo, "%d,%d,%s,%s,%d,%s,%s,%.2f,%s,%s\n",
                reservaciones[i].getCodigo(),
                reservaciones[i].getCodAlojamiento(),
                reservaciones[i].getDocumentoHuesped(),
                reservaciones[i].getFechaEntrada(),
                reservaciones[i].getDuracion(),
                reservaciones[i].getMetodoPago(),
                reservaciones[i].getFechaPago(),
                reservaciones[i].getMonto(),
                anotacionesLimpias,
                reservaciones[i].getEstado());
    }

    fclose(archivo);
}


// ------------------------ Opciones de Menú ---------------------------

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
                std::cout << "Bienvenido anfitrion.\n";
                menuAnfitrion(documento);  // llamada aquí
            } else std::cout << "No encontrado.\n";
            break;
        }
    } while (opcion != 3);
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

void menuHuesped(const char* documento) {
    int op;
    do {
        std::cout << "\n--- Menu Huesped ---\n";
        std::cout << "1. Realizar una reserva por filtros\n";
        std::cout << "2. Realizar una reserva por codigo\n";
        std::cout << "3. Anular una reserva\n";
        std::cout << "4. Consultar consumo\n";
        std::cout << "5. Volver al menu principal\nOpcion: ";
        std::cin >> op;

        switch (op) {
        case 1:
            realizarReservaPorFiltros(documento);
            break;
        case 2:
            realizarReservaPorCodigo(documento);
            break;
        case 3: {
            int codigo;
            std::cout << "Codigo de la reserva a anular: ";
            std::cin >> codigo;
            anularReservacion(codigo, documento);
            break;
        }
        case 4:
            mostrarConsumo();
            break;
        }
    } while (op != 5);
}
