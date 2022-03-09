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
	float xmin, ymin, xmax, ymax;
	float ogxmin, ogymin, ogxmax, ogymax;

	const float EPSILON = 0.01;

public:
	static Camera* mainCamera;

	Camera(Vector2D<float> initialPos, int cam_w, int cam_h);
	static void setMain(Camera* cam);

	void Move(const Vector2D<float>& v);
	void MoveToPoint(const Vector2D<float>& v);

	bool Lerp(const Vector2D<float>& v, float i);
	void LerpWithBounds(const Vector2D<float>& v, float i);
	void setBounds(float xmin, float ymin, float xmax, float ymax);

	SDL_Rect* renderRect();

	void MoveDir(Vector2D<float> dir);

	void setScale(float value);
	float getScale();
	void restoreScale();

	Vector2D<float> getCameraPosition();
	Vector2D<float> getCameraCenterPosition();
};

