#include "Semaforo.h"

Semaforo::Semaforo (const std::string& nombre, const int valorInicial):valorInicial(valorInicial), nombre(nombre) {

	key_t clave = ftok(nombre.c_str(), 'a');

	if (clave == -1) {
		std::string err = strerror(errno);
		Logger::getInstance()->log("[Semaforo] [" + nombre + "] Error obteniendo el token! " + err);
		exit(1);
	}

	this->id = semget(clave, 1, 0666 | IPC_CREAT);

	if (this->id == -1) {
		std::string err = strerror(errno);
		Logger::getInstance()->log("[Semaforo] [" + nombre + "] Error obteniendo el semaforo! " + err);
		exit(1);
	}

	int res = this->inicializar();

	if (res == -1) {
		std::string err = strerror(errno);
		Logger::getInstance()->log("[Semaforo] [" + nombre + "] Error inicializando el semaforo! " + err);
		exit(1);
	}
}

Semaforo::~Semaforo() {
}

int Semaforo::inicializar() const {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = this->valorInicial;

	int resultado = semctl(this->id, 0, SETVAL, init);

	return resultado;
}

int Semaforo::p() const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = -1;	// restar 1 al semaforo
	operacion.sem_flg = 0;

	int resultado = semop(this->id, &operacion, 1);
	return resultado;
}

int Semaforo::v() const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = 1;	// sumar 1 al semaforo
	operacion.sem_flg = 0;

	int resultado = semop(this->id, &operacion, 1);
	return resultado;
}

void Semaforo::eliminar() const {

	int res = semctl(this->id, 0, IPC_RMID);
	if (res == -1) {
		std::string err = strerror(errno);
		Logger::getInstance()->log("[Semaforo] [" + this->nombre + "] Error eliminando el semaforo! " + err);
		exit(1);
	}
}
