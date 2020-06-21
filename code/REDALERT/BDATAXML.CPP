// BDATAXML.CPP
//

#include "FUNCTION.H"
#include "tinyxml2.h"

#include <string>
#include <vector>

int64_t generateHashValue(const char* fname, const int size);

struct BuildingsTileRule_t {
	std::string name;
	int64_t hash;
	int shape;
	std::vector<std::string> frames;
};

struct BuildingsXMLInfo_t {
	std::vector<BuildingsTileRule_t> tiles;
};

BuildingsXMLInfo_t buildingsXmlRules;

const char* Buildings_FindHDTexture(const char* shapeFileName, int shapeNum, int frameNum) {
	char tmpFileName[2048];
	strcpy(tmpFileName, shapeFileName);
	COM_SetExtension(tmpFileName, strlen(tmpFileName), "");
	int64_t hash = generateHashValue(tmpFileName, strlen(tmpFileName));
	for (int i = 0; i < buildingsXmlRules.tiles.size(); i++) {
		if (buildingsXmlRules.tiles[i].hash == hash && buildingsXmlRules.tiles[i].shape == shapeNum) {
			static char hdTexturePath[2048];
			sprintf(hdTexturePath, "DATA/ART/TEXTURES/SRGB/RED_ALERT/STRUCTURES/%s", buildingsXmlRules.tiles[i].frames[frameNum].c_str());
			return &hdTexturePath[0];
		}
	}
	return NULL;
}

int Buildings_GetNumFramesForTile(const char* shapeFileName, int shapeNum) {
	int64_t hash = generateHashValue(shapeFileName, strlen(shapeFileName));
	for (int i = 0; i < buildingsXmlRules.tiles.size(); i++) {
		if (buildingsXmlRules.tiles[i].hash == hash && buildingsXmlRules.tiles[i].shape == shapeNum) {
			return buildingsXmlRules.tiles[i].frames.size();
		}
	}
	return 0;
}

void Buildings_ParseTile(tinyxml2::XMLNode* tile, BuildingsTileRule_t& tileRule) {
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
		if (FrameNode->FirstChild() != NULL) {
			tileRule.frames.push_back(FrameNode->FirstChild()->ToText()->Value());
		}
		FrameNode = FrameNode->NextSiblingElement("Frame");
	}
}

void Buildings_LoadRuleXML(const char* path, BuildingsXMLInfo_t& info) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path);

	tinyxml2::XMLElement* root = doc.FirstChildElement();
	if (root == NULL)
		return;

	tinyxml2::XMLNode* tilesetTypeClassNode = root->FirstChild();

	tinyxml2::XMLNode* tilesParent = tilesetTypeClassNode->FirstChildElement("Tiles");
	tinyxml2::XMLNode* tile = tilesParent->FirstChildElement("Tile");
	while (tile != NULL) {
		BuildingsTileRule_t tileRule;
		Buildings_ParseTile(tile, tileRule);
		info.tiles.push_back(tileRule);
		tile = tile->NextSiblingElement("Tile");
	}
}

void Buildings_LoadRules(void) {
	Buildings_LoadRuleXML("data/xml/tilesets/RA_STRUCTURES.XML", buildingsXmlRules);
}