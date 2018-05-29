#include "Log.h"

#include <iostream>
#include <fstream>
#include "CCBot.h"

Log::Log(CCBot & bot, int dna)
    : m_bot(bot), dna(dna)
{

}

void Log::onGameStart() {
	//filename
	std::ostringstream oss;
	oss << "/home/jan/Documents/Starcraft/Log/" << m_bot.getGeneration() << "/" << dna << ".log";
	std::string filename = oss.str();

	//print strategy
	std::ofstream logfile(filename);
	logfile << m_bot.getStrategyString() << "\n";
}
void Log::onGameEnd() {
	//filename
	std::ostringstream oss;
	oss << "/home/jan/Documents/Starcraft/Log/" << m_bot.getGeneration() << "/" << dna << ".log";
	std::string filename = oss.str();

	//open file
	std::ofstream ofs;
	ofs.open(filename.c_str(), std::ios_base::app);

	//write to log
	ofs << m_bot.GetCurrentFrame() << "\n";
    std::cout << "Game ended after: " << m_bot.GetCurrentFrame()<< " loops " << std::endl
    		<< (int) m_bot.UnitInfo().getUnitTypeCount(Players::Self, UnitType::GetUnitTypeFromName("TERRAN_MARINE", m_bot), true)<< std::to_string(dna)<< dna;
}
