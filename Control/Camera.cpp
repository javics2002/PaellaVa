#include "Camera.h"
#include "../sdlutils/SDLUtils.h"
#include <iostream>

Camera* Camera::mainCamera = nullptr;

Camera::Camera(Vector2D<float> initial, int cam_w, int cam_h) {
	pos = initial;

	width = cam_w;
	height = cam_h;

	winWidth = sdlutils().width();
	winHeight = sdlutils().height();

	/*xmin = ymin = xmax = ymax = 0;

	ogScale = scale = float(winWidth) / width;*/
}

void Camera::setScale(float value) {
	int auxWidth = width;
	int auxHeight = height;
	float auxScale = scale;

	scale = value;
	width = winWidth / scale;
	height = winHeight / scale;

	xmin = ogxmin * scale;
	xmax = ogxmax * scale;
	ymin = ogymin * scale;
	ymax = ogymax * scale;

	pos = pos + Vector2D<float>(auxWidth / 2 - width / 2, auxHeight / 2 - height / 2);
}

float Camera::getScale() {
	return scale;
}

void Camera::restoreScale() {
	setScale(ogScale);
}

void Camera::setMain(Camera* cam) {
	if (mainCamera != nullptr)
		delete mainCamera;
	mainCamera = cam;
}

void Camera::Move(const Vector2D<float>& newPos) {
	pos = newPos;
}

//void Camera::MoveToPoint(const Vector2D<double>& newPos) {
//	Vector2D newcamerapos = newPos - Vector2D(winWidth / 2, winHeight / 2) + Vector2D(winWidth - width, winHeight - height) / 2;
//	pos = newcamerapos;
//}
//
void Camera::Lerp(const Vector2D<float>& newPos, float i) {
	Vector2D<float> newcamerapos = newPos - Vector2D<float>(winWidth / 2, winHeight / 2) + Vector2D<float>(winWidth - width, winHeight - height) / 2;
	if ((pos - newcamerapos).magnitude() > 2) {
		//pos = Vector2D<int>::Lerp(pos, newcamerapos, i * consts::DELTA_TIME);

		pos = pos + (newPos - pos) * i;
	}
}
//void Camera::LerpWithBounds(const Vector2D& newPos, float i) {
//	Lerp(newPos, i);
//
//	Vector2D& p = pos;
//	if (p.getX() < xmin) p.setX(xmin);
//	else if (p.getX() > xmax - winWidth) p.setX(xmax - winWidth);
//
//	if (p.getY() < ymin) p.setY(ymin);
//	else if (p.getY() + height > ymax) p.setY(ymax - height);
//
//}

void Camera::setBounds(float a, float b, float c, float d) {
	ogxmin = a;
	ogymin = b;
	ogxmax = c;
	ogymax = d;

	xmin = ogxmin * scale;
	xmax = ogxmax * scale;
	ymin = ogymin * scale;
	ymax = ogymax * scale;
}

void Camera::MoveDir(Vector2D<float> dir) {
	pos = pos + dir;
}


SDL_Rect* Camera::renderRect() {
	return new SDL_Rect{ (int) pos.getX(), (int) pos.getY(), width, height };
}


Vector2D<float> Camera::getCameraPosition()
{
	return pos;
}

Vector2D<float> Camera::getCameraCenterPosition()
{
	return pos + Vector2D<float>(winWidth / 2, winHeight / 2) - Vector2D<float>(winWidth - width, winHeight - height) / 2;
}