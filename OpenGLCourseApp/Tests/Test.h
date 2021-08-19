#pragma once
namespace test {
	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};

		virtual void OnRender() {};
		virtual void OnGuiRender() {};
		virtual void OnUpdate(float deltaTime) {};
	};

}