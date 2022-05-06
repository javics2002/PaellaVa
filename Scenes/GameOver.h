#pragma once

#include "../Scenes/Scene.h"

const vector<string> nombreTilemaps = { "Jornada2", "Jornada3", "Jornada4", "Jornada5" };

class GameOver : public Scene
{
public:
	GameOver(Game* mGame, int puntuacion, int numeroJornada);

private:
	int mStarNumber = 1;
	int mNumJornada = 0;
	int mPoints = 0;
	int mMaxStars = 5; //maximo de estrellas
	int mPointStarValue = 2; //num de puntos que vale una estrella
	int mNumStarsAdded = 0;

	Texture* mStarTexture;
		
	void calculateStarNumber();

	void render() override;

	std::vector<Texture*> mReviews;
	std::vector<Texture*> mUsernames;

	void createRandomReviews();
	void createRandomUsernames();

	void renderReviews();
	
	void tweenEstrellas(vector<GameObject*> estrellas, int i = 0);
};

