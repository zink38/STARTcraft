#pragma once

#include <BWAPI.h>

namespace Tools
{

    //Pixel Radius of a base (mineral cluster) that ststic units must be within to be considered part of that base.
    //Mainly used for scouting and map analysis.
    constexpr size_t critcalValue = 450;
    constexpr size_t criticalValueSq = critcalValue * critcalValue;

    uint32_t LehmerPRNG(uint32_t seed);
    uint32_t LRNG2(uint32_t x, uint32_t y);



    BWAPI::Unit GetClosestUnitTo(BWAPI::Position p, const BWAPI::Unitset& units);
    BWAPI::Unit GetClosestUnitTo(BWAPI::Unit unit, const BWAPI::Unitset& units);

    int CountUnitsOfType(BWAPI::UnitType type, const BWAPI::Unitset& units);

    BWAPI::Unit GetUnitOfType(BWAPI::UnitType type);
    BWAPI::Unit GetDepot();

    bool BuildBuilding(BWAPI::UnitType type);

    void DrawUnitBoundingBoxes();
    void DrawUnitCommands();
    void DrawUnitIDs();
    void DrawUnitCircle(BWAPI::Unit unit, size_t radius);

    void SmartRightClick(BWAPI::Unit unit, BWAPI::Unit target);

    //Fixes issue when using commands that dont have a unit target and also now returns a boolean if the operation was successful 
    //even if the actual unit did not recieve a new right click command
    bool SmartRightClickBool(BWAPI::Unit unit, BWAPI::Unit target);

    int GetTotalSupply(bool inProgress = false);

    void DrawUnitHealthBars();
    void DrawHealthBar(BWAPI::Unit unit, double ratio, BWAPI::Color color, int yOffset);

    size_t CalcDistanceSquared(BWAPI::Position pos1, BWAPI::Position pos2);
    bool HasAssimilator(BWAPI::Unit gas);
    BWAPI::Unit GetRefinery(BWAPI::Unit gas);
}