#include "TextManager.h"
#include "Game.h"
#include "../sdlutils/VirtualTimer.h"
#include "../GameObjects/UI/ShowText.h"

TextManager::TextManager(Game* game_, string font) : anchoTexto(1000),anchoLetra(14), alturaLetra(28),
offsetYLinea(2), tiempoCreaccionLetra(1), ultimoCaracter(""), terminado(true), numeroLinea(0)
{
	game = game_;
	fuenteLetra = font;
	vt = new VirtualTimer();
}


void TextManager::update()
{
	if (rapido) {
		while (!terminado)
			anadeLetra();
	}
	else if (!terminado && vt->currTime() > tiempoCreaccionLetra) {

		anadeLetra();
		vt->reset();
	}
}

void TextManager::render()
{	
	for (int i = 0; i < lineas.size(); i++) {
		Texture text(sdlutils().renderer(), lineas[i], sdlutils().fonts().at(fuenteLetra), build_sdlcolor(0x444444ff));
		SDL_Rect dest = build_sdlrect(270,sdlutils().height()-135 + ((alturaLetra + offsetYLinea) * i), text.width(), text.height());
		text.render(dest);
	}
}

void TextManager::activaTexto(string dialogo)
{
	terminado = false;
	desactivado_ = false;
	ultimoParrafo = false;
	dialogoTerminado = false;
	dialogo_ = dialogo;
}

void TextManager::desactivaTexto()
{
	desactivado_ = true;
	terminado = true;
	ultimoParrafo = true;
	lineas.clear(); 
	numeroLinea = 0; 
}


void TextManager::anadeLetra()
{
	//dialogo ha terminado
	if (dialogo_ == "") {
		terminado = true;
		dialogoTerminado = true;
		ultimoParrafo = true;
		return;
	}

	if (numeroLinea >= lineas.size())
		lineas.push_back(" ");

	//solo cabe una letra mas, hay más de una letra en el dialog
	if (lineas[numeroLinea].size() * anchoLetra >= anchoTexto - anchoLetra && dialogo_.size() > 1) {//line width

		if (dialogo_[0] == ' ') {
			lineas[numeroLinea] += dialogo_[0];
			dialogo_.erase(0, 1);
		}
		else if (ultimoCaracter != " ")
			lineas[numeroLinea] += "-";


		numeroLinea++;

		if (numeroLinea * (alturaLetra + offsetYLinea) >= 64)
			terminado = true;

		ultimoCaracter = "";
	}
	else {
		lineas[numeroLinea] += dialogo_[0];
		dialogo_.erase(0, 1);
		ultimoCaracter = lineas[numeroLinea].back();
	}
}

void TextManager::cambiaVelocidad(bool r)
{
	rapido = r;
;}

