#include "Generations.h"

Generations::Generations(int g) : 
	_generations(g)

	{}
Generations:: ~Generations() {
}

int Generations:: getGenerations() const{
	return _generations;
}
void Generations:: setGenerations(int g) {
	_generations = g;
}


	