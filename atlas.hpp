#ifndef ATLAS_HPP
#define ATLAS_HPP

#include <vector>

#include "creature.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "armour.hpp"
#include "area.hpp"

// Atlas building functions. Atlases contain vectors of game data that
// is not modified in gameplay, so the base versions of creatures, items
// etc. Could easily be replaced with functions that load the information
// from config files instead of just defining the values in code
void buildatlas_creature(std::vector<Creature>& atlas);
void buildatlas_item(std::vector<Item>& atlas);
void buildatlas_weapon(std::vector<Weapon>& atlas);
void buildatlas_armour(std::vector<Armour>& atlas);
void buildatlas_area(std::vector<Area>& atlas,
	std::vector<Item>& items, std::vector<Weapon>& weapons,
	std::vector<Armour>& armour, std::vector<Creature>& creatures);

#endif /* ATLAS_HPP */
