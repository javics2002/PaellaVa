#pragma once
#pragma once

#include "../../sdlutils/Font.h"
#include "../Utils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../GameObjects/Imagen.h"
#include <vector>

using namespace std;

class VirtualTimer;
class Game;

class TextManager
{;

public:

	TextManager(Game* game_, string font);
	~TextManager() {}

	void update();
	void render();

	void activaTexto(string dialogo);
	void desactivaTexto();

	bool desactivado() { return mDesactivado; }

	bool esUltimoParrafo() { return mUltimoParrafo; }
	void reiniciaParrafo() { mTerminado = false; mLineasDeDialogo.clear(); mNumeroLinea = 0; mRapido = false; };
	bool TerminadoEscribir() { return mDialogoTerminado; }
	bool terminadoParrado() { return mTerminado; }
	void anadeLetra();
	void cambiaVelocidad(bool r);
	bool vaRapido() { return mRapido; };
	void cambiaPausa() { mPausa = !mPausa; };

private:

	VirtualTimer* mVirtualTimer;
	Game* mGame;
	string mFuenteLetra, mDialogo, mUltimoCaracter;
	vector<string> mLineasDeDialogo;



	bool mTerminado=true, mDialogoTerminado=true,mUltimoParrafo,mDesactivado=true, mRapido=false,mPausa = false;
	int mOffsetYLinea,mTiempoCreaccionLetra, mNumeroLinea,mAnchoLetra, mAlturaLetra, mAnchoTexto;;

};

