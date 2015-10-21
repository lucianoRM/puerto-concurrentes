#ifndef CONCU_TP1_SHAREDMEMORY_H
#define CONCU_TP1_SHAREDMEMORY_H

#include "LockFile.h"
#include "MemoriaCompartida2.h"

template<class T> class SharedMemory {
public:
	SharedMemory(const std::string &archivo, const char letra);
	~SharedMemory();
	void escribir(const T &dato);
	T leer() const;
private:
	MemoriaCompartida2<T> *buffer = NULL;
	LockFile *lockFile;
	void escribirInseguro(const T &dato);
	T leerInseguro() const;
};

template<class T> void SharedMemory<T>::escribirInseguro(const T &dato) {
	buffer->escribir(dato);
}

template<class T> T SharedMemory<T>::leerInseguro() const {
	T dato = buffer->leer();
	return dato;
}

template<class T> void SharedMemory<T>::escribir(const T &dato) {
	if (lockFile->tomarLock() == 0) {
		escribirInseguro(dato);
		lockFile->liberarLock();
	} else {
		// TODO: Loggear error
	}
}

template<class T> T SharedMemory<T>::leer() const {
	T dato;
	int res = this->lockFile->tomarLock();
	if (res <= 0 ) {
		std::string err = strerror(errno);
		Logger::getInstance()->log("Error al tomar el lock de la caja " + err);
		EndProcessException e;
		throw e;
	}
	dato = leerInseguro();
	this->lockFile->liberarLock();

	return dato;
}

template<class T> SharedMemory<T>::SharedMemory(const std::string &archivo,
		const char letra) {
	lockFile = new LockFile(archivo);
	buffer = new MemoriaCompartida2<T>(archivo, letra);
}

template<class T> SharedMemory<T>::~SharedMemory() {
	delete buffer;
	delete lockFile;
}

#endif
