#include "C.h"
#include <fstream>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

json read() {
	ifstream file("conf.json");
	string content;
	content.assign((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
	return json::parse(content);
}
json C::conf = read();

unsigned int C::barcos = (unsigned int) C::conf["barcos"];
unsigned int C::camiones = (unsigned int) C::conf["camiones"];
unsigned int C::gruas = (unsigned int) C::conf["gruas"];
unsigned int C::lugares = (unsigned int) C::conf["lugares"];
