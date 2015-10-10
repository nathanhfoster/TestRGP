#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

class Item
{
public:

	// Name and description of the item
	std::string name;
	std::string description;

	// Standard constructors, nothing special
	Item(std::string name, std::string description)
	{
		this->name = name;
		this->description = description;
	}

	Item()
	{
	}
};

#endif /* ITEM_HPP */
