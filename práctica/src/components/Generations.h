#pragma once
#include "../ecs/Component.h"

class Generations : public ecs::Component {
	public:
		Generations(int g);
		virtual ~Generations();

		int getGenerations();
		void setGenerations(int g);

	private:
		int _generations;
};