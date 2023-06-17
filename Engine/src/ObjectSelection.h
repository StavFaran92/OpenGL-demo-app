#pragma once

#include <unordered_set>

class Scene;
class Context;

class ObjectSelection
{
public:
	ObjectSelection(Context* context, Scene* scene);
	~ObjectSelection();

	void enableObjectSelection(bool enable);
	bool isEnabled() const;

	//void selectObject(int x, int y);
	bool selectObject(uint32_t id);
	void deselectedObject(uint32_t id);
	void clear();

	bool isObjectSelected(uint32_t id) const;
private:
	
private:
	std::unordered_set<uint32_t> m_selectedObjects;
	bool m_isObjectSelectionEnabled;
};

