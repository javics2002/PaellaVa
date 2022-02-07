#include <iostream>
#include "Game.h"
#include "ViewController.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        Game* game = new Game(1280, 720);

        ViewController controller(game);
        controller.run();
        delete game;
    }
    catch (const string& e) {
        cout << e;
    }

    return 0;
}


