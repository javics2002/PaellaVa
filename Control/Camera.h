#pragma once
#include "../Utils/Vector2D.h"
#include <SDL.h>
//#include "../utils/checkML.h"

class Camera
{
private:
	Vector2D<int> pos;
	int width, height;
	int winWidth, winHeight;

	float scale, ogScale;
	float xmin, ymin, xmax, ymax;
	float ogxmin, ogymin, ogxmax, ogymax;

public:
	static Camera* mainCamera;

	Camera(Vector2D<int> initialPos, int cam_w, int cam_h);
	static void setMain(Camera* cam);

	void Move(const Vector2D<int>& v);
	void MoveToPoint(const Vector2D<int>& v);

	void Lerp(const Vector2D<int>& v, float i);
	void LerpWithBounds(const Vector2D<int>& v, float i);
	void setBounds(float xmin, float ymin, float xmax, float ymax);

	SDL_Rect* renderRect();

	void MoveDir(Vector2D<int> dir);

	void setScale(float value);
	float getScale();
	void restoreScale();

	Vector2D<int> getCameraPosition();
	Vector2D<int> getCameraCenterPosition();
};

