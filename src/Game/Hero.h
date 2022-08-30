#pragma once

int GetNextExp(int level);

class Hero
{
public:
	Hero();

	int Level = 1;
	int Exp = 0;
	int NextExp = 0;
	int Hp = 0;
	int Gold = 0;
	int Food = 100;
	int MoveSpeed = 50;

	int Str = 1;
	int Def = 1;
	int Con = 1;
	int Dex = 1;
};