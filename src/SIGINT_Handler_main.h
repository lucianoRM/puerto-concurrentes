#ifndef SIGINT_HANDLER_main_H_
#define SIGINT_HANDLER_main_H_

#include <signal.h>
#include <assert.h>

#include "EventHandler.h"
#include "Logger.h"
#include <vector>

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
			Logger::getInstance()->log("[MAIN] Me llego SIGINT");
			std::vector<pid_t>::iterator it = this->children.begin();
			for (;it != this->children.end(); it++) {
				pid_t pid = *it;
				Logger::getInstance()->log("[MAIN] kill(" + std::to_string(pid) + ")");
				kill(pid, SIGINT);
				Logger::getInstance()->log("[MAIN] waitpid(" + std::to_string(pid) + ")");
				waitpid(pid, NULL, 0);
			}
			return 0;
		}
};

#endif /* SIGINT_HANDLER_H_ */
