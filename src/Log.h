#pragma once

class CCBot;

class Log {
	CCBot &  m_bot;
	int dna;

public:
	Log(CCBot & bot, int dna);

	void onGameEnd();
};

