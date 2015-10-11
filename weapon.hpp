#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "item.hpp"

#include <string>

// Weapons are items, so they should inherit their properties
class Weapon : public Item
{
public:

	// Weapon damage. 
	// Subscript [0] holds the quantity of dice.
	// Subscript [1] holds the amount of sides of the dice being rolled.
	unsigned damage[2]; 

	// Modifier to hit chance. Small values are encouraged, e.g.
	// 5-30%
	int attackMod;

	// Pass inherited qualities to the normal item constructor
	Weapon(std::string name, std::string description, int dieQuatity, int dieSides, int attackMod) :
		Item(name, description)
	{
		this->damage[0] = dieQuatity;
		this->damage[1] = dieSides;
		this->attackMod = attackMod;
	}

	Weapon()
	{
	}
};

#endif /* WEAPON_HPP */
