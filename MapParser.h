#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <map>
#include <string>
#include "GameMap.h"
#include "TileLayer.h"
#include "tinyxml.h"
#include <vector>

class MapParser {
	public:
		void Load();
		void Clean();

		inline GameMap* GetMap(std::string id) { return m_MapDict[id]; };
		inline static MapParser* GetInstance() {
			if (s_Instance != nullptr)
				return s_Instance;
			else
				return s_Instance = new MapParser();
		}

	private:
		bool Parse(std::string id, std::string source);
		Tileset ParseTileset(TiXmlElement* xmlTileset);
		TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TilesetList tilesets, int tilesize, int rowcount, int colcount);

	private:
		MapParser() {
			ListMap.push_back("map1.tmx");
			ListMap.push_back("map2.tmx");
			ListMap.push_back("map3.tmx");
			Completemap1 = false;
		};
		static MapParser* s_Instance;
		std::map<std::string, GameMap*> m_MapDict;
		std::vector<std::string> ListMap;
		bool Completemap1;
		std::string loadmap;
};

#endif // !1
