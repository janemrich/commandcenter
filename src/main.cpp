#include "Common.h"
#include "CCBot.h"
#include "JSONTools.h"
#include "Util.h"
#include <stdio.h>
#include <fstream>
#include <sstream>

#ifdef SC2API

#include "sc2utils/sc2_manage_process.h"
#include "sc2api/sc2_api.h"

using namespace std;

int main(int argc, char* argv[])
{

    bool CONNECT = true;

    sc2::Coordinator coordinator;
    std::cout << "load settings";
    if (!coordinator.LoadSettings(3, argv))
    {
        std::cout << "Unable to find or parse settings." << std::endl;
        return 3;
    }
    /*if (argc < 4) {
        std::cout << "Unable to find strategy." << std::endl;
        return 1;
    }*/

    /*
    std::string config = JSONTools::ReadFile("/home/jan/Documents/Starcraft/commandcenter/bin/BotConfig.txt");
    if (config.length() == 0)
    {
        std::cerr << "Config file could not be found, and is required for starting the bot\n";
        std::cerr << "Please read the instructions and try again\n";
        exit(-1);
    }

    std::ifstream file("/home/jan/Documents/Starcraft/commandcenter/bin/BotConfig.txt");
    json j;
    file >> j;
    */
    /*if (parsingFailed)
    {
        std::cerr << "Config file could not be parsed, and is required for starting the bot\n";
        std::cerr << "Please read the instructions and try again\n";
        exit(-1);
    }*/

    std::string botRaceString = "Terran";
    std::string enemyRaceString = "Random";
    std::string mapString = "BelShirVestigeLE.SC2Map";
    int stepSize = 10;
    sc2::Difficulty enemyDifficulty = sc2::Difficulty::VeryEasy;
    /*
    if (j.count("SC2API") && j["SC2API"].is_object())
    {
        const json & info = j["SC2API"];
        JSONTools::ReadString("BotRace", info, botRaceString);
        JSONTools::ReadString("EnemyRace", info, enemyRaceString);
        JSONTools::ReadString("MapFile", info, mapString);
        JSONTools::ReadInt("StepSize", info, stepSize);
        JSONTools::ReadInt("EnemyDifficulty", info, enemyDifficulty);
    }
    else
    {
        std::cerr << "Config file has no 'Game Info' object, required for starting the bot\n";
        std::cerr << "Please read the instructions and try again\n";
        exit(-1);
    }
    */
    // Add the custom bot, it will control the players.

	std::istringstream ss1(argv[4]);
	int generation;
	if (!(ss1 >> generation))
		cerr << "Invalid number " << argv[4] << '\n';
	istringstream ss2(argv[5]);
	int number;
	if (!(ss2 >> number))
		cerr << "Invalid number " << argv[5] << '\n';
    istringstream ss3(argv[6]);
    int thread;
    if (!(ss3 >> thread))
        cerr << "Invalid number " << argv[6] << '\n';
    istringstream ss4(argv[7]);
    string location;
    if (!(ss4 >> location))
        cerr << "Invalid location " << argv[7] << '\n';

	CCBot bot(generation, number, &coordinator, location);
	string strategy = argv[3];
	bot.setStrategy(strategy);

    
    // WARNING: Bot logic has not been thorougly tested on step sizes > 1
    //          Setting this = N means the bot's onFrame gets called once every N frames
    //          The bot may crash or do unexpected things if its logic is not called every frame
    coordinator.SetStepSize(stepSize);
    coordinator.SetRealtime(false);
    //coordinator.SetMultithreaded(true);

    coordinator.SetParticipants({
        sc2::CreateParticipant(Util::GetRaceFromString(botRaceString), &bot),
        sc2::CreateComputer(Util::GetRaceFromString(enemyRaceString), enemyDifficulty)
    });

    // Start the game.
    cerr << "connect / launch";
    if (CONNECT) {
        coordinator.Connect(8167 + thread);
    } else {
        coordinator.LaunchStarcraft();
    }

    cerr << "start game";

    coordinator.StartGame(mapString);

    cout << "game started\n";

    // Step forward the game simulation.
    int i = 0;
    while (true) 
    {
        coordinator.Update();
        printf("%d\n", i++);
    }

    return 0;
}

#else

#include <BWAPI/Client.h>

void UAlbertaBot_BWAPIReconnect() 
{
    while(!BWAPI::BWAPIClient.connect())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 }); 
    }
}

void UAlbertaBot_PlayGame()
{
    CCBot bot;
    
    // The main game loop, which continues while we are connected to BWAPI and in a game
	while (BWAPI::BWAPIClient.isConnected() && BWAPI::Broodwar->isInGame()) 
    {
        // Handle each of the events that happened on this frame of the game
		for (const BWAPI::Event & e : BWAPI::Broodwar->getEvents()) 
        {
			switch (e.getType()) 
            {
                case BWAPI::EventType::MatchStart:      { bot.OnGameStart();                 break; }
			    case BWAPI::EventType::MatchFrame:      { bot.OnStep();                      break; }
                                                        
                /*case BWAPI::EventType::MatchEnd:        { m->onEnd(e.isWinner());            break; }
			    case BWAPI::EventType::UnitShow:        { m->onUnitShow(e.getUnit());        break; }
			    case BWAPI::EventType::UnitHide:        { m->onUnitHide(e.getUnit());        break; }
			    case BWAPI::EventType::UnitCreate:      { m->onUnitCreate(e.getUnit());      break; }
			    case BWAPI::EventType::UnitMorph:       { m->onUnitMorph(e.getUnit());       break; }
			    case BWAPI::EventType::UnitDestroy:     { m->onUnitDestroy(e.getUnit());     break; }
			    case BWAPI::EventType::UnitRenegade:    { m->onUnitRenegade(e.getUnit());    break; }
			    case BWAPI::EventType::UnitComplete:    { m->onUnitComplete(e.getUnit());    break; }
			    case BWAPI::EventType::SendText:        { m->onSendText(e.getText());        break; }*/
			}
		}

		BWAPI::BWAPIClient.update();
		if (!BWAPI::BWAPIClient.isConnected()) 
        {
			std::cout << "Disconnected\n";
			break;
		}
	}

    std::cout << "Game Over\n";
}

int main(int argc, char * argv[]) 
{
    bool exitIfStarcraftShutdown = true;

    size_t gameCount = 0;
	while (true)
    {
        // if we are not currently connected to BWAPI, try to reconnect
        if (!BWAPI::BWAPIClient.isConnected())
        {
            UAlbertaBot_BWAPIReconnect();
        }

        // if we have connected to BWAPI
        while (BWAPI::BWAPIClient.isConnected())
        {
            // wait for the game to start until the game starts
            std::cout << "Waiting for game start\n";
	        while (BWAPI::BWAPIClient.isConnected() && !BWAPI::Broodwar->isInGame()) 
            {
		        BWAPI::BWAPIClient.update();
	        }

            // Check to see if Starcraft shut down somehow
            if (BWAPI::BroodwarPtr == nullptr)
            {
                break;
            }

            // If we are successfully in a game, call the module to play the game
            if (BWAPI::Broodwar->isInGame())
            {
                std::cout << "Playing game " << gameCount++ << " on map " << BWAPI::Broodwar->mapFileName() << "\n";

                UAlbertaBot_PlayGame();
            }
        }

        if (exitIfStarcraftShutdown && !BWAPI::BWAPIClient.isConnected())
        {
            return 0;
        }
    }

	return 0;
}

#endif
