#include "TextManager.h"
#include "Game.h"
#include "../sdlutils/VirtualTimer.h"
#include "../GameObjects/UI/UIManager.h"
#include "../GameObjects/UI/ShowText.h"
#include "../sdlutils/InputHandler.h"

TextManager::TextManager(Game* game_, string font) : anchoTexto(1000),anchoLetra(14), alturaLetra(28),
offsetYLinea(2), tiempoCreaccionLetra(1), ultimoCaracter(""), terminado(true), numeroLinea(0)
{
	mGame = game_;
	fuenteLetra = font;
	vt = new VirtualTimer();
}


void TextManager::update()
{
	if (rapido && !pausa) { // Si hemos pulsado el avanzar, recorremos todo el dialogo rapidamente
		while (!terminado)
			anadeLetra();
	}
	else if (!terminado && vt->currTime() > tiempoCreaccionLetra && !pausa) { // Si no estamos pausados y no ha terminado de escribir, escribe letra a letra con cierto tiempo

		anadeLetra();
		vt->reset();
	}
}

void TextManager::render() // Render del textManager
{	
	for (int i = 0; i < lineas.size(); i++) { // Renderizamos lo que llevemos de dialogo
		Texture text(sdlutils().renderer(), lineas[i], sdlutils().fonts().at(fuenteLetra), build_sdlcolor(0x444444ff));
		SDL_Rect dest = build_sdlrect(270,sdlutils().height()-135 + ((alturaLetra + offsetYLinea) * i), text.width(), text.height());
		text.render(dest);
	}
	if (ih().isMandoActive() && !desactivado_) { // Si hay un mando, renderizamos el boton para continuar
		Texture text1(sdlutils().renderer(), "Pulsa", sdlutils().fonts().at(fuenteLetra), build_sdlcolor(0x444444ff));
		SDL_Rect d = build_sdlrect(925, sdlutils().height() - 45, text1.width(), text1.height());
		text1.render(d);
	}
	else if (!desactivado_) { // Si estamos con teclado y raton, la tecla para continuar es la 'Q'
		Texture text1(sdlutils().renderer(), "Pulsa Q", sdlutils().fonts().at(fuenteLetra), build_sdlcolor(0x444444ff));
		SDL_Rect d = build_sdlrect(925, sdlutils().height() - 45, text1.width(), text1.height());
		text1.render(d);
	}
}

void TextManager::activaTexto(string dialogo) // Activamos el texto
{
	terminado = false; // Empieza a escribir por lo q no ha terminado
	desactivado_ = false; // Esta activo
	ultimoParrafo = false;
	dialogoTerminado = false;
	dialogo_ = dialogo; // Referencia al dialogo que vamos a escribir
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

