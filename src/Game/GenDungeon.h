#pragma once

#include "Generator.h"

class Dungeon : public Generator
{
protected:
	void placeDoors(int doorProb);
	void removeCorridors();
};

class ClassicDungeon : public Dungeon
{
private:
	void onGenerate() override;
};

class BSPDugeon : public Dungeon
{
private:
	void onGenerate() override;
};

class RoomsAndMazes : public Dungeon
{
private:
	void onGenerate() override;
};