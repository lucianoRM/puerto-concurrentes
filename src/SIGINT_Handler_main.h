#ifndef SIGINT_HANDLER_main_H_
#define SIGINT_HANDLER_main_H_

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"
#include "Logger.h"

class SIGINT_Handler_main : public EventHandler {

	private:
		std::vector<pid_t> children;

	public:

		SIGINT_Handler_main (std::vector<pid_t> pids) {
			this->children = pids;
		}

		~SIGINT_Handler_main () {
		}

		virtual int handleSignal ( int signum ) {
			assert ( signum == SIGINT );
			std::vector<pid_t>::iterator it = this->children.begin();
			for (;it != this->children.end(); it++) {
				pid_t pid = *it;
				Logger::getInstance()->log("Soy el master y estoy terminando a [" + std::to_string(pid) + "]");
				kill(pid, SIGINT);
			}
			return 0;
		}
};

#endif /* SIGINT_HANDLER_H_ */
