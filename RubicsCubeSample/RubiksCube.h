#pragma once

#include "sge.h"

class RubiksCube : public Model
{
public:
	enum class Shift
	{
		CW,
		CCW
	};

	RubiksCube();
	void init(size_t size = 3);

	void update(float deltaTime) override;

	void rotateFront(Shift dir);
	void rotateRight(Shift dir);
	void rotateTop(Shift dir);
	void rotateBack(Shift dir);
	void rotateLeft(Shift dir);
	void rotateBottom(Shift dir);

	//Testing purposes
	std::vector<Model*> getCubes()
	{
		return m_cubes;
	}

	
protected:
	void draw(IRenderer& renderer, Shader* shader = nullptr) override;
private:
	static Model* createRubiksCubeBox();
	Model* getCube(int i, int j, int k) const;

private:
	Object3D* m_faceFront  = nullptr;
	Object3D* m_faceRight  = nullptr;
	Object3D* m_faceTop    = nullptr;
	Object3D* m_faceBack   = nullptr;
	Object3D* m_faceLeft   = nullptr;
	Object3D* m_faceBottom = nullptr;

	std::vector<Model*> m_cubes;
	size_t m_size = 0;

	std::vector<std::function<bool(float)>> m_asyncCallbacks;
};

