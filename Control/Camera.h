#pragma once
#include "../Utils/Vector2D.h"
#include <SDL.h>
//#include "../utils/checkML.h"

class Camera
{
private:
	Vector2D<float> pos;
	int width, height;
	int winWidth, winHeight;

	float scale, ogScale;

public:
	static Camera* mainCamera;

	Camera(Vector2D<float> initialPos, int cam_w, int cam_h);
	static void setMain(Camera* cam);

	void Move(const Vector2D<float>& v);
	void MoveDir(Vector2D<float>& dir);

	void Lerp(const Vector2D<float>& v, float i);

	SDL_Rect* renderRect();


	void setScale(float value);
	float getScale();
	void restoreScale();

	Vector2D<float> getCameraPosition();
	Vector2D<float> getCameraCenterPosition();
};

