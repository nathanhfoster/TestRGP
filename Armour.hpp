#ifndef ARMOUR_HPP
#define ARMOUR_HPP

#include "Item.hpp"

#include <string>

// Armour should also inherit item properties
class Armour : public Item
{
public:

	// Armour can go into one of three slots, and only one piece of
	// armour may occupy each slot at a time. The N is there to quickly
	// retrieve the number of slots, without having to use another variable
	enum Slot { TORSO, HEAD, LEGS, N };

	Slot slot;

	// See Battle class for exact formula, values from 1-50 are reasonable
	int defense;
	int armourClassBoost;
	int healthBoost;

	// Usual constructor
	Armour(std::string name, std::string description, int defense, int armourClassBoost, int healthBoost, Armour::Slot slot) :
		Item(name, description)
	{
		this->defense = defense;
		this->armourClassBoost = armourClassBoost;
		this->healthBoost = healthBoost;
		this->slot = slot;
	}

	Armour()
	{
	}
};

#endif /* ARMOUR_HPP */
