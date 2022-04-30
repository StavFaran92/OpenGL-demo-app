#pragma once

#include <unordered_set>

class ObjectSelection
{
public:
	ObjectSelection();
	~ObjectSelection();

	//void selectObject(int x, int y);
	bool selectObject(uint32_t id);
	void deselectedObject(uint32_t id);
	void clearSelectedObjects();

	bool isObjectSelected(uint32_t id) const;
private:
	
private:
	std::unordered_set<uint32_t> m_selectedObjects;
};

