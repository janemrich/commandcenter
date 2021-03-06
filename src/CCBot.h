#pragma once

#include "Common.h"

#include "MapTools.h"
#include "BaseLocationManager.h"
#include "UnitInfoManager.h"
#include "WorkerManager.h"
#include "BotConfig.h"
#include "GameCommander.h"
#include "BuildingManager.h"
#include "StrategyManager.h"
#include "TechTree.h"
#include "MetaType.h"
#include "Unit.h"
#include "Log.h"


#ifdef SC2API
//class sc2::Coordinator;
class CCBot : public sc2::Agent 
#else
class CCBot
#endif
{
    MapTools                m_map;
    BaseLocationManager     m_bases;
    UnitInfoManager         m_unitInfo;
    WorkerManager           m_workers;
    StrategyManager         m_strategy;
    BotConfig               m_config;
    TechTree                m_techTree;
    GameCommander           m_gameCommander;
    int						dna;
    int						generation;
    sc2::Coordinator *		coordinator;

    std::vector<Unit>       m_allUnits;
    std::vector<CCPosition> m_baseLocations;
    std::string				strategy;
    std::string             location;

    void setUnits();

#ifdef SC2API
    void OnError(const std::vector<sc2::ClientError> & client_errors, 
                 const std::vector<std::string> & protocol_errors = {}) override;
#endif

public:

    Log						log;

    CCBot(int generation, int dna, sc2::Coordinator * coordinator, std::string location);

#ifdef SC2API
    void OnGameStart() override;
    void OnStep() override;
#else
    void OnGameStart();
    void OnStep();
#endif
    void stop();

          BotConfig & Config();
          WorkerManager & Workers();
    const BaseLocationManager & Bases() const;
    const MapTools & Map() const;
    const UnitInfoManager & UnitInfo() const;
    const StrategyManager & Strategy() const;
    const TypeData & Data(const UnitType & type) const;
    const TypeData & Data(const CCUpgrade & type) const;
    const TypeData & Data(const MetaType & type) const;
    const TypeData & Data(const Unit & unit) const;
    CCRace GetPlayerRace(int player) const;
    CCPosition GetStartLocation() const;

    void setStrategy(std::string strategy);
    std::string getStrategyString();
    std::string getLocation();
    int getDNA();
    int getGeneration();
    int GetCurrentFrame() const;
    int GetMinerals() const;
    int GetCurrentSupply() const;
    int GetMaxSupply() const;
    int GetGas() const;
    Unit GetUnit(const CCUnitID & tag) const;
    const std::vector<Unit> & GetUnits() const;
    const std::vector<CCPosition> & GetStartLocations() const;
};
