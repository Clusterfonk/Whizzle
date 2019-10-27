#include <iostream>
#include "card.h"
#include "card_manager.h"
#include "network_manager.h"
#include "core.h"

int main(int argc, char** argv) {
	std::cout << "first initialisiation" << std::endl;
	Card_Manager c_m;
	std::cout << "next initialisiation" << std::endl;
	Card_Manager c_b;

	Network_Manager n_m;
	n_m.connect("123");
	//n_m.listen();
	
	Core c;
	c.run();
	return 0;
}
