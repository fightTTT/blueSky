#pragma once
#include "Character.h"

class Ryu :
	public Character
{
public:
	Ryu();
	~Ryu();

	void SetMove(const GameCtrl &ctl) override;
};
