#include "EntryPoint.h"
#include "sge.h"

class CustomBoxBehaviour : public ScriptableEntity
{
	virtual void onCreate() override
	{
		std::cout << "Box was created modafaka.\n";
	}
};

class Sandbox : public Application
{
public:

	void start() override
	{

		auto box = ShapeFactory::createBox(&getContext()->getActiveScene()->getRegistry());

		std::vector<Transformation> transformations;

		const int gridLength = 100;

		for (int i = 0; i < gridLength; i++)
		{
			for (int j = 0; j < gridLength; j++)
			{
				Transformation trans(box, {(i - gridLength / 2) * 2, 0, (j - gridLength / 2) * 2});
				transformations.push_back(trans);
			}
		}

		auto& meshComponent = box.getComponent<MeshComponent>();

		box.addComponent<InstanceBatch>(transformations, meshComponent.mesh);
	}

};

Application* CreateApplication()
{
	return new Sandbox();
}