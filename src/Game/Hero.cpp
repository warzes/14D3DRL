#include "stdafx.h"
#include "Hero.h"

int GetNextExp(int level)
{
	float exponent = 1.5f;
	float baseXP = 100.0;
	return floor(baseXP * pow((float)level, exponent));
}

Hero::Hero()
{
	NextExp = GetNextExp(Level);
	Hp = Level * 5 + Con * 5;
	MoveSpeed = 50 + Dex;
}