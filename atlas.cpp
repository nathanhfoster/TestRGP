#include "atlas.hpp"
void buildatlas_creature(std::vector<Creature>& atlas)
{
	// Fill the atlas
	// Creature(Name, Health, Armor Class, Attack Mod, Damage Mod, Str, End, Dex, Level, Type)
	atlas.push_back(Creature("Rat", 25, 13, 1, 1, 8, 8, 12, 1));
	return;
}

void buildatlas_item(std::vector<Item>& atlas)
{
	// Item(Name, Description)
	atlas.push_back(Item("Gold Coin", "A small disc made of lustrous metal"));
	atlas.push_back(Item("Iron Key", "A heavy iron key with a simple cut"));

	return;
}

void buildatlas_weapon(std::vector<Weapon>& atlas)
{
	// Weapon(Name, Description, Die Quatity, Die Sides, Hit Rate)
	atlas.push_back(Weapon("Iron Dagger", "A short blade made of iron with a leather-bound hilt", 1, 4, 1));
	atlas.push_back(Weapon("Long Sword", "A galvanized steel blade with ample length and a respectable point", 1, 6, 2));
	atlas.push_back(Weapon("Excalibur", "The legendary blade, bestowed upon you by the Lady of the Lake", 3, 6, 5));

	return;
}

void buildatlas_armour(std::vector<Armour>& atlas)
{
	// Armour(Name, Description, Defense, Slot)
	atlas.push_back(Armour("Leather Cuirass", "Torso armour made of tanned hide", 4, Armour::Slot::TORSO));

	return;
}

void buildatlas_area(std::vector<Area>& atlas,
	std::vector<Item>& items, std::vector<Weapon>& weapons,
	std::vector<Armour>& armour, std::vector<Creature>& creatures)
{
	// Area definitions are somewhat more complicated:
	atlas.push_back(Area(Dialogue(			// Standard dialogue definiton
		"You are in room 1",				// Description
		{ "Go to room 2", "Search" }),		// Choices
		Inventory(							// Area inventory
	{
		std::make_pair(&items[0], 5)	// Pair of item and quantity

	},
	{
		std::make_pair(&weapons[0], 1)	// Pair of weapon and quantity
	},
	{
		std::make_pair(&armour[0], 1)	// Pair of armour and quantity
	}),
	{									// Creatures
	}));

	atlas.push_back(Area(Dialogue(
		"You are in room 2",
		{ "Go to room 1", "Search" }),
		Inventory(
	{
		std::make_pair(&items[0], 10),
		std::make_pair(&items[1], 1)
	},
	{
	},
	{
	}),
	{
		&creatures[0]
	}));

	return;
}
