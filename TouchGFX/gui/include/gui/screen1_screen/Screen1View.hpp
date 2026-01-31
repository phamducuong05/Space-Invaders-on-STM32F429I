#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <touchgfx/widgets/Image.hpp>
#include "BitmapDatabase.hpp"


static const int MAX_ALIENS = 30;

static const int MAX_BULLETS = 20;

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    void updateShipPosition(uint8_t dir);
    bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
protected:
    touchgfx::Image bulletPool[MAX_BULLETS];
    touchgfx::Image alienPool[MAX_ALIENS];
    touchgfx::Image* heartIcons[3];

	int tickCounter;
	int alienSpawnCounter;
	unsigned int randomSeed;
	int getRandomNumber(int min, int max)
	{
		randomSeed = (randomSeed * 1103515245 + 12345) & 0x7FFFFFFF;
		return min + (randomSeed % (max - min + 1));
	}
	int playerHealth;
	int invulnerabilityCounter;
	void updateHealthUI();
	int currentLevel;
	int enemiesDefeated;
	int enemiesToNextLevel;

	int currentAlienSpeed;
	int spawnSpeedRange;

	void nextLevel();
	Unicode::UnicodeChar levelTextBuffer[10];
	int nextSpawnTime;
	int levelUpTimer;
	int gameWinTimer;
	uint16_t currentAlienBitmapId;

};

#endif // SCREEN1VIEW_HPP
