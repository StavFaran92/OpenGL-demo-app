#pragma once
class Mouse
{
public:
	inline float GetXChange() {
		return xChange;
	}

	inline float GetYChange() {
		return yChange;
	}

	inline void ResetChange() {
		xChange = 0;
		yChange = 0;
	}

	inline bool GetMouseLeftPressed() {
		return mouseLeft;
	}

	inline bool GetMouseRightPressed() {
		return mouseRight;
	}

	inline bool GetMouseMiddlePressed() {
		return mouseMiddle;
	}

private:
	float lastX = 0;
	float lastY = 0;
	float xChange = 0;
	float yChange = 0;

	bool mouseLeft = false;
	bool mouseRight = false;
	bool mouseMiddle = false;
};
