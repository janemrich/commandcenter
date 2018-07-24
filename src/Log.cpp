#include "Log.h"

#include <iostream>
#include <fstream>
#include <sc2api/sc2_score.h>
#include "CCBot.h"

Log::Log(CCBot &bot, int dna)
        : m_bot(bot), dna(dna) {

}

using namespace std;

void Log::onGameStart() {
    ofstream ofs = openFile("log");
    ofs << m_bot.getStrategyString() << "\n";
}

void Log::create(std::string unit) {
    ofstream ofs = openFile("csv");

    //write to log
    sc2::ObservationInterface *obs = const_cast<sc2::ObservationInterface *>(m_bot.Observation());

    ofs << (int) (m_bot.GetCurrentFrame() / 22.4)
        << ","
        << unit
        << ","
        << m_bot.GetCurrentSupply()
        << ","
        << m_bot.GetMaxSupply()
        << ","
        << m_bot.GetMinerals()
        << ","
        << obs->GetFoodWorkers()
        << ","
        << obs->GetArmyCount()
        << endl;
}

void Log::onUpdate() {
    int currentTime = (int) (m_bot.GetCurrentFrame()/22.4);
    if (currentTime > timestamp) {
        timestamp = currentTime;
        create("");
    }
}

std::ofstream Log::openFile(std::string ending) {
    //filename
    std::ostringstream oss;
    oss << "/home/jan/Documents/Starcraft/Log/" << m_bot.getGeneration() << "/" << dna << "." << ending;
    std::string filename = oss.str();

    //open file
    std::ofstream ofs;
    ofs.open(filename.c_str(), std::ios_base::app);
    return ofs;
}

void Log::onGameEnd() {
    ofstream ofs = openFile("log");

    //write to log
    ofs << m_bot.GetCurrentFrame() << "\n";
}
