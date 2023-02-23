#include "BaseManager.h"
#include "base.h"

BaseManager::BaseManager()
{
}

BaseManager::BaseManager(BWAPI::Unit nexus, std::vector<BWAPI::Unitset> *mapBases, std::vector<int> *mapBaseOccupiers, MapTools *mapTools)
{
    m_buildingProbes = BWAPI::Unitset();
    m_exploringProbe = BWAPI::Unit();
    m_gateways = BWAPI::Unitset();
    m_forges = BWAPI::Unitset();
    m_cannons = BWAPI::Unitset();
    m_mapBaseOccupiers = mapBaseOccupiers;
    m_mapBases = mapBases;
    m_mapTools = mapTools;
    m_maxGateways = 0;
    m_maxCannons = 0;
    m_maxAssimilators = 0;
    m_maxForge = 0;
    m_maxObservers = 2;
	addBase(Base(nexus));
	BWAPI::Unitset units = BWAPI::Broodwar->self()->getUnits();
	units.erase(Tools::GetDepot());
	m_bases[0].assignProbes(units);
    m_tiedUpMinerals = 0;
    m_tiedUpGas = 0;
    m_assimilatorCount = 0;
    m_observerCount = 0;
    m_hasCyber = false;
    m_hasRobo = false;
    m_hasObs = false;
    m_cyber = BWAPI::Unit();
    m_robo = BWAPI::Unit();
    m_obs = BWAPI::Unit();
    m_isPreparingToBuildPylon = false;
    m_isPylonBuilding = false;
    m_isPreparingToBuildGateway = false;
    m_isGatewayBuilding = false;
    m_isPreparingToBuildAssimilator = false;
    m_isAssimilatorBuilding = false;
    m_isPreparingToBuildCyber = false;
    m_isCyberBuilding = false;
    m_isPreparingToBuildNexus = false;
    m_isNexusBuilding = false;
    m_isPreparingToBuildForge = false;
    m_isForgeBuilding = false;
    m_isPreparingToBuildCannon = false;
    m_isCannonBuilding = false;
    m_isPreparingToBuildRobo = false;
    m_isRoboBuilding = false;
    m_isPreparingToBuildObs = false;
    m_isObsBuilding = false;

    m_isPreparingToUncoverMapBase = false;
}

void BaseManager::addBase(Base base)
{
    //adjust amount of production per base
	m_bases.push_back(base);
    m_maxCannons += 3;
    m_maxGateways += 4;
    if (base.getGasCount() > 0)
    {
        m_maxAssimilators += base.getGasCount();
        if (m_maxForge < 3)
        {
            m_maxForge ++;
        }
    }
    
}

void BaseManager::run()
{
    //std::cout << m_bases.size() << std::endl;
    //std::cout << "Minerals: " << BWAPI::Broodwar->self()->minerals() << " - TiedUpMinerals: " << m_tiedUpMinerals << " = "
    //   << BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals << std::endl;

    for (auto mapBase : *m_mapBases)
    {

    }

    bool adjusted = false;
    BWAPI::Unitset toErase = BWAPI::Unitset();
    for (auto probe : m_buildingProbes)
    {
        if (probe->isIdle())
        {
            adjusted = true;
            addProbe(probe);
            toErase.insert(probe);
        }
    }

    if (adjusted)
    {
        for (auto probe : toErase)
        {
            m_buildingProbes.erase(probe);
        }
    }

    if (adjusted && m_buildingProbes.size() == 0)
    {
        if (m_isPreparingToBuildPylon)
        {
            m_isPreparingToBuildPylon = false;
            m_tiedUpMinerals -= 100;
            //std::cout << "PylonFailed" << std::endl;
        }
        if (m_isPreparingToBuildGateway)
        {
            m_isPreparingToBuildGateway = false;
            m_tiedUpMinerals -= 150;
            //std::cout << "GatewayFailed" << std::endl;
        }
        if (m_isPreparingToBuildAssimilator)
        {
            m_isPreparingToBuildAssimilator = false;
            m_tiedUpMinerals -= 100;
            //std::cout << "AssimilatorFailed" << std::endl;
        }
        if (m_isPreparingToBuildCyber)
        {
            m_isPreparingToBuildCyber = false;
            m_tiedUpMinerals -= 200;
            //std::cout << "CyberFailed" << std::endl;
        }
        if (m_isPreparingToBuildNexus)
        {
            m_isPreparingToBuildNexus = false;
            m_tiedUpMinerals -= 400;
            //std::cout << "NexusFailed" << std::endl;
        }
        if (m_isPreparingToBuildForge)
        {
            m_isPreparingToBuildForge = false;
            m_tiedUpMinerals -= 150;
            //std::cout << "ForgeFailed" << std::endl;
        }
        if (m_isPreparingToBuildCannon)
        {
            m_isPreparingToBuildCannon = false;
            m_tiedUpMinerals -= 150;
            //std::cout << "CannonFailed" << std::endl;
        }
        if (m_isPreparingToBuildRobo)
        {
            m_isPreparingToBuildRobo = false;
            m_tiedUpMinerals -= BWAPI::UnitTypes::Protoss_Robotics_Facility.mineralPrice();
            m_tiedUpGas -= BWAPI::UnitTypes::Protoss_Robotics_Facility.gasPrice();
            //std::cout << "RoboFailed" << std::endl;
        }
        if (m_isPreparingToBuildObs)
        {
            m_isPreparingToBuildObs = false;
            m_tiedUpMinerals -= BWAPI::UnitTypes::Protoss_Observatory.mineralPrice();
            m_tiedUpGas -= BWAPI::UnitTypes::Protoss_Observatory.gasPrice();
            //std::cout << "ObsFailed" << std::endl;
        }
    }

    if (m_exploringProbe && m_exploringProbe->isIdle())
    {
        addProbe(m_exploringProbe);
        if (m_isPreparingToUncoverMapBase)
        {
            m_isPreparingToUncoverMapBase = false;
        }
        m_exploringProbe = BWAPI::Unit();
    }


    /*if (m_isPreparingToUncoverMapBase)
    {
        m_isPreparingToUncoverMapBase = false;
    }*/
    size_t j = 0;
    for (Base& base : m_bases)
    {
        size_t i = 0;
        for (Base& nestedBase : m_bases)
        {
            
            if (base.getProbeCount() < nestedBase.getProbeCount() - 2)
            {
                auto probe = nestedBase.removeProbe();
                base.assignProbe(probe);
                //std::cout << "Base: " << j << " Trasnfered Probe: " << probe->getID() << " To Base: " << i << std::endl;
            }
            i++;
        }
        j++;
    }

    if (m_tiedUpMinerals < 0)
    {
        m_tiedUpMinerals = 0;
    }


    BWAPI::UnitType type = BWAPI::UnitType();
    BWAPI::UpgradeType upgrade = BWAPI::UpgradeType();
    int level = 0;


    
    /*if (!m_isPreparingToBuildNexus && !m_isNexusBuilding && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 400) )
    {
        if (prepareToBuildNexus())
        {
            m_tiedUpMinerals += 400;
            m_isPreparingToBuildNexus = true;
        }
    }*/

    //BUILD ORDER / DECISION TREE
    type = BWAPI::UnitTypes::Protoss_Robotics_Facility;
    if (!m_isPreparingToBuildRobo && !m_isRoboBuilding && !m_hasRobo && m_hasCyber && m_cyber->isCompleted() && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= type.mineralPrice()) && (BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= type.gasPrice()))
    {
        if (prepareToBuildRobo())
        {
            m_tiedUpMinerals += type.mineralPrice();
            m_tiedUpGas += type.gasPrice();
            m_isPreparingToBuildRobo = true;
        }
    }

    type = BWAPI::UnitTypes::Protoss_Observatory;
    if (!m_isPreparingToBuildObs && !m_isObsBuilding && !m_hasObs && m_hasRobo && m_robo->isCompleted() && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= type.mineralPrice()) && (BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= type.gasPrice()))
    {
        if (prepareToBuildObs())
        {
            m_tiedUpMinerals += type.mineralPrice();
            m_tiedUpGas += type.gasPrice();
            m_isPreparingToBuildObs = true;
        }
    }

    if (!m_isPreparingToBuildCyber && !m_isCyberBuilding && !m_hasCyber && hasCompletedGateway() && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 200))
    {
        if (prepareToBuildCyber())
        {
            m_tiedUpMinerals += 200;
            m_isPreparingToBuildCyber = true;
        }
    }

    if (m_hasCyber && !m_isPreparingToBuildAssimilator && !m_isAssimilatorBuilding && (m_assimilatorCount < m_maxAssimilators) && m_gateways.size() >= m_maxGateways / 2 && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 100))
    {
        if (prepareToBuildAssimilator())
        {
            m_tiedUpMinerals += 100;
            m_isPreparingToBuildAssimilator = true;
        }
    }

    if (!m_isPreparingToBuildCannon && !m_isCannonBuilding && hasCompletedForge() && (m_cannons.size() < m_maxCannons) && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 150) && m_cannons.size() < m_gateways.size())
    {
        if (prepareToBuildCannon())
        {
            m_tiedUpMinerals += 150;
            m_isPreparingToBuildCannon = true;
        }
    }

    if (!m_isPreparingToBuildForge && !m_isForgeBuilding && (m_forges.size() < m_maxForge) && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 150) && m_forges.size() < m_gateways.size() && (m_forges.size() < 1 || m_hasCyber))
    {
        if (prepareToBuildForge())
        {
            m_tiedUpMinerals += 150;
            m_isPreparingToBuildForge = true;
        }
    }

    if (!m_isPreparingToBuildGateway && !m_isGatewayBuilding && (m_gateways.size() < m_maxGateways) && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 150) && (m_gateways.size() < 1 || m_hasCyber) )
    {
        if (prepareToBuildGateway())
        {
            m_tiedUpMinerals += 150;
            m_isPreparingToBuildGateway = true;
        }
    }

    if (!m_isPreparingToBuildPylon && !m_isPylonBuilding && (BWAPI::Broodwar->self()->supplyTotal() - BWAPI::Broodwar->self()->supplyUsed() < 11) && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 100))
    {
        if (prepareToBuildPylon())
        {
            m_tiedUpMinerals += 100;
            m_isPreparingToBuildPylon = true;
        }
    }

    for (Base& base : m_bases)
    {
        base.runBase();
        if (!base.hasNexus())
        {
            for (Base& base2 : m_bases)
            {
                if (base2.hasNexus())
                {
                    base2.assignProbes(base.lostProbes);
                    base.lostProbes.clear();
                }
            }
        }
        else
        {
            if (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 50 && !base.hasMaxProbes())
            {
                base.trainAdditionalWorkers();
            }
        }
        
    }
    size_t constraints = 1;
    if ( (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) == 1))
    {
        constraints = 3;
    }
    for (size_t i = 0; i < constraints; i++)
    {
        if (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 125 && BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= 50 && m_gateways.size() > 0 && m_hasCyber)
        {
            trainAdditionalDragoon();
        }
    }

    if ( !(BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) == 1) && BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 150 && BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= 150 && m_hasCyber)
    {
        upgradeDragoon();
    }
    
    type = BWAPI::UnitTypes::Protoss_Observer;
    if (m_hasRobo && m_hasObs && m_robo->isCompleted() && m_obs->isCompleted() && m_observerCount < m_maxObservers && (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= type.mineralPrice()) && (BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= type.gasPrice()))
    {
        trainAdditionalObserver();
    }
    for (size_t i = 0; i < 2; i++)
    {
        if (BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= 100 && m_gateways.size() > 0)
        {
            trainAdditionalZealot();
        }
    }
    
    
    //Disable Observer Upgrades not useful at the moment pick one upgrade for now

    upgrade = BWAPI::UpgradeTypes::Gravitic_Boosters;
    level = BWAPI::Broodwar->self()->getUpgradeLevel(upgrade);
    if (m_hasObs && m_obs->isCompleted() && m_observerCount > 0 &&(level != 1) && BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= upgrade.mineralPrice()  && BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= upgrade.gasPrice()  && (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) == 1 || m_cyber->isUpgrading()))
    {
        upgradeObserver(upgrade);
    }
    /*upgrade = BWAPI::UpgradeTypes::Sensor_Array;
    level = BWAPI::Broodwar->self()->getUpgradeLevel(upgrade);
    if (m_hasObs && m_obs->isCompleted() && m_observerCount > 0 && (level != 1) && BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= upgrade.mineralPrice() && BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= upgrade.gasPrice() && (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) == 1 || m_cyber->isUpgrading()))
    {
        upgradeObserver(upgrade);
    }*/
    
    //std::cout << "Ground Attack Upgrade Cost: " << BWAPI::UpgradeTypes::Protoss_Ground_Weapons.mineralPrice() << std::endl;
    upgrade = BWAPI::UpgradeTypes::Protoss_Ground_Weapons;
    level = BWAPI::Broodwar->self()->getUpgradeLevel(upgrade);
    if (  (level < 3) && BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= upgrade.mineralPrice() + level*upgrade.mineralPriceFactor() && BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= upgrade.gasPrice() + level*upgrade.gasPriceFactor() && (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) == 1 || m_cyber->isUpgrading()) && hasCompletedForge())
    {
        upgradeForge(upgrade);
    }
    upgrade = BWAPI::UpgradeTypes::Protoss_Ground_Armor;
    level = BWAPI::Broodwar->self()->getUpgradeLevel(upgrade);
    if ( (level < 3) && BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= upgrade.mineralPrice() + level * upgrade.mineralPriceFactor() && BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= upgrade.gasPrice() + level * upgrade.gasPriceFactor() && (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) == 1 || m_cyber->isUpgrading()) && hasCompletedForge())
    {
        upgradeForge(upgrade);
    }
    upgrade = BWAPI::UpgradeTypes::Protoss_Plasma_Shields;
    level = BWAPI::Broodwar->self()->getUpgradeLevel(upgrade);
    if (m_observerCount > 0 && (level < 3) && BWAPI::Broodwar->self()->minerals() - m_tiedUpMinerals >= upgrade.mineralPrice() + level * upgrade.mineralPriceFactor() && BWAPI::Broodwar->self()->gas() - m_tiedUpGas >= upgrade.gasPrice() + level * upgrade.gasPriceFactor() && (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) == 1 || m_cyber->isUpgrading()) && hasCompletedForge())
    {
        upgradeForge(upgrade);
    }
}

bool BaseManager::hasCompletedGateway()
{
    if (!(m_gateways.size() > 0))
    {
        return false;
    }
    else {
        for (auto& gateway : m_gateways)
        {
            if (gateway->isCompleted())
            {
                return true;
            }
        }
    }
    return false;
}

bool BaseManager::hasCompletedForge()
{
    if (!(m_forges.size() > 0))
    {
        return false;
    }
    else {
        for (auto& forge : m_forges)
        {
            if (forge->isCompleted())
            {
                return true;
            }
        }
    }
    return false;
}

void BaseManager::decTiedUpResourcesAndManageCreatedBuildings(BWAPI::Unit unit)
{
    const auto& type = unit->getType();
    //std::cout << type.getName() << ": " << type.mineralPrice() << std::endl;
    m_tiedUpMinerals -= type.mineralPrice();
    m_tiedUpGas -= type.gasPrice();
    if (type == BWAPI::UnitTypes::Protoss_Pylon)
    {
        m_isPreparingToBuildPylon = false;
        m_isPylonBuilding = true;
        Base *selectedBase = &m_bases[0];
        for (size_t i = 1; i < m_bases.size(); i++)
        {
            if (unit->getPosition().getApproxDistance(m_bases[i].getResources().getPosition()) < unit->getPosition().getApproxDistance(selectedBase->getResources().getPosition()))
            {
                selectedBase = &m_bases[i];
            }
        }
        selectedBase->addPylon(unit);
    }
    else if (type == BWAPI::UnitTypes::Protoss_Gateway)
    {
        m_isPreparingToBuildGateway = false;
        m_isGatewayBuilding = true;
        m_gateways.insert(unit);
        Base* selectedBase = &m_bases[0];
        for (size_t i = 1; i < m_bases.size(); i++)
        {
            if (unit->getPosition().getApproxDistance(m_bases[i].getResources().getPosition()) < unit->getPosition().getApproxDistance(selectedBase->getResources().getPosition()))
            {
                selectedBase = &m_bases[i];
            }
        }
        selectedBase->addGateway(unit);
    }
    else if (type == BWAPI::UnitTypes::Protoss_Assimilator)
    {
        m_isPreparingToBuildAssimilator = false;
        m_isAssimilatorBuilding = true;
        m_assimilatorCount++;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Cybernetics_Core)
    {
        m_isPreparingToBuildCyber = false;
        m_isCyberBuilding = true;
        m_cyber = unit;
        m_hasCyber = true;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Nexus)
    {
        m_isPreparingToBuildNexus = false;
        m_isNexusBuilding = true;
        
    }
    else if (type == BWAPI::UnitTypes::Protoss_Forge)
    {
        m_isPreparingToBuildForge = false;
        m_isForgeBuilding = true;
        m_forges.insert(unit);

    }
    else if (type == BWAPI::UnitTypes::Protoss_Photon_Cannon)
    {
        m_isPreparingToBuildCannon = false;
        m_isCannonBuilding = true;
        m_cannons.insert(unit);
        Base* selectedBase = &m_bases[0];
        for (size_t i = 1; i < m_bases.size(); i++)
        {
            if (unit->getPosition().getApproxDistance(m_bases[i].getResources().getPosition()) < unit->getPosition().getApproxDistance(selectedBase->getResources().getPosition()))
            {
                selectedBase = &m_bases[i];
            }
        }
        selectedBase->addCannon(unit);
    }
    else if (type == BWAPI::UnitTypes::Protoss_Robotics_Facility)
    {
        m_isPreparingToBuildRobo = false;
        m_isRoboBuilding = true;
        m_robo = unit;
        m_hasRobo = true;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Observatory)
    {
        m_isPreparingToBuildObs = false;
        m_isObsBuilding = true;
        m_obs = unit;
        m_hasObs = true;
    }
}

void BaseManager::buildingComplete(BWAPI::UnitType type)
{
    if (type == BWAPI::UnitTypes::Protoss_Pylon)
    {
       m_isPylonBuilding = false;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Gateway)
    {
        m_isGatewayBuilding = false;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Assimilator)
    {
        m_isAssimilatorBuilding = false;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Cybernetics_Core)
    {
        m_isCyberBuilding = false;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Nexus)
    {
        m_isNexusBuilding = false;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Forge)
    {
        m_isForgeBuilding = false;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Photon_Cannon)
    {
        m_isCannonBuilding = false;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Robotics_Facility)
    {
        m_isRoboBuilding = false;
    }
    else if (type == BWAPI::UnitTypes::Protoss_Observatory)
    {
        m_isObsBuilding = false;
    }

}

void BaseManager::baseUnitDestroyed(BWAPI::Unit unit)
{
    auto type = unit->getType();
    if (type == BWAPI::UnitTypes::Protoss_Probe)
    {
        removeProbe(unit);
    }
    else if (type == BWAPI::UnitTypes::Protoss_Pylon)
    {
        if (!unit->isCompleted())
        {
            m_isPylonBuilding = false;
        }
        for (auto& base : m_bases)
        {
            base.removePylon(unit);
        }
    }
    else if (type == BWAPI::UnitTypes::Protoss_Gateway)
    {
        if (!unit->isCompleted())
        {
            m_isGatewayBuilding = false;
        }
        m_gateways.erase(unit);
        for (auto& base : m_bases)
        {
            base.removeGateway(unit);
        }
    }
    else if (type == BWAPI::UnitTypes::Protoss_Assimilator)
    {
        if (!unit->isCompleted())
        {
            m_isAssimilatorBuilding = false;
        }
    }
    else if (type == BWAPI::UnitTypes::Protoss_Cybernetics_Core)
    {
        if (!unit->isCompleted())
        {
            m_isCyberBuilding = false;
        }
        m_hasCyber = false;
        m_cyber = BWAPI::Unit();
    }
    else if (type == BWAPI::UnitTypes::Protoss_Nexus)
    {
    }
    else if (type == BWAPI::UnitTypes::Protoss_Forge)
    {
        if (!unit->isCompleted())
        {
            m_isForgeBuilding = false;
        }
        m_forges.erase(unit);
    }
    else if (type == BWAPI::UnitTypes::Protoss_Photon_Cannon)
    {
        if (!unit->isCompleted())
        {
            m_isCannonBuilding = false;
        }
        m_cannons.erase(unit);

        for (auto& base : m_bases)
        {
            base.removeCannon(unit);
        }
    }
    else if (type == BWAPI::UnitTypes::Protoss_Robotics_Facility)
    {
        if (!unit->isCompleted())
        {
            m_isRoboBuilding = false;
        }
        m_hasRobo = false;
        m_robo = BWAPI::Unit();
    }
    else if (type == BWAPI::UnitTypes::Protoss_Observatory)
    {
        if (!unit->isCompleted())
        {
            m_isObsBuilding = false;
        }
        m_hasObs = false;
        m_obs = BWAPI::Unit();
    }
}



bool BaseManager::prepareToBuildPylon()
{
    const int maxBuildRange = std::rand()%300;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Pylon;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    Base* selectedBase = &m_bases[0];

    //std::cout << "Preparing to build: " << type.getName() << std::endl;

    //Select Base with fewest Pylons 
    for (size_t i = 1; i < m_bases.size(); i++)
    {
        if (m_bases[i].getPylonCount() < selectedBase->getPylonCount())
        {
            selectedBase = &m_bases[i];
        }
    }
    
    //Get a desirable build position near the centre of the base
    if(selectedBase->hasNexus())
    desiredPos = selectedBase->getPosition();
    //Adjust Distance from Nexus
    if (BWAPI::Broodwar->self()->supplyUsed() > 52)
    {
        buildPos = BWAPI::Broodwar->getBuildLocation(type, desiredPos, maxBuildRange);
    }
    else
    {
        buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Nexus, desiredPos, maxBuildRange);
        buildPos = BWAPI::Broodwar->getBuildLocation(type, buildPos, maxBuildRange);
    }
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //td::cout << type.getName() << ": "<< "INVALID POSITION" << std::endl;
        return false;
    }

    //Get a probe 
    for(Base & base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the " << type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos);
}

bool BaseManager::prepareToBuildForge()
{
    const int maxBuildRange = std::rand() % 60;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Forge;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    BWAPI::TilePosition vector = BWAPI::TilePosition();
    BWAPI::TilePosition vector1 = BWAPI::TilePosition();
    //Used for vector math
    int tmp = 0;
    Base* selectedBase = &m_bases[0];

    //std::cout << "Preparing to build: " << type.getName() << std::endl;

    //Select Base that has a Pylon 
    for (size_t i = 1; i < m_bases.size(); i++)
    {
        if (m_bases[i].hasCompletedPylon())
        {
            selectedBase = &m_bases[i];
            break;
        }
    }

    //Get a desirable build position near the centre of the base
    desiredPos = selectedBase->getPosition();
    //Adjust Distance from Nexus
    vector = desiredPos;
    vector1 = BWAPI::TilePosition(selectedBase->getResources().getPosition());
    vector = vector - vector1;
    //build towards minerals
    desiredPos -= vector;
    //rotated vector [put cyber away from front]
    if (vector.x != 0)
    {
        vector.x = vector.x / std::abs(vector.x);
    }
    if (vector.y != 0)
    {
        vector.y = vector.y / std::abs(vector.y);
    }
    vector.x *= -1;
    tmp = vector.y;
    vector.y = vector.x;
    vector.x = tmp;
    //Add rotated vector to side step workers
    desiredPos += vector;
    buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Nexus, desiredPos, maxBuildRange);
    buildPos = BWAPI::Broodwar->getBuildLocation(type, buildPos, maxBuildRange);
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //std::cout << type.getName() << ": " << "INVALID POSITION" << std::endl;
        return false;
    }
    /*if (!BWAPI::Broodwar->hasPower(buildPos))
    {
        std::cout << type.getName() << ": " << "No Power" << std::endl;
        return false;
    }*/

    //Get a probe 
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the " << type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos);
    
}

bool BaseManager::prepareToBuildGateway()
{
    const int maxBuildRange = (std::rand() % 300) + 100;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Gateway;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    BWAPI::TilePosition vector = BWAPI::TilePosition();
    BWAPI::TilePosition vector1 = BWAPI::TilePosition();
    Base* selectedBase = &m_bases[0];

    //std::cout << "Preparing to build: " << type.getName() << std::endl;

    //Select Base with fewest Gateways that has a pylon 
    for (size_t i = 1; i < m_bases.size(); i++)
    {
        if (m_bases[i].hasCompletedPylon()) {
            if (m_bases[i].getGatewayCount() < selectedBase->getGatewayCount())
            {
                selectedBase = &m_bases[i];
            }
        } 
    }

    //Get a desirable build position near the centre of the base
    desiredPos = selectedBase->getPosition();
    //Adjust Distance from Nexus
    vector = desiredPos;
    vector1 = BWAPI::TilePosition(selectedBase->getResources().getPosition());
    vector = vector - vector1;
    desiredPos += vector;
    if (BWAPI::Broodwar->self()->supplyUsed() > 52)
    {
        buildPos = BWAPI::Broodwar->getBuildLocation(type, desiredPos, maxBuildRange);
    }
    else
    {
        buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Nexus, desiredPos, maxBuildRange);
        buildPos = BWAPI::Broodwar->getBuildLocation(type, buildPos, maxBuildRange);
    }
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //std::cout << type.getName() << ": " << "INVALID POSITION" << std::endl;
        return false;
    }
    /*if (!BWAPI::Broodwar->hasPower(buildPos))
    {
        std::cout << type.getName() << ": " << "No Power" << std::endl;
        return false;
    }*/

    //Get a probe 
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the "<< type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos);
}

bool BaseManager::prepareToBuildAssimilator()
{
    const int maxBuildRange = 3;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Assimilator;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    BWAPI::Unitset gases = BWAPI::Unitset();
    bool desiredPositionSet = false;
    Base* selectedBase = &m_bases[0];

    //std::cout << "Preparing to build: " << type.getName() << std::endl;

    //Select Base with fewest Pylons 
    for (size_t i = 1; i < m_bases.size(); i++)
    {
        if (m_bases[i].hasGas() && m_bases[i].canBuildAssimilator())
        {
            selectedBase = &m_bases[i];
        }
    }

    if (!selectedBase->hasGas() || !selectedBase->canBuildAssimilator())
    {
        //std::cout << "Can not build Assimilator" << std::endl;
        return false;
    }


    //Get a desirable build position on gas geyser
    gases = selectedBase->getGases();
    for (auto gas : gases)
    {
        if (!gas->getType().isRefinery())
        {
            desiredPos = gas->getTilePosition();
            desiredPositionSet = true;
            break;
        }
    }

    if (!desiredPositionSet)
    {
        //std::cout << "NO GAS TO BUILD ON" << std::endl;
        return false;
    }

    buildPos = BWAPI::Broodwar->getBuildLocation(type, desiredPos, maxBuildRange);
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //std::cout << type.getName() << ": " << "INVALID POSITION" << std::endl;
        return false;
    }

    //Get a probe 
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the " << type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos);
}



bool BaseManager::prepareToBuildCyber()
{
    const int maxBuildRange = (std::rand() % 60) + 10;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    BWAPI::TilePosition vector = BWAPI::TilePosition();
    BWAPI::TilePosition vector1 = BWAPI::TilePosition();
    //Used for vector math
    int tmp = 0;
    Base* selectedBase = &m_bases[0];

    //std::cout << "Preparing to build: " << type.getName() << std::endl;

    //Select Base that has a Pylon 
    for (size_t i = 1; i < m_bases.size(); i++)
    {
        if (m_bases[i].hasCompletedPylon())
        {
            selectedBase = &m_bases[i];
            break;
        }
    }

    //Get a desirable build position near the centre of the base
    desiredPos = selectedBase->getPosition();
    //Adjust Distance from Nexus
    vector = desiredPos;
    vector1 = BWAPI::TilePosition(selectedBase->getResources().getPosition());
    vector = vector - vector1;
    //build towards minerals
    desiredPos -= vector;
    //rotated vector [put cyber away from front]
    if (vector.x != 0)
    {
        vector.x = vector.x / std::abs(vector.x);
    }
    if (vector.y != 0)
    {
        vector.y = vector.y / std::abs(vector.y);
    }
    vector.x *= -1;
    tmp = vector.y;
    vector.y = vector.x;
    vector.x = tmp;
    //Add rotated vector to side step workers
    desiredPos += vector;

    buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Nexus, desiredPos, maxBuildRange);
    buildPos = BWAPI::Broodwar->getBuildLocation(type, buildPos, maxBuildRange);
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //std::cout << type.getName() << ": " << "INVALID POSITION" << std::endl;
        return false;
    }
    /*if (!BWAPI::Broodwar->hasPower(buildPos))
    {
        std::cout << type.getName() << ": " << "No Power" << std::endl;
        return false;
    }*/

    //Get a probe 
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the " << type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos);
}

bool BaseManager::prepareToBuildCannon()
{
    const int maxBuildRange = std::rand() % 150;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Photon_Cannon;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    Base* selectedBase = &m_bases[0];

    //std::cout << "Preparing to build: " << type.getName() << std::endl;

    //Select Base with fewest Gateways that has a pylon 
    for (size_t i = 1; i < m_bases.size(); i++)
    {
        if (m_bases[i].hasCompletedPylon()) {
            if (m_bases[i].getCannonCount() < selectedBase->getCannonCount())
            {
                selectedBase = &m_bases[i];
            }
        }
    }

    //Get a desirable build position near the centre of the base
    desiredPos = selectedBase->getPosition();
    
    buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Nexus, desiredPos, maxBuildRange);
    buildPos = BWAPI::Broodwar->getBuildLocation(type, buildPos, maxBuildRange);
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //std::cout << type.getName() << ": " << "INVALID POSITION" << std::endl;
        return false;
    }
    /*if (!BWAPI::Broodwar->hasPower(buildPos))
    {
        std::cout << type.getName() << ": " << "No Power" << std::endl;
        return false;
    }*/

    //Get a probe 
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the " << type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos);
}

bool BaseManager::prepareToBuildRobo()
{
    const int maxBuildRange = std::rand() % 60;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Robotics_Facility;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    BWAPI::TilePosition vector = BWAPI::TilePosition();
    BWAPI::TilePosition vector1 = BWAPI::TilePosition();
    //Used for vector math
    int tmp = 0;
    Base* selectedBase = &m_bases[0];

    //std::cout << "Preparing to build: " << type.getName() << std::endl;

    //Select Base that has a Pylon 
    for (size_t i = 1; i < m_bases.size(); i++)
    {
        if (m_bases[i].hasCompletedPylon())
        {
            selectedBase = &m_bases[i];
            break;
        }
    }

    //Get a desirable build position near the centre of the base
    desiredPos = selectedBase->getPosition();
    //Adjust Distance from Nexus
    vector = desiredPos;
    vector1 = BWAPI::TilePosition(selectedBase->getResources().getPosition());
    vector = vector - vector1;
    //build towards minerals
    desiredPos -= vector;
    //rotated vector [put cyber away from front]
    if (vector.x != 0)
    {
        vector.x = vector.x / std::abs(vector.x);
    }
    if (vector.y != 0)
    {
        vector.y = vector.y / std::abs(vector.y);
    }
    vector.x *= -1;
    tmp = vector.y;
    vector.y = vector.x;
    vector.x = tmp;
    //Add rotated vector to side step workers
    desiredPos += vector;

    buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Nexus, desiredPos, maxBuildRange);
    buildPos = BWAPI::Broodwar->getBuildLocation(type, buildPos, maxBuildRange);
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //std::cout << type.getName() << ": " << "INVALID POSITION" << std::endl;
        return false;
    }
    /*if (!BWAPI::Broodwar->hasPower(buildPos))
    {
        std::cout << type.getName() << ": " << "No Power" << std::endl;
        return false;
    }*/

    //Get a probe 
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the " << type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos);
}

bool BaseManager::prepareToBuildObs()
{
    const int maxBuildRange = std::rand() % 60;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Observatory;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    BWAPI::TilePosition vector = BWAPI::TilePosition();
    BWAPI::TilePosition vector1 = BWAPI::TilePosition();
    //Used for vector math
    int tmp = 0;
    Base* selectedBase = &m_bases[0];

    //std::cout << "Preparing to build: " << type.getName() << std::endl;

    //Select Base that has a Pylon 
    for (size_t i = 1; i < m_bases.size(); i++)
    {
        if (m_bases[i].hasCompletedPylon())
        {
            selectedBase = &m_bases[i];
            break;
        }
    }

    //Get a desirable build position near the centre of the base
    desiredPos = selectedBase->getPosition();
    //Adjust Distance from Nexus
    vector = desiredPos;
    vector1 = BWAPI::TilePosition(selectedBase->getResources().getPosition());
    vector = vector - vector1;
    //build towards minerals
    desiredPos -= vector;
    //rotated vector [put cyber away from front]
    if (vector.x != 0)
    {
        vector.x = vector.x / std::abs(vector.x);
    }
    if (vector.y != 0)
    {
        vector.y = vector.y / std::abs(vector.y);
    }
    vector.x *= -1;
    tmp = vector.y;
    vector.y = vector.x;
    vector.x = tmp;
    //Add rotated vector to side step workers
    desiredPos += vector;

    buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Nexus, desiredPos, maxBuildRange);
    buildPos = BWAPI::Broodwar->getBuildLocation(type, buildPos, maxBuildRange);
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //std::cout << type.getName() << ": " << "INVALID POSITION" << std::endl;
        return false;
    }
    /*if (!BWAPI::Broodwar->hasPower(buildPos))
    {
        std::cout << type.getName() << ": " << "No Power" << std::endl;
        return false;
    }*/

    //Get a probe 
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the " << type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos);
}

/*bool BaseManager::prepareToBuildNexus()
{
    const int maxBuildRange = 7;
    const BWAPI::UnitType type = BWAPI::UnitTypes::Protoss_Nexus;
    BWAPI::Unit buildingProbe = BWAPI::Unit();
    BWAPI::TilePosition desiredPos = BWAPI::TilePosition();
    BWAPI::TilePosition buildPos = BWAPI::TilePosition();
    BWAPI::Unitset selectedMapBase = BWAPI::Unitset();

    std::cout << "Preparing to build: " << type.getName() << std::endl;

    size_t i = 0;
    int leastDistance = -1;
    for (auto& mapBase : *m_mapBases)
    {
        if ((*m_mapBaseOccupiers)[i] == 0)
        {
            //skip over island bases
            if (!Tools::GetUnitOfType(BWAPI::UnitTypes::Protoss_Probe)->hasPath((*mapBase.cbegin())))
            {
                std::cout << "IslandBase" << std::endl;
                continue;
            }
            if (leastDistance < 0)
            {
                selectedMapBase = mapBase;
                leastDistance = mapBase.getPosition().getApproxDistance(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
            }
            else if (mapBase.getPosition().getApproxDistance(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation())) < leastDistance)
            {
                selectedMapBase = mapBase;
                leastDistance = mapBase.getPosition().getApproxDistance(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
            }
        }
        i++;
    }
    if (!(*m_mapTools).isExplored(selectedMapBase.getPosition()))
    {
        if (!m_isPreparingToUncoverMapBase)
        {
            prepareToExploreMapBase(selectedMapBase);
        }

        return false;
    }
    if (selectedMapBase.size() == 0)
    {
        //std::cout << "No bases are available" << std::endl;
        return false;
    }

    //Get a desirable build position near the centre of the base
    desiredPos = BWAPI::TilePosition(selectedMapBase.getPosition());
    if (buildPos == BWAPI::TilePositions::Invalid || !buildPos.isValid())
    {
        //td::cout << type.getName() << ": "<< "INVALID POSITION" << std::endl;
        return false;
    }

    //Get a probe 
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            if (base.getProbe()->hasPath(BWAPI::Position(buildPos)) && base.getProbe()->canBuild(type, buildPos))
            {
                buildingProbe = base.removeProbe();
                m_buildingProbes.insert(buildingProbe);
                break;
            }
        }
    }
    if (!buildingProbe)
    {
        //std::cout << "There are no probes or no probes can build the " << type.getName() << std::endl;
        return false;
    }
    return buildingProbe->build(type, buildPos); */
    ///////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    /*std::cout << "Prepare Nexus" << std::endl;
    //std::cout << m_tiedUpMinerals << std::endl;
    BWAPI::TilePosition desiredPos;
    BWAPI::Unitset selectedMapBase;
    
    desiredPos = BWAPI::TilePosition(selectedMapBase.getPosition());
    //std::cout << "Base Selected" << std::endl;
    // Ask BWAPI for a building location near the desired position for the type
    const int maxBuildRange = 7;
    const BWAPI::TilePosition buildPos = BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Nexus, desiredPos, maxBuildRange);
    if (buildPos == BWAPI::TilePositions::Invalid)
    {
        //std::cout << "INVALID POSITION" << std::endl;
        return false;
    }
    //std::cout << "Build Location" << std::endl;
    //Select building probe
    BWAPI::Unit buildingProbe = BWAPI::Unit();
   
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            
            buildingProbe = base.removeProbe();
            if (buildingProbe->canBuild(BWAPI::UnitTypes::Protoss_Nexus, buildPos))
            {
                m_buildingProbes.insert(buildingProbe);
                break;
            }
            else 
            {
                base.assignProbe(buildingProbe);
                buildingProbe = BWAPI::Unit();
            }
           
        }
        else
        {
            std::cout << "No Probes" << std::endl;
        }
    }
    
    
    //std::cout << "SelectProbe" << std::endl;
    return buildingProbe->build(BWAPI::UnitTypes::Protoss_Nexus, buildPos);*/
//} 

void BaseManager::prepareToExploreMapBase(BWAPI::Unitset mapBase)
{
    //std::cout << "A Brave new world" << std::endl;
    BWAPI::Unit exploringProbe = BWAPI::Unit();
    for (Base& base : m_bases)
    {
        if (base.getProbeCount() > 0)
        {
            m_isPreparingToUncoverMapBase = true;
            exploringProbe = base.removeProbe();
            
        }
    }

    exploringProbe->move(mapBase.getPosition());
    m_buildingProbes.insert(exploringProbe);
}

void BaseManager::trainAdditionalZealot()
{
    for (auto& gateway : m_gateways)
    {
        if (gateway->isCompleted() && !gateway->isTraining())
        {
            gateway->train(BWAPI::UnitTypes::Protoss_Zealot);
            break;
        }
    }
}

void BaseManager::trainAdditionalDragoon()
{
    for (auto& gateway : m_gateways)
    {
        if (gateway->isCompleted() && !gateway->isTraining())
        {
            gateway->train(BWAPI::UnitTypes::Protoss_Dragoon);
            break;
        }
    }
}

void BaseManager::trainAdditionalObserver()
{
    if (m_robo && m_robo->isCompleted() && !m_robo->isTraining())
    {
        m_robo->train(BWAPI::UnitTypes::Protoss_Observer);
    }
}

void BaseManager::upgradeDragoon()
{
    if (m_cyber && m_cyber->isCompleted() && !m_cyber->isUpgrading())
    {
        m_cyber->upgrade(BWAPI::UpgradeTypes::Singularity_Charge);
    }
}
void BaseManager::upgradeForge(BWAPI::UpgradeType type)
{
    for (auto forge : m_forges)
    {
        if (forge && forge->isCompleted() && !forge->isUpgrading())
        {
            forge->upgrade(type);
            break;
        }
    }
}
void BaseManager::upgradeObserver(BWAPI::UpgradeType type)
{
    if (m_obs && m_obs->isCompleted() && !m_obs->isUpgrading())
    {
        m_obs->upgrade(type);
    }
}

BWAPI::Unit BaseManager::removeProbe()
{

    Base& selectedBase = m_bases[0];
    size_t mostProbes = selectedBase.getProbeCount();
    for (auto& base : m_bases)
    {
        if (base.getProbeCount() > mostProbes)
        {
            selectedBase = base;
            mostProbes = selectedBase.getProbeCount();
        }
    }

    if (selectedBase.getProbeCount() <= 0)
    {
        return nullptr;
    }
    else
    {
        return selectedBase.removeProbe();
    }

}

void BaseManager::removeProbe(BWAPI::Unit probe)
{
    for (auto& base : m_bases)
    {
        base.removeProbe(probe);
    }

    m_buildingProbes.erase(probe);
}

void BaseManager::addProbe(BWAPI::Unit unit)
{
    if (unit->getType() != BWAPI::UnitTypes::Protoss_Probe)
    {
        return;
    }
    Base& selectedBase = m_bases[0];
    int leastProbes = selectedBase.getProbeCount();
    for (auto& base : m_bases)
    {
        if (base.hasNexus())
        {
            if (base.getProbeCount() < leastProbes)
            {
                selectedBase = base;
                leastProbes = base.getProbeCount();
            }
        }
        
    }
    if (selectedBase.hasNexus())
    {
        selectedBase.assignProbe(unit);
    }
}

void BaseManager::incObserverCount()
{
    m_observerCount++;
}
void BaseManager::decObserverCount()
{
    m_observerCount--;
}