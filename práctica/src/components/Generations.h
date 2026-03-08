#ifndef GENERATIONS_H
#define GENERATIONS_H

#include "../ecs/Component.h"

class Generations : public ecs::Component {
	public:
		__CMPID_DECL__(ecs::cmp::GENERATIONS)
		Generations(int g);
		virtual ~Generations();

		int getGenerations() const;
		void setGenerations(int g);

	private:
		int _generations;
};

#endif // GENERATIONS_H