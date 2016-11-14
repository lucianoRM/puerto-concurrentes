#include "Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	mknod ( static_cast<const char*>(nombre.c_str()),S_IFIFO|0666,0 );

}

Fifo::~Fifo() {
}

void Fifo::cerrar() {
	fd = -1;
	close ( fd );
}

void Fifo::eliminar() const {
	unlink ( nombre.c_str() );
}
