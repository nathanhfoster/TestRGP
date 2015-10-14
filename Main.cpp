#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <utility>
#include <cstdlib>
#include <ctime>

#include "Atlas.hpp"
#include "Item.hpp"
#include "Weapon.hpp"
#include "Armour.hpp"
#include "Inventory.hpp"
#include "Creature.hpp"
#include "Dialogue.hpp"
#include "Area.hpp"
#include "Battle.hpp"
// New character menu
Creature dialogue_newchar();

// Character information menu, displays the items the player has, their
// current stats etc.
void dialogue_menu(Creature& player);

int main(void)
{
	std::vector<Creature> creatureAtlas;
	std::vector<Item> itemAtlas;
	std::vector<Weapon> weaponAtlas;
	std::vector<Armour> armourAtlas;
	std::vector<Area> areaAtlas;

	Creature player;

	// Build the atlases
	buildatlas_creature(creatureAtlas);
	buildatlas_item(itemAtlas);
	buildatlas_weapon(weaponAtlas);
	buildatlas_armour(armourAtlas);
	buildatlas_area(areaAtlas, itemAtlas, weaponAtlas, armourAtlas, creatureAtlas);

	// Seed the random number generator with the system time, so the
	// random numbers produced by rand() will be different each time
	srand(time(NULL));

	// Main game menu dialogue
	int result = Dialogue(
		"Welcome!",
		{ "New Game" }).activate();

	switch (result)
	{
	case 1: player = dialogue_newchar(); break;
	default: return 0; break;
	}

	// Set the current area to be the first area in the atlas, essentially
	// placing the player there upon game start
	Area* currentArea = &(areaAtlas[0]);

	// Play the game until a function breaks the loop and closes it
	while (1)
	{
		// If the player has died then inform them as such and close
		// the program
		if (player.health <= 0)
		{
			std::cout << "\t----YOU DIED----\n    Game Over\n";
			return 0;
		}

		// If the area the player is in has any creatures inside it,
		// then begin a battle with the last creature in the list
		// before moving on the next one. This makes the creature
		// list act like a stack
		if (currentArea->creatures.size() > 0)
		{
			for (int i = currentArea->creatures.size() - 1; i >= 0; --i)
			{
				Battle(&player, currentArea->creatures[i]).run();
				// the creature from the area. This is fine to do
				// because if the player wins the creature will not respawn,
				// and if the creature wins the player isn't around to see it
				// (This does break the 'non-mutable' feature of the atlases,
				// but doing so saves a lot of memory, as we don't need to keep
				// two versions of each area)
				currentArea->creatures.pop_back();
			}
		}

		// Activate the current area's dialogue
		result = currentArea->dialogue.activate();

		// These could be moved inside of the area code using an event
		// style system, but that allows for much less flexibility with
		// what happens in each area. Since we're defining the areas in
		// code anyway, sticking with this isn't too much of a problem,
		// and it keeps things easy to understand
		if (currentArea == &(areaAtlas[0]))
		{
			switch (result)
			{
				// Open the menu
			case 0:
				dialogue_menu(player);
				break;
			case 1:
				// Move to area 1
				currentArea = &(areaAtlas[1]);
				break;
			case 2:
				// Search the area
				currentArea->search(player);
				break;
			default:
				break;
			}
		}
		else if (currentArea == &(areaAtlas[1]))
		{
			switch (result)
			{
				// Open the menu
			case 0:
				dialogue_menu(player);
				break;
				// Move to area 0
			case 1:
				currentArea = &(areaAtlas[0]);
				break;
				// Search the area
			case 2:
				currentArea->search(player);
				break;
			default:
				break;
			}
		}
	}

	return 0;
}

// Create a new character
Creature dialogue_newchar()
{
	// Ask for a name and class
	// Name does not use a dialogue since dialogues only request options,
	// not string input. Could be generalised into its own TextInput
	// class, but not really necessary
	std::cout << "Choose your name" << std::endl;
	std::string name;
	std::cin >> name;

	int result = Dialogue(
		"Choose your class",
		{ "Fighter", "Rogue" }).activate();

	switch (result) // Creature(Name, Health, Armour Class, Attack Mod, Damage Mod, Str, End, Dex, Level, Type)
	{
		// Fighter class favours health and strength
	case 1:
		return Creature(name, 35, 16, 1, 1, 16, 10, 12, 1, "Fighter");
		break;

		// Rogue class favours dexterity and hit rate
	case 2:
		return Creature(name, 30, 14, 1, 1, 14, 10, 20, 1, "Rogue");
		break;

		// Default case that should never happen, but it's good to be safe
	default:
		return Creature(name, 30, 12, 1, 1, 13, 11, 11, 1, "Adventurer");
		break;
	}
}

void dialogue_menu(Creature& player)
{
	// Output the menu
	int result = Dialogue(
		"Menu\n====",
		{ "Items", "Equipment", "Character" }).activate();

	switch (result)
	{
		// Print the items that the player owns
	case 1:
		std::cout << "Items\n=====\n";
		player.inventory.print();
		std::cout << "----------------\n";
		break;
		// Print the equipment that the player is wearing (if they are
		// wearing anything) and then ask if they want to equip a weapon
		// or some armour
	case 2:
	{
		std::cout << "Equipment\n=========\n";
		std::cout << "Head: "
			<< (player.equippedArmour[Armour::Slot::HEAD] != nullptr ?
			player.equippedArmour[Armour::Slot::HEAD]->name : "Nothing")
			<< std::endl;
		std::cout << "Torso: "
			<< (player.equippedArmour[Armour::Slot::TORSO] != nullptr ?
			player.equippedArmour[Armour::Slot::TORSO]->name : "Nothing")
			<< std::endl;
		std::cout << "Legs: "
			<< (player.equippedArmour[Armour::Slot::LEGS] != nullptr ?
			player.equippedArmour[Armour::Slot::LEGS]->name : "Nothing")
			<< std::endl;
		std::cout << "Weapon: "
			<< (player.equippedWeapon != nullptr ?
			player.equippedWeapon->name : "Nothing")
			<< std::endl;

		int result2 = Dialogue(
			"",
			{ "Equip Armour", "Equip Weapon", "Close" }).activate();

		// Equipping armour
		if (result2 == 1)
		{
			int userInput = 0;

			// Cannot equip armour if they do not have any
			// Print a list of the armour and retrieve the amount
			// of armour in one go
			int numItems = player.inventory.print_armour(true);
			if (numItems == 0) break;

			while (!userInput)
			{
				// Choose a piece of armour to equip
				std::cout << "Equip which item?" << std::endl;
				std::cin >> userInput;
				// Equipment is numbered but is stored in a list,
				// so the number must be converted into a list element
				if (userInput >= 1 && userInput <= numItems)
				{
					int i = 1;

					for (auto it : player.inventory.armour)
					{
						if (i++ == userInput)
						{
							// Equip the armour if it is found
							player.equipArmour(it.first);
							break;
						}
					}
				}
			}
		}
		// Equip a weapon, using the same algorithms as for armour
		else if (result2 == 2)
		{
			int userInput = 0;
			int numItems = player.inventory.print_weapons(true);

			if (numItems == 0) break;

			while (!userInput)
			{
				std::cout << "Equip which item?" << std::endl;
				std::cin >> userInput;
				if (userInput >= 1 && userInput <= numItems)
				{
					int i = 1;

					for (auto it : player.inventory.weapons)
					{
						if (i++ == userInput)
						{
							player.equipWeapon(it.first);
							break;
						}
					}
				}
			}
		}
		std::cout << "----------------\n";
		break;
	}
	// Output the character information, including name, class (if
	// they have one), stats, level, and experience
	case 3:
		std::cout << "Character\n=========\n";
		std::cout << player.name;
		if (player.className != "") std::cout << " the " << player.className;
		std::cout << std::endl;

		std::cout << "HP: " << player.health << " / " << player.maxHealth << std::endl;
		std::cout << "Armour Class: " << player.armourClass << std::endl;
		if (player.equippedWeapon != nullptr)
		std::cout << "Weapon: " << player.equippedWeapon->name
			<< " (" << player.equippedWeapon->damage[0] 
			<< "d" << player.equippedWeapon->damage[1] << ")" << std::endl;
		std::cout << "Attack Mod: " << player.attackMod << std::endl;
		std::cout << "Damage Mod: " << player.DamageMod << std::endl;
		std::cout << "Str: " << player.str << std::endl;
		std::cout << "End: " << player.end << std::endl;
		std::cout << "Dex: " << player.dex << std::endl;
		std::cout << "Lvl: " << player.level << " (" << player.exp;
		std::cout << " / " << player.expToLevel(player.level + 1) << ")" << std::endl;
		std::cout << "----------------\n";
		break;
	default:
		break;
	}

	return;
}
