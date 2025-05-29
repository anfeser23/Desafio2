#include "funciones.h"
using namespace std;


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
    cout << endl;
    cout << endl << "Nombre: " << nombre << endl;
    cout << endl << "Codigo: " << codigo << endl;
    cout << endl << "Departamento: " << departamento << endl;
    cout << endl << "Municipio: " << municipio << endl;
    cout << endl << "Tipo: " << tipo << endl;
    cout << endl << "Direccion: " << direccion << endl;
    cout << endl << "Precio por noche: $" << precio << endl;
    cout << endl << "Amenidades: ";
    cout << endl << "Ascensor: " << (amenidades.ascensor ? "Sí" : "No") << ", ";
    cout << endl << "Piscina: " << (amenidades.piscina ? "Sí" : "No") << ", ";
    cout << endl << "Aire acondicionado: " << (amenidades.aireAcondicionado ? "Sí" : "No") << ", ";
    cout << endl << "Caja fuerte: " << (amenidades.cajaFuerte ? "Sí" : "No") << ", ";
    cout << endl << "Parqueadero: " << (amenidades.parqueadero ? "Sí" : "No") << ", ";
    cout << endl << "Patio: " << (amenidades.patio ? "Sí" : "No") << endl;
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


// ----------------------------- Implementacion funciones principales ----------------------------



// Reserva por codigo

void reservarAlojamientoPorCodigo(int codigo, const char* fechaEntrada, int duracion, const char* documentoHuesped) {
    Alojamiento* aloj = buscarAlojamientoPorCodigo(codigo);
    if (!aloj || !aloj->estaDisponible(fechaEntrada, duracion)) {
        std::cout << "Alojamiento no encontrado o no disponible.\n";
        return;
    }

    int codReserva;
    char metodoPago[10], fechaPago[11], anotaciones[MAX_ANOTACIONES];

    cout << endl << "Codigo unico para la reserva: ";
    cin >> codReserva;

    cout << endl << "Metodo de pago: ";
    cin >> metodoPago;

    cout << endl << "Fecha de pago (YYYY-MM-DD): ";
    cin >> fechaPago;

    cin.ignore();  // limpiar buffer antes de leer anotaciones
    cout << endl << "Anotaciones: ";
    cin.getline(anotaciones, MAX_ANOTACIONES);

    float monto = aloj->getPrecio() * duracion;

    /*reservaciones[totalReservas++] = Reservacion(
        codReserva, codigo, documentoHuesped, fechaEntrada, duracion,
        metodoPago, fechaPago, monto, anotaciones
        );*/

    Reservacion* nuevaReser = new Reservacion (
        codReserva, codigo, documentoHuesped, fechaEntrada, duracion,
        metodoPago, fechaPago, monto, anotaciones
        );

    reservaciones[totalReservas++] = * nuevaReser;

    aloj->agregarReserva(fechaEntrada, duracion);

    Huesped* h = buscarHuespedPorDocumento(documentoHuesped);
    if (h) h->agregarReserva(codReserva);

    guardarReservasEnArchivo();

    cout << endl << "Reserva realizada con exito. Monto total: $" << monto << "\n";

    delete nuevaReser;
}

void realizarReservaPorCodigo(const char* documentoHuesped) {
    int codigoAloj, duracion, codReserva;
    char fecha[11], metodoPago[10], fechaPago[11], anotaciones[MAX_ANOTACIONES];

    cout << endl << "Ingrese el codigo del alojamiento a reservar: ";
    cin >> codigoAloj;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << endl << "Entrada invalida. Cancelando reserva.\n";
        return;
    }

    Alojamiento* alojamiento = buscarAlojamientoPorCodigo(codigoAloj);

    if (!alojamiento) {
        cout << endl << "Codigo de alojamiento no encontrado.\n";
        return;
    }

    cout << endl << "Ingrese la fecha de entrada (YYYY-MM-DD): ";
    cin >> fecha;

    cout << endl << "Duracion de la estancia (en dias): ";
    cin >> duracion;

    if (!alojamiento->estaDisponible(fecha, duracion)) {
        cout << endl << "El alojamiento no esta disponible en esa fecha.\n";
        return;
    }

    if (!alojamiento || alojamientoOcupadoEnRango(codigoAloj, fecha, duracion)) {
        cout << endl << "El alojamiento no esta disponible en esa fecha.\n";
        return;
    }

    // Validación de código único
    while (true) {
        cout << endl << "Codigo unico para la reserva: ";
        cin >> codReserva;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << endl << "Codigo invalido. Intente de nuevo.\n";
            continue;
        }

        if (!codigoReservaExiste(codReserva)) break;
        cout << endl << "Ese codigo ya existe. Ingrese otro.\n";
    }

    cout << endl << "Metodo de pago: ";
    cin >> metodoPago;

    cout << endl << "Fecha de pago (YYYY-MM-DD): ";
    cin >> fechaPago;

    cin.ignore(); // Limpia el buffer antes de getline
    cout << endl << "Anotaciones: ";
    cin.getline(anotaciones, MAX_ANOTACIONES);

    float monto = alojamiento->getPrecio() * duracion;

    /*reservaciones[totalReservas++] = Reservacion(
        codReserva, codigoAloj, documentoHuesped,
        fecha, duracion, metodoPago, fechaPago,
        monto, anotaciones
        );*/

    Reservacion* nuevaReserva = new Reservacion (
        codReserva, codigoAloj, documentoHuesped,
        fecha, duracion, metodoPago, fechaPago,
        monto, anotaciones
        );

    reservaciones[totalReservas++] = *nuevaReserva;

    alojamiento->agregarReserva(fecha, duracion);

    Huesped* h = buscarHuespedPorDocumento(documentoHuesped);
    if (h) h->agregarReserva(codReserva);

    guardarReservasEnArchivo();

    cout << endl << "Reserva completada. Monto total: $" << monto << "\n";

    delete nuevaReserva;
}

bool alojamientoOcupadoEnRango(int codAlojamiento, const char* fechaNueva, int duracionNueva) {
    tm entradaNueva = {};
    if (!convertirFecha(fechaNueva, entradaNueva)) return true; // Si no se puede interpretar, consideramos ocupado

    time_t tEntradaNueva = mktime(&entradaNueva);
    time_t tFinNueva = tEntradaNueva + duracionNueva * 86400; // segundos por día

    for (int i = 0; i < totalReservas; ++i) {
        if (reservaciones[i].getCodAlojamiento() != codAlojamiento) continue;

        tm entradaExistente = {};
        if (!convertirFecha(reservaciones[i].getFechaEntrada(), entradaExistente)) continue;

        time_t tEntradaExistente = mktime(&entradaExistente);
        time_t tFinExistente = tEntradaExistente + reservaciones[i].getDuracion() * 86400;

        // Verificar solapamiento
        if (tEntradaNueva < tFinExistente && tFinNueva > tEntradaExistente) {
            return true;  // Hay cruce
        }
    }

    return false;  // No hay conflictos
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
        cout << endl << "No se encontraron alojamientos disponibles con esos filtros.\n";
    }
}


void realizarReservaPorFiltros(const char* documentoHuesped) {
    char fechaDeseada[11], municipio[30];
    int duracion;
    float precioMaximo = -1;

    cout << endl << "Fecha deseada (YYYY-MM-DD): ";
    cin >> fechaDeseada;

    cout << endl << "Municipio: ";
    cin >> municipio;

    cout << endl << "Duracion de la estancia (en dias): ";
    cin >> duracion;

    char deseaFiltrar;
    cout << endl << "¿Desea filtrar por precio maximo? (s/n): ";
    cin >> deseaFiltrar;

    if (deseaFiltrar == 's' || deseaFiltrar == 'S') {
        cout << endl << "Precio maximo por noche: ";
        cin >> precioMaximo;
    }

    cout << endl << "\nAlojamientos disponibles:\n";
    buscarAlojamientosPorFiltros(fechaDeseada, municipio, duracion, precioMaximo);

    int codigo;
    cout << endl << "Ingrese el codigo del alojamiento que desea reservar (0 para cancelar): ";
    cin >> codigo;

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
            cout << endl << "\n--- Reserva " << reservaciones[i].getCodigo() << " ---\n";
            cout << endl << "Huesped: " << reservaciones[i].getDocumentoHuesped() << "\n";
            cout << endl << "Alojamiento: " << reservaciones[i].getCodAlojamiento() << "\n";
            cout << endl << "Fecha entrada: " << reservaciones[i].getFechaEntrada() << "\n";
            cout << endl << "Duracion: " << reservaciones[i].getDuracion() << " dias\n";
            cout << endl << "Pago: $" << reservaciones[i].getMonto() << " - Metodo: " << reservaciones[i].getMetodoPago() << "\n";
            cout << endl << "Notas: " << reservaciones[i].getAnotaciones() << "\n";
        }
    }

    if (!hay) std::cout << "No hay reservas asociadas a sus alojamientos.\n";
}

// Consumo

void mostrarConsumo() {
    cout << endl << "Iteraciones: " << contadorIteraciones << "\n";
    cout << endl << "Memoria: " << sizeof(Huesped) * totalHuespedes + sizeof(Anfitrion) * totalAnfitriones +
                               sizeof(Alojamiento) * totalAlojamientos + sizeof(Reservacion) * totalReservas
         << " bytes\n";
}

// Anular reservación

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
        cout << endl << "Reservacion marcada como anulada.\n";
        guardarReservasEnArchivo();  // actualiza el archivo con el nuevo estado
    } else {
        cout << endl << "No se encontro la reservacion o no tiene permiso.\n";
    }
}

// Historico

void actualizarHistorico(const char* hoy) {
    FILE* archivo = fopen("historico_reservas.txt", "a");
    if (!archivo) {
        std::cerr << "Error: no se pudo abrir el archivo 'historico_reservas.txt'." << std::endl;
        return;
    }

    for (int i = 0; i < totalReservas;) {
        contadorIteraciones++;
        if (fechaEsAnterior(reservaciones[i].getFechaEntrada(), hoy)) {
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
            for (int j = i; j < totalReservas - 1; ++j)
                reservaciones[j] = reservaciones[j + 1];
            totalReservas--;
        } else {
            i++;
        }
    }

    fclose(archivo);
    std::cout << "\nHistorico actualizado.\n";
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

    char fechasStr[4000];

    Reservacion& a = reservaciones[totalReservas++];
    a = Reservacion(cod, codAloj, docH, fEntrada, dur, metodo, fPago, monto, notas);

    int index = 0;
    char* token = strtok(fechasStr, ";");
    while (token && index < 365) {
        a.setFechaReservadaR(index, token);
        token = strtok(nullptr, ";");
        ++index;
    }

    fclose(archivo);
}

// Funciones para escritura

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
        cout << endl;
        cout << endl << "\n---------------- UdeAStay --------------------\n" << endl;
        cout << endl << "1. Ingresar como huesped\n";
        cout << endl << "2. Ingresar como anfitrion\n";
        cout << endl << "3. Salir\n" << endl;
        cout << endl << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << endl << "Documento del huesped: ";
            cin >> documento;
            if (buscarHuespedPorDocumento(documento)) {
                cout << endl << "Bienvenido huesped.\n";
                menuHuesped(documento);
            } else cout << endl << "No encontrado.\n";
            break;
        case 2:
            cout << endl << "Documento del anfitrion: ";
            cin >> documento;
            if (buscarAnfitrionPorDocumento(documento)) {
                cout << endl << "Bienvenido anfitrion.\n";
                menuAnfitrion(documento);  // llamada aquí
            } else cout << endl << "No encontrado.\n";
            break;
        }
    } while (opcion != 3);
}

void menuAnfitrion(const char* documento) {
    int op;
    do {
        cout << endl;
        cout << endl << "\n--- Menu Anfitrion ---\n" << endl;
        cout << endl << "1. Ver reservas en mis alojamientos\n";
        cout << endl << "2. Anular una reserva\n";
        cout << endl << "3. Consultar consumo\n";
        cout << endl << "4. Volver al menu principal\n" << endl;
        cout << endl << "Opcion: ";
        cin >> op;

        switch (op) {
        case 1:
            verReservasDeAnfitrion(documento);
            break;
        case 2: {
            int codigo;
            cout << endl << "Codigo de la reserva a anular: ";
            cin >> codigo;
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
        cout << endl;
        cout << endl << "\n--- Menu Huesped ---\n" << endl;
        cout << endl << "1. Realizar una reserva por filtros\n";
        cout << endl << "2. Realizar una reserva por codigo\n";
        cout << endl << "3. Anular una reserva\n";
        cout << endl << "4. Consultar consumo\n";
        cout << endl << "5. Volver al menu principal\n" <<  endl;
        cout << endl << "Opcion: ";
        cin >> op;

        switch (op) {
        case 1:
            realizarReservaPorFiltros(documento);
            break;
        case 2:
            realizarReservaPorCodigo(documento);
            break;
        case 3: {
            int codigo;
            cout << endl << "Codigo de la reserva a anular: ";
            cin >> codigo;
            anularReservacion(codigo, documento);
            break;
        }
        case 4:
            mostrarConsumo();
            break;
        }
    } while (op != 5);
}
