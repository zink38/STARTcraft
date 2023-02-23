#pragma once

#include "Base.h"

class BaseManager
{

private:
	std::vector<Base> m_bases;
	std::vector<int> *m_mapBaseOccupiers;
	std::vector<BWAPI::Unitset> *m_mapBases;
	MapTools* m_mapTools;

	//Keep these vectors in sync. So that probes have a corresponding command and in the event of a building probe death a new probe can take it's place 
	//without recomputing the command
	BWAPI::Unitset m_buildingProbes;
	BWAPI::Unit m_exploringProbe;

	BWAPI::Unitset m_gateways;
	BWAPI::Unitset m_forges;
	BWAPI::Unitset m_cannons;

	BWAPI::Unit m_cyber;
	BWAPI::Unit m_robo;
	BWAPI::Unit m_obs;

	int m_tiedUpMinerals;
	int m_tiedUpGas;


	size_t m_supplyBuffer;
	size_t m_maxGateways;
	size_t m_maxAssimilators;
	size_t m_maxForge;
	size_t m_maxCannons;
	size_t m_maxObservers;
	size_t m_assimilatorCount;
	size_t m_observerCount;

	bool m_hasCyber;
	bool m_hasRobo;
	bool m_hasObs;

	bool m_isPreparingToUncoverMapBase;

	//bool m_isExpanding;
	bool m_isPreparingToBuildPylon;
	bool m_isPylonBuilding;
	bool m_isPreparingToBuildGateway;
	bool m_isGatewayBuilding;
	bool m_isPreparingToBuildAssimilator;
	bool m_isAssimilatorBuilding;
	bool m_isPreparingToBuildCyber;
	bool m_isCyberBuilding;
	bool m_isPreparingToBuildNexus;
	bool m_isNexusBuilding;
	bool m_isPreparingToBuildForge;
	bool m_isForgeBuilding;
	bool m_isPreparingToBuildCannon;
	bool m_isCannonBuilding;
	bool m_isPreparingToBuildRobo;
	bool m_isRoboBuilding;
	bool m_isPreparingToBuildObs;
	bool m_isObsBuilding;


	
	bool prepareToBuildPylon();
	bool prepareToBuildGateway();
	bool prepareToBuildAssimilator();
	bool prepareToBuildCyber();
	bool prepareToBuildNexus();
	bool prepareToBuildForge();
	bool prepareToBuildCannon();
	bool prepareToBuildRobo();
	bool prepareToBuildObs();

	bool hasCompletedGateway();
	bool hasCompletedForge();

	void prepareToExploreMapBase(BWAPI::Unitset mapBase);

	void trainAdditionalZealot();
	void trainAdditionalDragoon();
	void trainAdditionalObserver();

	void upgradeDragoon();
	void upgradeForge(BWAPI::UpgradeType type);
	void upgradeObserver(BWAPI::UpgradeType type);
	
public:
	BaseManager();
	BaseManager(BWAPI::Unit nexus, std::vector<BWAPI::Unitset> *mapBases, std::vector<int> *mapBaseOccupiers, MapTools *mapTools);

	BWAPI::Unit removeProbe();
	void removeProbe(BWAPI::Unit probe);
	void addProbe(BWAPI::Unit unit);
	void run();
	void addBase(Base base);
	void decTiedUpResourcesAndManageCreatedBuildings(BWAPI::Unit unit);
	void buildingComplete(BWAPI::UnitType type);
	void baseUnitDestroyed(BWAPI::Unit unit);

	void incObserverCount();
	void decObserverCount();

};

