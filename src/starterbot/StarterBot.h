#pragma once

#include "MapTools.h"
#include "BaseManager.h"
#include <BWAPI.h>
#include <iostream>
#include <fstream>

class StarterBot
{
	MapTools m_mapTools;

    BaseManager m_baseManager;
	bool m_isFirstBase;

	//Unitset of all resources in a cluster defining a base
    std::vector<BWAPI::Unitset> m_mapBases;

	//Identifies if a base is occupied and by who 
    // 0 = unoccupied base, 1 = your main base, -1 = enemy main base, 2 your expansions, -2 enemy expansions
    std::vector<int> m_baseOccupier;


public:

    StarterBot();

    // helper functions to get you started with bot programming and learn the API
    void drawDebugInformation();

    //Analytic functions
    void computeBases();

    // functions that are triggered by various BWAPI events from main.cpp
	void onStart();
	void onFrame();
	void onEnd(bool isWinner);
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);
	void onSendText(std::string text);
	void onUnitCreate(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onUnitShow(BWAPI::Unit unit);
	void onUnitHide(BWAPI::Unit unit);
	void onUnitRenegade(BWAPI::Unit unit);

};