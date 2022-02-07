#include "Restaurante.h"
#include "../GameObjects/GrupoClientes.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Paella.h"
#include "../GameObjects/Ingredientes/Ingrediente.h"
#include "../GameObjects/Muebles/Mueble.h"
#include <iostream>
using namespace std;

Restaurante::Restaurante()
{
	if (map.load("../Assets/Tilemap/Restaurante.tmx"))
	{
		std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;
		if (map.isInfinite())
		{
			std::cout << "Map is infinite.\n";
		}

		const auto& mapProperties = map.getProperties();
		std::cout << "Map has " << mapProperties.size() << " properties" << std::endl;
		for (const auto& prop : mapProperties)
		{
			std::cout << "Found property: " << prop.getName() << std::endl;
			std::cout << "Type: " << int(prop.getType()) << std::endl;
		}

		std::cout << std::endl;

		const auto& layers = map.getLayers();
		std::cout << "Map has " << layers.size() << " layers" << std::endl;
		for (const auto& layer : layers)
		{
			std::cout << "Found Layer: " << layer->getName() << std::endl;
			std::cout << "Layer Type: " << int(layer->getType()) << std::endl;

			if (layer->getType() == tmx::Layer::Type::Group)
			{
				std::cout << "Checking sublayers" << std::endl;
				const auto& sublayers = layer->getLayerAs<tmx::LayerGroup>().getLayers();
				std::cout << "LayerGroup has " << sublayers.size() << " layers" << std::endl;
				for (const auto& sublayer : sublayers)
				{
					std::cout << "Found Layer: " << sublayer->getName() << std::endl;
					std::cout << "Layer Type: " << int(sublayer->getType()) << std::endl;

					if (sublayer->getType() == tmx::Layer::Type::Object)
					{
						std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<tmx::ObjectGroup>().getObjects().size() << " objects" << std::endl;
					}
					else if (sublayer->getType() == tmx::Layer::Type::Tile)
					{
						std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<tmx::TileLayer>().getTiles().size() << " tiles" << std::endl;
					}
				}
			}

			if (layer->getType() == tmx::Layer::Type::Object)
			{
				const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
				std::cout << "Found " << objects.size() << " objects in layer" << std::endl;
				for (const auto& object : objects)
				{
					std::cout << "Object " << object.getUID() << ", " << object.getName() << std::endl;
					const auto& properties = object.getProperties();
					std::cout << "Object has " << properties.size() << " properties" << std::endl;
					for (const auto& prop : properties)
					{
						std::cout << "Found property: " << prop.getName() << std::endl;
						std::cout << "Type: " << int(prop.getType()) << std::endl;
					}

					if (!object.getTilesetName().empty())
					{
						std::cout << "Object uses template tile set " << object.getTilesetName() << "\n";
					}
				}
			}

			if (layer->getType() == tmx::Layer::Type::Tile)
			{
				const auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
				if (tiles.empty())
				{
					const auto& chunks = layer->getLayerAs<tmx::TileLayer>().getChunks();
					if (chunks.empty())
					{
						std::cout << "Layer has missing tile data\n";
					}
					else
					{
						std::cout << "Layer has " << chunks.size() << " tile chunks.\n";
					}
				}
				else
				{
					std::cout << "Layer has " << tiles.size() << " tiles.\n";
				}
			}

			const auto& properties = layer->getProperties();
			std::cout << properties.size() << " Layer Properties:" << std::endl;
			for (const auto& prop : properties)
			{
				std::cout << "Found property: " << prop.getName() << std::endl;
				std::cout << "Type: " << int(prop.getType()) << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Failed loading map" << std::endl;
	}
}

Restaurante::~Restaurante()
{
}

void Restaurante::Render(SDL_Renderer* renderer, SDL_Texture* texture, TMXLoader* loader)
{
	SDL_SetRenderDrawColor(renderer, 10, 255, 255, SDL_ALPHA_TRANSPARENT);
	SDL_RenderClear(renderer);

	char tileID = 0;

	int tileWidth = loader->getMap("testmap")->getTileWidth();
	int tileHeight = loader->getMap("testmap")->getTileHeight();

	for (int i = 0; i < loader->getMap("testmap")->getWidth(); ++i)
	{
		for (int j = 0; j < loader->getMap("testmap")->getHeight(); ++j)
		{
			// get the tile at current position
			tileID = loader->getMap("testmap")->getTileLayer("Tile Layer 1")->getTileVector()[i][j];

			// only render if it is an actual tile (tileID = 0 means no tile / don't render anything here)
			if (tileID > 0)
			{
				SDL_Rect srcrect = { ((tileID - 1) % 3) * tileWidth, ((tileID - 1) / 3) * tileHeight, tileWidth, tileHeight };
				SDL_Rect dstrect = { j * 25, i * 25, 25, 25 };
				SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
			}
		}
	}

	SDL_RenderPresent(renderer);
}
