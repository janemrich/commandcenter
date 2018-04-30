#include "Log.h"

#include <iostream>
#include <fstream>
#include "CCBot.h"

Log::Log(CCBot & bot, int dna)
    : m_bot(bot), dna(dna)
{

}

void Log::onGameEnd() {
	std::ostringstream oss;
	oss << "/home/jan/Documents/Starcraft/Log/" << dna << ".log";
	std::string filename = oss.str();

	std::ofstream file(filename);
	file << m_bot.GetCurrentFrame();
    std::cout << "Game ended after: " << m_bot.GetCurrentFrame()<< " loops " << std::endl
    		<< (int) m_bot.UnitInfo().getUnitTypeCount(Players::Enemy, UnitType::GetUnitTypeFromName("TERRAN_MARINE", m_bot), true)<< std::to_string(dna)<< dna;
}
