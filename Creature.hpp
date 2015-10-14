#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "Inventory.hpp"
#include "Weapon.hpp"
#include "Armour.hpp"

#include <string>

class Creature
{
public:

	// Name of the creature and the name of its class, if it has one
	// Class may be Fighter, Rogue etc
	std::string name;
	std::string className;

	// Creature stats. Reasonable values are in parentheses
	int health;		// Current hit points (10-1000+)
	int maxHealth;	// Maximum hit points (10-1000+)
	int armourClass;
	int attackMod;
	int DamageMod;
	int str;		// Strength. Determines damage in battle (1-100)
	int end;		// Endurance. Determines maximum health (1-100)
	int dex;		// Dexterity. Determines speed in battle (1-100)

	// Current level of the creature. Determines the amount of experience
	// that it gives to the victor when defeated (see Battle class for more)
	// and the amount of experience required to level up again. Upon
	// levelling up the creature will gain stat improvements.
	// 1-50 is reasonable
	unsigned int level;

	// Current experience. 0-1M is reasonable, see the levelup() function
	// for a decent scale
	unsigned int exp;

	// Items that the creature possesses
	Inventory inventory;

	// Currently equipped weapon. Used as a pointer to an atlas entry,
	// but not necessary. nullptr denotes that no weapon is equipped
	Weapon* equippedWeapon;

	// Armour currently equipped into each slot
	Armour* equippedArmour[Armour::Slot::N];

	Creature(std::string name, int health, int armourClass, int attackMod, int DamageMod, 
		int str, int end, int dex, unsigned int level = 1, std::string className = "")
	{
		this->name = name;
		this->health = health;
		this->maxHealth = health;
		this->armourClass = armourClass;
		this->attackMod = attackMod;
		this->DamageMod = DamageMod;
		this->str = str;
		this->end = end;
		this->dex = dex;
		this->className = className;
		this->equippedArmour[Armour::Slot::HEAD] = nullptr;
		this->equippedArmour[Armour::Slot::TORSO] = nullptr;
		this->equippedArmour[Armour::Slot::LEGS] = nullptr;
		this->equippedWeapon = nullptr;
		this->level = level;
		this->exp = 0;
	}

	Creature()
	{
		this->equippedArmour[Armour::Slot::HEAD] = nullptr;
		this->equippedArmour[Armour::Slot::TORSO] = nullptr;
		this->equippedArmour[Armour::Slot::LEGS] = nullptr;
		this->equippedWeapon = nullptr;
		this->level = 1;
		this->exp = 0;
	}

	// Equip a weapon by setting the equipped weapon pointer. Currently
	// a pointless function (simple enough to be rewritten each time)
	// but handy if dual wielding is ever added, or shields etc
	void equipWeapon(Weapon* weapon)
	{
		// If weapon is already wielded 
		// unequipped it and remove it's modifiers
		if (this->equippedWeapon != nullptr)
		{
			this->attackMod -= equippedWeapon->attackMod;
			this->equippedWeapon = nullptr;
		}

		this->equippedWeapon = weapon;
		this->attackMod += weapon->attackMod;
		
		return;
	}	

	// Equip the armour into it's correct slot. A slightly more useful
	// function!
	void equipArmour(Armour* armour)
	{
		// If already wearing armor 
		// unequipped it and remove it's modifiers.
		if (this->equippedArmour[(int)armour->slot] != nullptr)
		{
			this->health -= equippedArmour[(int)armour->slot]->healthBoost;
			this->maxHealth -= equippedArmour[(int)armour->slot]->healthBoost;
			this->equippedArmour[(int)armour->slot] = nullptr;
		}

		this->equippedArmour[(int)armour->slot] = armour;
		this->health += armour->healthBoost;
		this->maxHealth += armour->healthBoost;

		return;
	}

	// Calculates the experience required to reach a certain level,
	// *in total*. Really this is class specific and not object specific
	unsigned int expToLevel(unsigned int level)
	{
		// Exp to level x = 128*x^2
		return 128 * level * level;
	}

	// Level the creature to the next level if it has enough experience
	// to do so, returning true if it could level up and false otherwise.
	bool levelUp()
	{
		// We want the experience to the next level, not the current level
		if (this->exp >= expToLevel(this->level + 1))
		{
			// Advance to the next level
			++level;

			// Variables to keep track of stat changes. Neater than
			// having a bunch of stat increases all over the place,
			// and removes the issue of the next level's stats affecting
			// themselves (increasing endurance then increasing health
			// based on the boosted instead of the original value, for
			// example
			unsigned int healthBoost = 0;
			unsigned int armourClassBoost = 0;
			unsigned int attackModBoost = 0;
			unsigned int DamageModBoost = 0;
			unsigned int strBoost = 0;
			unsigned int endBoost = 0;
			unsigned int dexBoost = 0;

			// Give a large boost to health every third level
			if (level % 3 == 0)
			{
				// Randomly increase health, but always give a sizeable
				// chunk proportional to the creature's endurance
				healthBoost = 10 + (rand() % 4) + this->end / 4;
			}
			else
			{
				// Just increase health by a small amount
				healthBoost = this->end / 4;
			}
			// If the creature is a fighter, then favour strength and
			// endurance boosts over dexterity, but increase dexterity
			// 50% of the time too
			if (this->className == "Fighter")
			{
				strBoost = 1;
				endBoost = 1;
				if (rand() % 2 == 0) dexBoost = 1;
			}
			// Same as for fighter but favour dexterity and endurance
			// instead. Rogue's favour endurance too in order to keep
			// them at roughly the same capability
			else if (this->className == "Rogue")
			{
				endBoost = 1;
				dexBoost = 1;
				if (rand() % 2 == 0) strBoost = 1;
			}

			// Adjust all of the variables accordingly
			this->maxHealth += healthBoost;
			this->armourClass += armourClassBoost;
			this->attackMod += attackModBoost;
			this->DamageMod += DamageModBoost;
			this->str += strBoost;
			this->end += endBoost;
			this->dex += dexBoost;

			// Tell the user that they grew a level, what the boosts where
			// and what their stats are now
			std::cout << this->name << " grew to level " << level << "!\n";
			std::cout << "Health +" << healthBoost << " -> " << this->maxHealth << std::endl;
			std::cout << "Str +" << strBoost << " -> " << this->str << std::endl;
			std::cout << "End +" << endBoost << " -> " << this->end << std::endl;
			std::cout << "Dex +" << dexBoost << " -> " << this->dex << std::endl;
			std::cout << "----------------\n";

			return true;
		}
		return false;
	}
};

#endif /* CREATURE_HPP */
