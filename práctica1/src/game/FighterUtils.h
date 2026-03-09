#pragma once

#include "FighterFacade.h"

class FighterUtils : public FighterFacade {
public:
	FighterUtils();
	virtual ~FighterUtils();

	void create_fighter() override;
	void reset_fighter() override;
	void reset_lives() override;
	int update_lives(int n) override;

private:
	ecs::Entity* fighter_;
};