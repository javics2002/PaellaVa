#pragma once
#include "Scene.h"
#include "../Utils/ParticleExample.h"

class HostClient : public Scene
{
	int mMaxCaracteres = 16;
	string mIp = "";
	ParticleExample* mParticles;

	ShowText* mTextIp;
	ShowText* mCursor;
	ShowText* mIpNoValida;
	UiButton* mClientButton;
	UiButton* mHostButton;

	int mPosYOver = 0;
	int mPosYNotOver = 0;

	bool mEscribiendo = false;
	bool mEscrito = false;

	float mFrameRate = 800;
	float mTime;

	float mFrameRateIpNV = 500;
	float mTiempoIpNV;

	bool esValida(string ipText);
	vector<string> split(string ipText);

public:
	HostClient(Game* mGame);

	void update() override;
	void render() override;
};

