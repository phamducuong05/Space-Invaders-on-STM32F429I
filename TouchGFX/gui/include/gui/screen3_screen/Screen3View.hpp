#ifndef SCREEN3VIEW_HPP
#define SCREEN3VIEW_HPP

#include <gui_generated/screen3_screen/Screen3ViewBase.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>

static const int MAX_ALIENS_3 = 30;
static const int MAX_ENEMY_BULLETS = 20;
static const int MAX_BULLETS_3 = 30;

class Screen3View : public Screen3ViewBase
{
public:
    Screen3View();
    virtual ~Screen3View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    void takeDamage();
	void handleGameOver();
	void updateHealthUI();
	int getRandomNumber(int min, int max);
	bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
	void updateShipPosition(uint8_t dir);

protected:
	unsigned int randomSeed;
	int tickCounter;

	// Biến trạng thái game
	int currentScore;
	int playerHealth;
	bool isGameOver;
	int spawnTimer;
	int invulnerabilityCounter;
	int gameOverTickCounter;

	// Buffer hiển thị chữ
	Unicode::UnicodeChar scoreBuffer[20];
	Unicode::UnicodeChar yourScoreBuffer[20];

	// Mảng quản lý đối tượng
	touchgfx::Image alienPool[MAX_ALIENS_3];
	int alienSpeedX[MAX_ALIENS_3]; // Tốc độ ngang của quái
	int alienReloadTimer[MAX_ALIENS_3];

	touchgfx::Image enemyBulletPool[MAX_ENEMY_BULLETS];
	touchgfx::Image bulletPool[MAX_BULLETS_3];

	// Mảng trái tim (nếu bạn có dùng hiển thị máu)
	touchgfx::Image* heartIcons[3];
};

#endif // SCREEN3VIEW_HPP
