//
// Created by agustin on 28/9/15.
//

#ifndef C_H_
#define C_H_

#include "json.hpp"

using json = nlohmann::json;

json readConf();

class C {
public:
	static unsigned int camiones;
	static unsigned int barcos;
	static unsigned int lugares;
	static unsigned int gruas;
	static float logTime;
private:
	static json conf;
};

#endif // C_H_
