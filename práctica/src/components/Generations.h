#ifndef GENERATIONS_H
#define GENERATIONS_H

#include "../ecs/Component.h"

class Generations : public ecs::Component {
	public:
		Generations(int g);
		virtual ~Generations();

		int getGenerations() const;
		void setGenerations(int g);

	private:
		int _generations;
};

#endif // GENERATIONS_H