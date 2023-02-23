#pragma once

#include "MapTools.h"
#include "Tools.h"
#include <BWAPI.h>

class Base
{

private:

	BWAPI::Unit m_nexus;
	BWAPI::Unitset m_gases;
	BWAPI::Unitset m_minerals;
	BWAPI::Unitset m_pylons;
	BWAPI::Unitset m_gateways;
	BWAPI::Unitset m_cannons;

	//for if nexus dies
	BWAPI::TilePosition m_position;
	BWAPI::Position		m_retreatPosition;

	std::vector<BWAPI::Unitset> m_mineralProbes;
	std::vector<BWAPI::Unitset> m_gasProbes;

	size_t m_maxProbes;
	size_t m_pylonCount;
	size_t m_gatewayCount;

	bool m_gotBuiltProbe;

	void getBuiltProbe();
	void lockMineralWorkersToMinerals();
	void lockGasWorkersToGas();
	void sendWorkersToGases();
	void setUp();
	
public:
	Base();
	Base(BWAPI::Unit nexus);

	BWAPI::Unitset lostProbes;

	void assignProbes(BWAPI::Unitset probes);
	void assignProbe(BWAPI::Unit probe);

	const BWAPI::Unitset& getGases();
	BWAPI::Unitset getResources();
	BWAPI::Unit getNexus();
	BWAPI::Unit getProbe();
	BWAPI::Unit removeProbe();
	void removeProbe(BWAPI::Unit probe);
	BWAPI::TilePosition getPosition();
	int getProbeCount();
	size_t getPylonCount();
	size_t getGasCount();
	size_t getGatewayCount();
	size_t getCannonCount();
	bool hasMaxProbes();
	bool hasNexus();
	bool hasGas();
	bool hasCompletedPylon();
	const bool canMineGas();
	const bool canBuildAssimilator();
	void addPylon(BWAPI::Unit pylon);
	void removePylon(BWAPI::Unit pylon);
	void addGateway(BWAPI::Unit gateway);
	void removeGateway(BWAPI::Unit gateway);
	void addCannon(BWAPI::Unit gateway);
	void removeCannon(BWAPI::Unit gateway);
	void runBase();
	void trainAdditionalWorkers();
};
