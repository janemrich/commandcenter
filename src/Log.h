#pragma once

#include "Common.h"

class CCBot;

class Log {
	CCBot &  m_bot;
	int dna;

public:
	Log(CCBot & bot, int dna);

	void onGameStart();
	void onGameEnd();
};

