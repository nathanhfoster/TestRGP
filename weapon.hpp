#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "item.hpp"

#include <string>

// Weapons are items, so they should inherit their properties
class Weapon : public Item
{
public:

	// Weapon damage. See the Battle class for formula, but values
	// between 1-50 are reasonable
	unsigned damage;

	// Modifier to hit chance. Small values are encouraged, e.g.
	// 5-30%
	double hitRate;

	// Pass inherited qualities to the normal item constructor
	Weapon(std::string name, std::string description, int damage, double hitRate) :
		Item(name, description)
	{
		this->damage = damage;
		this->hitRate = hitRate;
	}

	Weapon()
	{
	}
};

#endif /* WEAPON_HPP */
