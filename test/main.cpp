#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "card.h"
#include "card_manager.h"
#include "network_manager.h"
#include "core.h"

int main(int argc, char** argv) {
	Core c;
	c.run();
	return 0;
}
