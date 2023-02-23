/*
    Authors:
        Keegan Churchill-Myers 201510260
*/
#include "BaseManager.h"
#include "StarterBot.h"
#include "Tools.h"
#include "MapTools.h"
#include <iostream>
#include <fstream>


StarterBot::StarterBot()
{
    m_isFirstBase = true;
}
BWAPI::Position tempPos;
// Called when the bot starts!
void StarterBot::onStart()
{
    // Set our BWAPI options here    
	BWAPI::Broodwar->setLocalSpeed(15);
    BWAPI::Broodwar->setFrameSkip(0);
    
    // Enable the flag that tells BWAPI top let users enter input while bot plays
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

    // Call MapTools OnStart
    m_mapTools.onStart();

    //Initialize Multi used vars
    const auto& startingNexus = Tools::GetDepot();

    //MapBases
    
    computeBases();
    m_baseOccupier = std::vector<int>(m_mapBases.size(), 0);
    size_t i = 0;
    for (auto& base : m_mapBases)
    {
        if (Tools::CalcDistanceSquared(startingNexus->getPosition(), base.getPosition()) < Tools::criticalValueSq)
        {
            m_baseOccupier[i] = 1;
        }
        i++;
    }

    //Macro
    m_baseManager = BaseManager(Tools::GetDepot(), &m_mapBases, &m_baseOccupier, &m_mapTools);

}

// Called whenever the game ends and tells you if you won or not
void StarterBot::onEnd(bool isWinner)
{
    
    

}

//Sets starter bots knowledge of potential bases
void StarterBot::computeBases()
{
    //Reliable constant for determining what distance consitutes a mineral cluster
    
    std::vector<BWAPI::Unitset> resourceClusters;
    bool unitInserted = false;
    for (auto& unit : BWAPI::Broodwar->getStaticMinerals())
    {
        if (unit->getResources() > 300)
        {
            unitInserted = false;
            if (resourceClusters.size() == 0)
            {
                BWAPI::Unitset potentialMineralCluster;
                potentialMineralCluster.insert(unit);
                resourceClusters.push_back(potentialMineralCluster);
            }
            else
            {
                //loop through all previously clumped minerals to see if this mineral is within range
                //of the clump, else mark it as a new mineral cluster. 
                for (auto& mineralCluster : resourceClusters)
                {
                    if (unitInserted) { break; }
                    if (Tools::CalcDistanceSquared(unit->getPosition(), mineralCluster.getPosition()) < Tools::criticalValueSq)
                    {
                        mineralCluster.insert(unit);
                        unitInserted = true;
                        break;
                    }
                }
                if (!unitInserted)
                {
                    BWAPI::Unitset potentialMineralCluster;
                    potentialMineralCluster.insert(unit);
                    resourceClusters.push_back(potentialMineralCluster);
                    unitInserted = true;
                }
            }
        }
    }
    for (auto& unit : BWAPI::Broodwar->getStaticGeysers())
    {
        unitInserted = false;
        //loop through all previously clumped minerals to see if this geyser  is within range
        //of the clump, else mark it as a new mineral cluster. 
        for (auto& resourceCluster : resourceClusters)
        {
            if (unitInserted) { break; }
            if (Tools::CalcDistanceSquared(unit->getPosition(), resourceCluster.getPosition()) < Tools::criticalValueSq)
            {
                resourceCluster.insert(unit);
                unitInserted = true;
                break;
            }
        }
    }
    //Delete scattered resources that act as map features (outliers)

    for (auto& cluster : resourceClusters)
    {

        for (auto it = cluster.cbegin(), next_it = it; it != cluster.cend(); it = next_it)
        {
            ++next_it;
            if ((*it)->getInitialResources() < 300)
            {
                cluster.erase(it);
            }
        }
    }
    for (auto it = resourceClusters.begin(); it != resourceClusters.end();) //removed & from auto it check behaviour
    {
        if (it->size() < 4)
        {
            it = resourceClusters.erase(it);
        }  
        else
        {
            ++it;
        }
    }

    m_mapBases = resourceClusters;

    /*size_t i = 0;
    for (auto& resourceCluster : resourceClusters)
    {
        std::cout << "Group " << i << ": ";
        for (auto& resource : resourceCluster)
        {
            std::cout << resource->getID() << ", ";

        }
        i++;
        std::cout << std::endl;
        
    }
    std::cout << resourceClusters.size() << std::endl;*/
    
    
}

// Called on each frame of the game
void StarterBot::onFrame()
{
    // Update our MapTools information
    m_mapTools.onFrame();

    //Macro
    m_baseManager.run();

    // Draw unit health bars, which brood war unfortunately does not do
    Tools::DrawUnitHealthBars();

    // Draw some relevent information to the screen to help us debug the bot
    drawDebugInformation();


}

// Called whenever a unit is destroyed, with a pointer to the unit
void StarterBot::onUnitDestroy(BWAPI::Unit unit)
{
    if (BWAPI::Broodwar->self() == unit->getPlayer()) 
    { 
        if (unit->getType().isWorker() || unit->getType().isBuilding()) 
        {
            m_baseManager.baseUnitDestroyed(unit);
        }
        if (unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
        {
            m_baseManager.decObserverCount();
        }
    }
    else
    {
        if (BWAPI::Broodwar->enemy() == unit->getPlayer())
        {
            if (unit->getType().isBuilding())
            {
                size_t i = 0;
                for (auto& mapBase : m_mapBases)
                {
                    if (mapBase.getPosition().getApproxDistance(unit->getPosition()) < Tools::critcalValue)
                    {
                        const auto& units = BWAPI::Broodwar->getUnitsInRadius(mapBase.getPosition(), Tools::critcalValue, BWAPI::Filter::IsBuilding && BWAPI::Filter::IsEnemy);
                        if (units.size() < 1 && m_baseOccupier[i] < 0)
                        {
                            m_baseOccupier[i] = 0;
                        }
                        break;
                    }
                    i++;
                }
            }
        }
    }
}

// Called whenever a unit is morphed, with a pointer to the unit
// Zerg units morph when they turn into other units
void StarterBot::onUnitMorph(BWAPI::Unit unit)
{
	
}

// Called whenever a text is sent to the game by a user
void StarterBot::onSendText(std::string text) 
{ 
    if (text == "/map")
    {
        m_mapTools.toggleDraw();
    }
}

// Called whenever a unit is created, with a pointer to the destroyed unit
// Units are created in buildings like barracks before they are visible, 
// so this will trigger when you issue the build command for most units
void StarterBot::onUnitCreate(BWAPI::Unit unit)
{ 
    if (!m_isFirstBase && unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self())
    {
        m_baseManager.decTiedUpResourcesAndManageCreatedBuildings(unit);
    }

    if (BWAPI::Broodwar->enemy() == unit->getPlayer())
    {
        if (unit->getType().isBuilding())
        {
            size_t i = 0;
            for (auto& mapBase : m_mapBases)
            {
                if (mapBase.getPosition().getApproxDistance(unit->getPosition()) < Tools::critcalValue)
                {
                    if (m_baseOccupier[i] == 0)
                    {
                        m_baseOccupier[i] = -2;
                        break;
                    }
                }
                i++;
            }
        }
    }

}

// Called whenever a unit finished construction, with a pointer to the unit
void StarterBot::onUnitComplete(BWAPI::Unit unit)
{
	if (unit->getPlayer() == BWAPI::Broodwar->self() && unit->getType().isBuilding())
    {
        if (unit->getType() == BWAPI::UnitTypes::Protoss_Nexus)
        {
            if (m_isFirstBase)
            {
                m_isFirstBase = false;
            }
            else
            {
                size_t i = 0;
                size_t closestBase = 0;
                int leastDistance = -1;
                for (auto& mapBase : m_mapBases)
                {
                    if (leastDistance < 0)
                    {
                        closestBase = 0;
                        leastDistance = unit->getPosition().getApproxDistance(mapBase.getPosition());
                    }
                    else if (unit->getPosition().getApproxDistance(mapBase.getPosition()) < leastDistance)
                    {
                        leastDistance = unit->getPosition().getApproxDistance(mapBase.getPosition());
                        closestBase = i;
                    }
                    i++;
                }
                m_baseOccupier[closestBase] = 2;
                //m_baseManager.addBase(Base(unit));
            }
        }
        else
        {
            m_baseManager.buildingComplete(unit->getType());
        }
    }

    if (BWAPI::Broodwar->self() == unit->getPlayer())
    {
        if (unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
        {
            m_baseManager.incObserverCount();
        }
    }
}

// Called whenever a unit appears, with a pointer to the destroyed unit
// This is usually triggered when units appear from fog of war and become visible
void StarterBot::onUnitShow(BWAPI::Unit unit)
{ 
    if (BWAPI::Broodwar->self() == unit->getPlayer())
    {
    }
    else
    {
        if (BWAPI::Broodwar->enemy() == unit->getPlayer())
        {
            if (unit->getType().isBuilding())
            {
                size_t i = 0;
                for (auto& mapBase : m_mapBases)
                {
                    if (mapBase.getPosition().getApproxDistance(unit->getPosition()) < Tools::critcalValue)
                    {
                        if (m_baseOccupier[i] == 0)
                        {
                            m_baseOccupier[i] = -2;
                            break;
                        }
                    }
                    i++;
                }
            }
        }
    }
}

// Called whenever a unit gets hidden, with a pointer to the destroyed unit
// This is usually triggered when units enter the fog of war and are no longer visible
void StarterBot::onUnitHide(BWAPI::Unit unit)
{ 
	
}

// Called whenever a unit switches player control
// This usually happens when a dark archon takes control of a unit
void StarterBot::onUnitRenegade(BWAPI::Unit unit)
{ 
    if (unit->getType().isRefinery() && unit->getPlayer() == BWAPI::Broodwar->self())
    {
        m_baseManager.decTiedUpResourcesAndManageCreatedBuildings(unit);
    }
}

// Draw some relevent information to the screen to help us debug the bot
void StarterBot::drawDebugInformation()
{
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 10), "Hello, World!\n");
    Tools::DrawUnitCommands();
    Tools::DrawUnitBoundingBoxes();
    //Tools::DrawUnitIDs();
    Tools::DrawUnitCircle(Tools::GetDepot(), 125);
    Tools::DrawUnitCircle(Tools::GetDepot(), 500);
    for (auto& mineralCluster : m_mapBases)
    {
        BWAPI::Broodwar->drawCircleMap(mineralCluster.getPosition(), (int)Tools::critcalValue, BWAPI::Colors::Green);
    }
}

