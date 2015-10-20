#ifndef SIGINT_HANDLER_H_
#define SIGINT_HANDLER_H_

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"
#include "Logger.h"

class SIGINT_Handler : public EventHandler {

	private:
		sig_atomic_t gracefulQuit;

	public:

		SIGINT_Handler () :
				gracefulQuit(0)
		{}

		~SIGINT_Handler () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGINT );
			this->gracefulQuit = 1;
			Logger::getInstance()->log("ME LLEGO LA SEÑAL PARA MORIR");
			return 0;
		}

		sig_atomic_t getGracefulQuit () const {
			return this->gracefulQuit;
		}

};

#endif /* SIGINT_HANDLER_H_ */
