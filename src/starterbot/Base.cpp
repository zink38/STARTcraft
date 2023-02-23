#include "Base.h"
#include "MapTools.h"
#include "Tools.h"

Base::Base()
{
}

Base::Base(BWAPI::Unit nexus)
{
    lostProbes = BWAPI::Unitset();
    m_retreatPosition = BWAPI::Position();
    m_pylons = BWAPI::Unitset();
    m_gateways = BWAPI::Unitset();
    m_gasProbes = std::vector<BWAPI::Unitset>();
    m_mineralProbes = std::vector<BWAPI::Unitset>();
    m_nexus = nexus;
    m_position = nexus->getTilePosition();
    m_gotBuiltProbe = true;

    setUp();
}

void Base::setUp()
{
    m_minerals = m_nexus->getUnitsInRadius(300, BWAPI::Filter::IsMineralField);
    m_gases = m_nexus->getUnitsInRadius(300, BWAPI::Filter::IsResourceContainer && !BWAPI::Filter::IsMineralField);
    for (size_t i = 0; i < m_minerals.size(); i++)
    {
        m_mineralProbes.push_back(BWAPI::Unitset());
    }
    for (size_t i = 0; i < m_gases.size(); i++)
    {
        m_gasProbes.push_back(BWAPI::Unitset());
    }
    m_maxProbes = 2 * m_minerals.size() + 3 * m_gases.size();

}

//Managing probes and sorting them , grouping them to a specified mineral field
void Base::assignProbes(BWAPI::Unitset probes)
{
    for (auto& probe : probes)
    {
        assignProbe(probe);
    }
}
void Base::assignProbe(BWAPI::Unit probe)
{
    for (size_t i = 0; true ; i++)
    {
        size_t gasIndex = 0;
        for (auto& gas : m_gases)
        {
            if (gas->getType().isRefinery() && gas->getPlayer() == BWAPI::Broodwar->self() && gas->isCompleted())
            {

                if (m_gasProbes[gasIndex].size() == i && i < 3 )
                {
                    m_gasProbes[gasIndex].insert(probe);
                    return;
                }
            }
            gasIndex++;
        }

        for (auto& probes : m_mineralProbes)
        {
            if (probes.size() == i)
            {
                probes.insert(probe);
                return;
            }
        }
    } 
}


BWAPI::Unit Base::getProbe()
{
    BWAPI::Unit selectedProbe = BWAPI::Unit();
    for (size_t i = 3; i > 0; i--)
    {
        for (auto& probes : m_mineralProbes)
        {
            if (i == 3)
            {
                if (probes.size() >= i)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isCarryingMinerals())
                        {
                            selectedProbe = probe;
                            //std::cout << "Erasing L1 I1 " << probe->getID() << std::endl;
                            return selectedProbe;
                        }
                    }
                }
            }
            else
            {
                if (probes.size() == i)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isCarryingMinerals())
                        {
                            selectedProbe = probe;
                            //std::cout << "Erasing L1 I2 " << probe->getID() << std::endl;
                            return selectedProbe;
                        }
                    }
                }
            }
        }
    }

    for (size_t i = 3; i > 0; i--)
    {
        for (auto& probes : m_mineralProbes)
        {
            if (i == 3)
            {
                if (probes.size() >= i)
                {
                    for (auto probe : probes)
                    {

                        selectedProbe = probe;
                        //std::cout << "Erasing L2 I1 " << probe->getID() << std::endl;
                        return selectedProbe;

                    }
                }
            }
            else
            {
                if (probes.size() == i)
                {
                    for (auto probe : probes)
                    {

                        selectedProbe = probe;
                        //std::cout << "Erasing L2 I2 " << probe->getID() << std::endl;
                        return selectedProbe;

                    }
                }
            }
        }
    }

    for (size_t i = 3; i > 0; i--)
    {
        for (auto& probes : m_gasProbes)
        {
            if (i == 3)
            {
                if (probes.size() >= i)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isCarryingGas())
                        {
                            selectedProbe = probe;
                            return selectedProbe;
                        }
                    }
                }
            }
            else
            {
                if (probes.size() == i)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isCarryingGas())
                        {
                            selectedProbe = probe;
                            return selectedProbe;
                        }
                    }
                }
            }
        }
    }

    for (size_t i = 3; i > 0; i--)
    {
        for (auto& probes : m_gasProbes)
        {
            if (i == 3)
            {
                if (probes.size() >= i)
                {
                    for (auto probe : probes)
                    {

                        selectedProbe = probe;
                        return selectedProbe;

                    }
                }
            }
            else
            {
                if (probes.size() == i)
                {
                    for (auto probe : probes)
                    {

                        selectedProbe = probe;
                        return selectedProbe;

                    }
                }
            }
        }
    }

    //std::cout << "Returning Null probe" << std::endl;
    return selectedProbe;
}

//Check if the base has probes before calling this function
BWAPI::Unit Base::removeProbe()
{
    BWAPI::Unit selectedProbe = BWAPI::Unit();
    for (size_t i = 3; i > 0; i--)
    {
        for (auto& probes : m_mineralProbes)
        {
            if (i == 3)
            {
                if (probes.size() >= i)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isCarryingMinerals())
                        {
                            selectedProbe = probe;
                            probes.erase(probe);
                            selectedProbe->stop();
                            //std::cout << "Erasing L1 I1 " << probe->getID() << std::endl;
                            return selectedProbe;
                        }
                    }
                }
            }
            else
            {
                if (probes.size() == i)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isCarryingMinerals())
                        {
                            selectedProbe = probe;
                            probes.erase(probe);
                            selectedProbe->stop();
                            //std::cout << "Erasing L1 I2 " << probe->getID() << std::endl;
                            return selectedProbe;
                        }
                    }
                }
            }
        }
    }

    for (size_t i = 3; i > 0; i--)
    {
        for (auto& probes : m_mineralProbes)
        {
            if (i == 3)
            {
                if (probes.size() >= i)
                {
                    for (auto probe : probes)
                    {

                        selectedProbe = probe;
                        probes.erase(probe);
                        //std::cout << "Erasing L2 I1 " << probe->getID() << std::endl;
                        return selectedProbe;

                    }
                }
            }
            else
            {
                if (probes.size() == i)
                {
                    for (auto probe : probes)
                    {

                        selectedProbe = probe;
                        probes.erase(probe);
                        selectedProbe->stop();
                        //std::cout << "Erasing L2 I2 " << probe->getID() << std::endl;
                        return selectedProbe;

                    }
                }
            }
        }
    }

    for (size_t i = 3; i > 0; i--)
    {
        for (auto& probes : m_gasProbes)
        {
            if (i == 3)
            {
                if (probes.size() >= i)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isCarryingGas())
                        {
                            selectedProbe = probe;
                            probes.erase(probe);
                            selectedProbe->stop();
                            //std::cout << "Erasing L1 I1 " << probe->getID() << std::endl;
                            return selectedProbe;
                        }
                    }
                }
            }
            else
            {
                if (probes.size() == i)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isCarryingGas())
                        {
                            selectedProbe = probe;
                            probes.erase(probe);
                            selectedProbe->stop();
                            //std::cout << "Erasing L1 I2 " << probe->getID() << std::endl;
                            return selectedProbe;
                        }
                    }
                }
            }
        }
    }

    for (size_t i = 3; i > 0; i--)
    {
        for (auto& probes : m_gasProbes)
        {
            if (i == 3)
            {
                if (probes.size() >= i)
                {
                    for (auto probe : probes)
                    {

                        selectedProbe = probe;
                        probes.erase(probe);
                        selectedProbe->stop();
                        //std::cout << "Erasing L2 I1 " << probe->getID() << std::endl;
                        return selectedProbe;

                    }
                }
            }
            else
            {
                if (probes.size() == i)
                {
                    for (auto probe : probes)
                    {

                        selectedProbe = probe;
                        probes.erase(probe);
                        selectedProbe->stop();
                        //std::cout << "Erasing L2 I2 " << probe->getID() << std::endl;
                        return selectedProbe;

                    }
                }
            }
        }
    }
    //std::cout << "Returning Null probe" << std::endl;
    return selectedProbe;
}

void Base::removeProbe(BWAPI::Unit probe)
{
    for (auto& probes : m_mineralProbes)
    {
        probes.erase(probe);
    }
    for (auto& probes : m_gasProbes)
    {
        probes.erase(probe);
    }
}

void Base::addPylon(BWAPI::Unit pylon)
{
    
    m_pylons.insert(pylon);
}

void Base::addGateway(BWAPI::Unit gateway)
{
    if (m_gateways.size() == 0)
    {
        m_retreatPosition = gateway->getPosition();
    }
    m_gateways.insert(gateway);
}

void Base::addCannon(BWAPI::Unit cannon)
{
    m_gateways.insert(cannon);
}

void Base::removePylon(BWAPI::Unit pylon)
{
    m_pylons.erase(pylon);
}

void Base::removeGateway(BWAPI::Unit gateway)
{
    m_pylons.insert(gateway);
}

void Base::removeCannon(BWAPI::Unit cannon)
{
    m_pylons.insert(cannon);
}

BWAPI::TilePosition Base::getPosition()
{
    return m_position;
}

bool Base::hasMaxProbes()
{
    bool result = false;
    if (getProbeCount() >= m_maxProbes)
    {
        result = true;
    }
    return result;
}

bool Base::hasCompletedPylon()
{
    for (auto pylon : m_pylons)
    {
        if (pylon->isCompleted())
        {
            return true;
        }
    }
    return false;
}

const bool Base::canBuildAssimilator()
{
    for (auto& gas : m_gases)
    {
        if (!gas->getType().isRefinery())
        {
            return true;
        }
    }
    return false;
}

const bool Base::canMineGas()
{
    for (auto& gas : m_gases)
    {
        if (gas->getType().isRefinery() && gas->getPlayer() == BWAPI::Broodwar->self())
        {
            return true;
        }
    }
    return false;
}

int Base::getProbeCount()
{
    int total = 0;
    for (auto& probes : m_mineralProbes)
    {
        total += probes.size();
    }
    for (auto& probes : m_gasProbes)
    {
        total += probes.size();
    }
    
    return total;
}

size_t Base::getGatewayCount()
{
    return m_gateways.size();
}

size_t Base::getCannonCount()
{
    return m_cannons.size();
}

size_t Base::getPylonCount()
{
    return m_pylons.size();
}

size_t Base::getGasCount()
{
    return m_gases.size();
}

bool Base::hasNexus()
{
    if(!m_nexus || !m_nexus->exists())
    {
        return false;
    }
    else
    {
        return true;
    }
    
}

bool Base::hasGas()
{
    return (m_gases.size() != 0);
}

BWAPI::Unit Base::getNexus()
{
    return m_nexus;
}

BWAPI::Unitset Base::getResources()
{
    auto result = m_minerals;
    for (auto gas : m_gases)
    {
        result.insert(gas);
    }
    return result;
}

const BWAPI::Unitset& Base::getGases()
{
    return m_gases;
}

//Frame to fram running of a base
void Base::runBase()
{
    //Draw for debugging
   /* for (auto& gas : m_gases)
    {
        Tools::DrawUnitCircle(gas, 30);
    }
    for (auto& mineral : m_minerals)
    {
        Tools::DrawUnitCircle(mineral, 15);
    }
    
    size_t j = 0;
    for (const auto& probeSection : m_mineralProbes)
    {
        if (probeSection.size() > 0)
        {
            size_t i = 0;
            for (const auto& probe : probeSection)
            {
                i++;
                Tools::DrawUnitCircle(probe, 5);
            }
        }
        j++;
    }*/
        
    if (!m_nexus->exists())
    {
        for (size_t i = 0; i < getProbeCount(); i++)
        {
            lostProbes.insert(removeProbe());
        }
        
        for (auto probe : lostProbes)
        {
            // if there's no valid unit, ignore the command
            if (probe)
            {
                if (!(probe->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount()))
                {
                    if (probe->getLastCommand().getTargetPosition() != m_retreatPosition)
                    {
                        probe->rightClick(m_retreatPosition);
                    }
                }
            }
        }        
    }
    else {
        //tell workers to mine something if they are out of minerals
        bool haveminerals = false;
        for (auto& mineral : m_minerals)
        {
            if (mineral->exists())
            {
                haveminerals = true;
            }
        }
        if (haveminerals)
        {
            lockMineralWorkersToMinerals();
        }
        else
        {
            for (auto& probes : m_mineralProbes)
            {
                BWAPI::Unit mineral = BWAPI::Broodwar->getClosestUnit(probes.getPosition(), BWAPI::Filter::IsMineralField);
                if (mineral)
                {
                    for (auto probe : probes)
                    {
                        if (!probe->isGatheringMinerals())
                        {
                            Tools::SmartRightClick(probe, mineral);
                        }

                    }
                }  
            }
        }
        //Same for gases
        lockGasWorkersToGas();
        //Add probes to the base as they finish building
        if (!m_nexus->isTraining() && !m_gotBuiltProbe)
        {
            getBuiltProbe();
        }
    }
    
}

//Keeps probes mining the appropiat mineral field
void Base::lockMineralWorkersToMinerals()
{
    size_t index = 0;
    //Loop through the base's member minerals 
    for (auto& mineral : m_minerals)
    {

        auto& probes = m_mineralProbes[index];

        for (auto& probe : probes)
        {
            if (probe->isIdle())
            {
                probe->gather(mineral);
            }
            else if (!probe->isCarryingMinerals() && probe->getLastCommand().getType() == BWAPI::UnitCommandTypes::Gather)
            {
                if (probe->getTarget() != mineral)
                {
                    probe->gather(mineral);
                }
            }
        }
        index++;
    } 
}

void Base::lockGasWorkersToGas()
{
    size_t index = 0;
    for (auto& gas : m_gases)
    {

        auto& probes = m_gasProbes[index];
        for (auto& probe : probes)
        {
            if (probe->isIdle())
            {
                probe->gather(gas);
            }
            else if (!probe->isCarryingGas() && probe->getLastCommand().getType() == BWAPI::UnitCommandTypes::Gather)
            {
                if (probe->getTarget() != gas)
                {
                    probe->gather(gas);
                }
            }
        }
        index++;
    }
}

void Base::sendWorkersToGases()
{
    size_t index = 0;
    for (auto& gas : m_gases)
    {
        auto& probes = m_gasProbes[index];
        //std::cout << "Before For loop" << std::endl;
        for (auto& probe : probes)
        {
            //std::cout << "Looping through gas probes" << std::endl;
            if (!probe)
            {
                continue;
            }
         
            Tools::SmartRightClick(probe, gas);
            
        }
        index++;
    }
    //std::cout << "Finished commanding gas probes" << std::endl;
}

//selects and adds a newly built probe
void Base::getBuiltProbe()
{
    BWAPI::Unit probe = m_nexus->getClosestUnit(BWAPI::Filter::IsOwned && BWAPI::Filter::IsIdle && BWAPI::Filter::IsWorker, 65);
    if (probe)
    {
        assignProbe(probe);
        m_gotBuiltProbe = true;
    }
}

//Train more workers
void Base::trainAdditionalWorkers()
{
    if (m_nexus && !m_nexus->isTraining())
    {
        if (m_nexus->train(BWAPI::UnitTypes::Protoss_Probe))
        {
            m_gotBuiltProbe = false;
        }
    }
}





