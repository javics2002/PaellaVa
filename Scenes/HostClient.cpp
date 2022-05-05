#include "HostClient.h"
#include "../Scenes/Lobby.h"
#include "../Control/NetworkManager.h"
#include "../Scenes/Jornada.h"
#include "../GameObjects/UI/ShowText.h"
#include "../Scenes/Menu.h"

HostClient::HostClient(Game* mGame) : Scene(mGame)
{
	mBackground->setTexture("hostClientBg");
	mBackground->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	mBackground->setDimension(sdlutils().width(), sdlutils().height() + 100);

	int mOffsetX = 300, mOffsetY = 150;

	mParticles = new ParticleExample();
	mParticles->setRenderer(sdlutils().renderer());
	mParticles->setStyle(ParticleExample::NONE);

	UiButton* mBackButton = new UiButton(mGame, "back", 70, 50, 125, 60);
	mBackButton->setInitialDimension(mBackButton->getWidth(), mBackButton->getHeight());
	mBackButton->setAction([](Game* mGame, bool& exit) {
		mGame->sendMessageScene(new Menu(mGame));
		});

	mUiManager->addButton(mBackButton);

	mHostButton = new UiButton(mGame, "cocinera",
		sdlutils().width() / 2 - mOffsetX, sdlutils().height() - 350, 500, 700);
	mHostButton->setInitialDimension(500, 700);
	mHostButton->setAction([](Game* mGame, bool& exit) {
		//Host
		mGame->getNetworkManager()->init('h', nullptr, mGame->getNombre());

		mGame->sendMessageScene(new Lobby(mGame));
		});
	mUiManager->addButton(mHostButton);

	mClientButton = new UiButton(mGame, "camarero",
		sdlutils().width() / 2 + mOffsetX, sdlutils().height() - 350, 500, 700);
	mClientButton->setInitialDimension(500, 700);

	mPosYNotOver = 165;
	mPosYOver = 145;
	mTextIp = new ShowText(mGame, " ", "ip", mClientButton->getX() + 20, mPosYNotOver);
	mTextIp->setActive(true);

	mIpNoValida = new ShowText(mGame, "IP NO VALIDA", "ipCursor", mClientButton->getX() + 20, 225);
	mIpNoValida->setActive(false);

	mCursor = new ShowText(mGame, "|", "ipCursor", mClientButton->getX() + 20, mPosYNotOver);
	mCursor->setActive(false);

	mClientButton->setAction([this](Game* mGame, bool& exit) {
		//Client
		if (!mEscribiendo) {
			mEscribiendo = true;
			ih().clearInputBuffer();
			mCursor->setActive(true);
			mTime = sdlutils().currRealTime();
		}

		else {
			
			mIp.erase(remove(mIp.begin(), mIp.end(), ' '), mIp.end());
			if (esValida(mIp)) {
				// game->sendMessageScene(new IntroduceIP(game));
				if (mGame->getNetworkManager()->init('c', mIp.c_str(), mGame->getNombre())) {
					// crear el lobby
					mGame->sendMessageScene(new Lobby(mGame, mGame->getNetworkManager()->getOtherName()));
				}
			}
			else {
				mIpNoValida->setActive(true);
				mTiempoIpNV = sdlutils().currRealTime();
			}
		}
		});

	mUiManager->addButton(mClientButton);	
}

bool HostClient::esValida(string ipText)
{
	if (ipText == " ") return false;

	vector<string> aux = split(ipText);

	if (aux.size() < 4) return false;

	for (int i = 0; i < aux.size(); i++) {

		string digito = aux[i];

		if (digito.size() == 0) return false;

		for (int j = 0; j < digito.size(); j++) {
			
			if (!std::isdigit(digito[j])) {
				return false;
			}
		}

		if (stoi(digito) >= 256) {
			return false;
		}
	}

	return true;
}

void HostClient::update()
{
	Scene::update();
	mParticles->update();

	if (mEscribiendo) {

		char c = ih().getTypedKey();

		if (c != ' ' && c != '\r')
		{
			if (!mEscrito) mEscrito = true;

			if (c == '\b')
			{
				if (!mIp.empty())
					mIp.pop_back();
				else
					mIp = ' ';
			}

			else if (mIp.size() < mMaxCaracteres) {
				mIp += c;
			}

			if (mIp.empty())
				mIp = ' ';

			mTextIp->setTexture(mIp, string("ip"), {255,255,255,0}, { 0, 0, 0, 0 });
			mTextIp->setDimension();

		}
		mTextIp->render(nullptr);
	}
	//Preguntar isHover y 
	if (mClientButton->hover()) {
		mTextIp->setPosition(mTextIp->getX(), mPosYOver);
		mCursor->setPosition(mCursor->getX(), mPosYOver);
		mParticles->setStyle(ParticleExample::LIGHT);
		mParticles->setPosition(mClientButton->getX(), sdlutils().height());
	}

	else {
		mTextIp->setPosition(mTextIp->getX(), mPosYNotOver);
		mCursor->setPosition(mCursor->getX(), mPosYNotOver);	
	}

	if (mHostButton->hover()) {
		mParticles->setStyle(ParticleExample::LIGHT);
		mParticles->setPosition(mHostButton->getX(), sdlutils().height());
	}

	if (sdlutils().currRealTime() - mTiempoIpNV > mFrameRateIpNV) {
		mIpNoValida->setActive(false);
	}

}

vector<string> HostClient::split(string ipText)
{
	vector<string> mIpText;
	char mDelimitador = '.';

	string mDigitos = "";

	for (int i = 0; i < ipText.size(); i++) {
		if (ipText[i] != mDelimitador) {
			mDigitos += ipText[i];
			if(i + 1 == ipText.size()) mIpText.push_back(mDigitos);
		}

		else {
			mIpText.push_back(mDigitos);
			mDigitos = "";
		}
	}

	return mIpText;
}

void HostClient::render() {

	mBackground->render(mCamera->renderRect());
	mObjectManager->render(mCamera->renderRect());
	mParticles->draw(mCamera->renderRect());
	mUiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
	mTextIp->render(nullptr);
	mIpNoValida->render(nullptr);

	if (!mEscrito && sdlutils().currRealTime() - mTime > mFrameRate) {
		mCursor->render(nullptr);
		mTime = sdlutils().currRealTime();
	}

	mTextMngr->render();
}



