#pragma once
#include <memory>
#include <unordered_set>

#include "IProjector.h"


class FrameBufferObject;
class RenderBufferObject;
class TextureHandler;


class ObjectSelection : public IProjector
{
public:
	struct PixelInfo {
		unsigned int ObjectID = 0;
		unsigned int DrawID = 0;
		unsigned int PrimID = 0;
	};

	ObjectSelection();
	~ObjectSelection();

	void selectObject(int x, int y);
	void deselectedObject(uint32_t id);

	void clearSelectedObjects();

	bool isObjectSelected(uint32_t id) const;

	// Inherited via IProjector
	virtual bool init() override;
	virtual void enableWriting() override;
	virtual void disableWriting() override;
private:
	bool selectObject(uint32_t id);
private:
	std::unordered_set<uint32_t> m_selectedObjects;

	std::shared_ptr<FrameBufferObject> m_frameBuffer = nullptr;
	std::shared_ptr<RenderBufferObject> m_renderBuffer = nullptr;
	std::shared_ptr<TextureHandler> m_textureHandler = nullptr;

};

