#include "TextManager.h"
#include "Game.h"
#include "../sdlutils/VirtualTimer.h"
#include "../GameObjects/UI/UIManager.h"
#include "../GameObjects/UI/ShowText.h"
#include "../sdlutils/InputHandler.h"

TextManager::TextManager(Game* game_, string font) : mAnchoTexto(1000),mAnchoLetra(14), mAlturaLetra(28),
mOffsetYLinea(2), mTiempoCreaccionLetra(1), mUltimoCaracter(""), mTerminado(true), mNumeroLinea(0)
{
	mGame = game_;
	mFuenteLetra = font;
	mVirtualTimer = new VirtualTimer();
}


void TextManager::update()
{
	if (mRapido && !mPausa) { // Si hemos pulsado el avanzar, recorremos todo el dialogo rapidamente
		while (!mTerminado)
			anadeLetra();
	}
	else if (!mTerminado && mVirtualTimer->currTime() > mTiempoCreaccionLetra && !mPausa) { // Si no estamos pausados y no ha terminado de escribir, escribe letra a letra con cierto tiempo

		anadeLetra();
		mVirtualTimer->reset();
	}
}

void TextManager::render() // Render del textManager
{	
	for (int i = 0; i < mLineasDeDialogo.size(); i++) { // Renderizamos lo que llevemos de dialogo
		Texture text(sdlutils().renderer(), mLineasDeDialogo[i], sdlutils().fonts().at(mFuenteLetra), build_sdlcolor(0x444444ff));
		SDL_Rect dest = build_sdlrect(270,sdlutils().height()-135 + ((mAlturaLetra + mOffsetYLinea) * i), text.width(), text.height());
		text.render(dest);
	}
	if (ih().isMandoActive() && !mDesactivado) { // Si hay un mando, renderizamos el boton para continuar
		Texture text1(sdlutils().renderer(), "Pulsa", sdlutils().fonts().at(mFuenteLetra), build_sdlcolor(0x444444ff));
		SDL_Rect d = build_sdlrect(925, sdlutils().height() - 45, text1.width(), text1.height());
		text1.render(d);
	}
	else if (!mDesactivado) { // Si estamos con teclado y raton, la tecla para continuar es la 'Q'
		Texture text1(sdlutils().renderer(), "Pulsa Q", sdlutils().fonts().at(mFuenteLetra), build_sdlcolor(0x444444ff));
		SDL_Rect d = build_sdlrect(925, sdlutils().height() - 45, text1.width(), text1.height());
		text1.render(d);
	}
}

void TextManager::activaTexto(string dialogo) // Activamos el texto
{
	mTerminado = false; // Empieza a escribir por lo q no ha terminado
	mDesactivado = false; // Esta activo
	mUltimoParrafo = false;
	mDialogoTerminado = false;
	mDialogo = dialogo; // Referencia al dialogo que vamos a escribir
}

void TextManager::desactivaTexto()
{
	mDesactivado = true;
	mTerminado = true;
	mUltimoParrafo = true;
	mLineasDeDialogo.clear(); 
	mNumeroLinea = 0; 
}


void TextManager::anadeLetra()
{
	//dialogo ha terminado
	if (mDialogo == "") {
		mTerminado = true;
		mDialogoTerminado = true;
		mUltimoParrafo = true;
		return;
	}

	if (mNumeroLinea >= mLineasDeDialogo.size())
		mLineasDeDialogo.push_back(" ");

	//solo cabe una letra mas, hay más de una letra en el dialog
	if (mLineasDeDialogo[mNumeroLinea].size() * mAnchoLetra >= mAnchoTexto - mAnchoLetra && mDialogo.size() > 1) {//line width

		if (mDialogo[0] == ' ') {
			mLineasDeDialogo[mNumeroLinea] += mDialogo[0];
			mDialogo.erase(0, 1);
		}
		else if (mUltimoCaracter != " ")
			mLineasDeDialogo[mNumeroLinea] += "-";


		mNumeroLinea++;

		if (mNumeroLinea * (mAlturaLetra + mOffsetYLinea) >= 64)
			mTerminado = true;

		mUltimoCaracter = "";
	}
	else {
		mLineasDeDialogo[mNumeroLinea] += mDialogo[0];
		mDialogo.erase(0, 1);
		mUltimoCaracter = mLineasDeDialogo[mNumeroLinea].back();
	}
}

void TextManager::cambiaVelocidad(bool r)
{
	mRapido = r;
;}

