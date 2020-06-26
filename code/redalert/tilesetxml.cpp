// TILESETXML.CPP
//

#include "FUNCTION.H"
#include "tinyxml2.h"

#include <string>
#include <vector>

int64_t generateHashValue(const char* fname, const int size);

struct TilesetTileRule_t {
	std::string name;
	int64_t hash;
	int shape;
	std::vector<std::string> frames;
};

struct TilesetXMLInfo_t {
	std::vector< TilesetTileRule_t> tiles;
};

TilesetXMLInfo_t theaterXMLRules[3];

const char* Tileset_FindHDTexture(int theaterType, const char* shapeFileName, int shapeNum, int frameNum) {
	int64_t hash = generateHashValue(shapeFileName, strlen(shapeFileName));
	for (int i = 0; i < theaterXMLRules[theaterType].tiles.size(); i++) {
		if (theaterXMLRules[theaterType].tiles[i].hash == hash && theaterXMLRules[theaterType].tiles[i].shape == shapeNum) {
			return theaterXMLRules[theaterType].tiles[i].frames[frameNum].c_str();
		}
	}
	return NULL;
}

int Tileset_GetNumFramesForTile(int theaterType, const char* shapeFileName, int shapeNum) {
	int64_t hash = generateHashValue(shapeFileName, strlen(shapeFileName));
	for (int i = 0; i < theaterXMLRules[theaterType].tiles.size(); i++) {
		if (theaterXMLRules[theaterType].tiles[i].hash == hash && theaterXMLRules[theaterType].tiles[i].shape == shapeNum) {
			return theaterXMLRules[theaterType].tiles[i].frames.size();
		}
	}
	return 0;
}

void Tileset_ParseTile(tinyxml2::XMLNode* tile, TilesetTileRule_t& tileRule) {
	tinyxml2::XMLNode* KeyNode = tile->FirstChildElement("Key");
	tinyxml2::XMLNode* NameNode = KeyNode->FirstChildElement("Name");
	tinyxml2::XMLNode* ShapeNode = KeyNode->FirstChildElement("Shape");
	tinyxml2::XMLNode* ValueNode = tile->FirstChildElement("Value");
	tinyxml2::XMLNode* FramesNode = ValueNode->FirstChildElement("Frames");
	tinyxml2::XMLNode* FrameNode = FramesNode->FirstChildElement("Frame");

	tileRule.name = NameNode->FirstChild()->ToText()->Value();
	tileRule.hash = generateHashValue(tileRule.name.c_str(), tileRule.name.size());
	tileRule.shape = atoi(ShapeNode->FirstChild()->ToText()->Value());

	while (FrameNode != NULL) {
		tileRule.frames.push_back(FrameNode->FirstChild()->ToText()->Value());
		FrameNode = FrameNode->NextSiblingElement("Frame");
	}
}

void Tileset_LoadRuleXML(const char* path, TilesetXMLInfo_t& info) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path);

	tinyxml2::XMLElement* root = doc.FirstChildElement();
	if (root == NULL)
		return;

	tinyxml2::XMLNode* tilesetTypeClassNode = root->FirstChild();

	tinyxml2::XMLNode* tilesParent = tilesetTypeClassNode->FirstChildElement("Tiles");
	tinyxml2::XMLNode* tile = tilesParent->FirstChildElement("Tile");
	while (tile != NULL) {
		TilesetTileRule_t tileRule;
		Tileset_ParseTile(tile, tileRule);
		info.tiles.push_back(tileRule);
		tile = tile->NextSiblingElement("Tile");
	}
}

void Tileset_LoadRules(void) {
	for (int i = 0; i < 3; i++) {
		char tmp[512];
		sprintf(tmp, "data/xml/tilesets/ra_terrain_%s.xml", Theaters[i].Name);
		Tileset_LoadRuleXML(tmp, theaterXMLRules[i]);
	}
}