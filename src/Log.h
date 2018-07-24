#pragma once

#include "Common.h"

class CCBot;

class Log {
	CCBot &  m_bot;
	int dna;
	int timestamp = 0;

public:
	Log(CCBot & bot, int dna);

	void onGameStart();
	void onGameEnd();

    void create(std::string unit);

    void onUpdate();

    std::ofstream openFile(std::string ending);
};

