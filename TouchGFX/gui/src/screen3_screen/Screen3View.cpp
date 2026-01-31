#include <gui/screen3_screen/Screen3View.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>
#include <cstdio>

static const int SCREEN_WIDTH = 240;
static const int SCREEN_HEIGHT = 320;
static const uint16_t ALIEN_BITMAPS[] = {
    BITMAP_ALIEN1_ID,
    BITMAP_ALIEN2_ID,
    BITMAP_ALIEN3_ID,
    BITMAP_ALIEN4_ID,
	BITMAP_ALIEN5_ID,
	BITMAP_ALIEN6_ID
};

Screen3View::Screen3View()
{
}

void Screen3View::setupScreen()
{
    Screen3ViewBase::setupScreen();
    randomSeed = 12345;

    currentScore = 0;
    playerHealth = 3;
    isGameOver = false;
    spawnTimer = 0;
    invulnerabilityCounter = 0;
    tickCounter = 0;

    Unicode::snprintf(scoreBuffer, 20, "0");
    txtScore.setWildcard(scoreBuffer);
    txtScore.invalidate();

    txtGameLose.setVisible(false);
    txtGameLose.invalidate();

    txtYourScore.setVisible(false);
    txtYourScore.invalidate();

    txtHighScore.setVisible(false);
    txtHighScore.invalidate();

    for (int i = 0; i < MAX_ALIENS_3; i++)
    {

        alienPool[i].setBitmap(touchgfx::Bitmap(ALIEN_BITMAPS[i%6]));
        alienPool[i].setVisible(false);
        add(alienPool[i]);

        alienSpeedX[i] = 0;
    }

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        enemyBulletPool[i].setBitmap(touchgfx::Bitmap(BITMAP_ENEMY_BULLET_RED_ID));
        enemyBulletPool[i].setVisible(false);
        add(enemyBulletPool[i]);
    }

    for (int i = 0; i < MAX_BULLETS_3; i++)
    {
        bulletPool[i].setBitmap(touchgfx::Bitmap(BITMAP_BULLET_DOUBLE_ID));
        bulletPool[i].setVisible(false);
        add(bulletPool[i]);
    }
    heartIcons[0] = &heart1;
	heartIcons[1] = &heart2;
	heartIcons[2] = &heart3;

    updateHealthUI();
}

void Screen3View::tearDownScreen()
{
    Screen3ViewBase::tearDownScreen();
}

void Screen3View::updateShipPosition(uint8_t dir)
{
    int step = 5;

    purpleplane.invalidate();

    if (dir == 'L' && purpleplane.getX() > 0)
    {
        purpleplane.setX(purpleplane.getX() - step);
    }
    else if (dir == 'R' && purpleplane.getX() < (240 - purpleplane.getWidth()))
    {
        purpleplane.setX(purpleplane.getX() + step);
    }
    else if (dir == 'U' && purpleplane.getY() > 0)
    {
        purpleplane.setY(purpleplane.getY() - step);
    }
    else if (dir == 'D' && purpleplane.getY() < (320 - purpleplane.getHeight()))
    {
        purpleplane.setY(purpleplane.getY() + step);
    }

    purpleplane.invalidate();
}

void Screen3View::handleTickEvent()
{

	if (isGameOver)
	{
		gameOverTickCounter++;

		if (gameOverTickCounter > 180)
		{
			presenter->gotoMenu();
		}

		return;
	}

    tickCounter++;
    randomSeed++;

    if (invulnerabilityCounter > 0)
    {
        invulnerabilityCounter--;
        if (invulnerabilityCounter % 5 == 0)
        {
            purpleplane.setVisible(!purpleplane.isVisible());
            purpleplane.invalidate();
        }

        if (invulnerabilityCounter == 0)
        {
            purpleplane.setVisible(true);
            purpleplane.invalidate();
        }
    }

    if (tickCounter % 15 == 0)
    {
        for (int i = 0; i < MAX_BULLETS_3; i++)
        {
            if (!bulletPool[i].isVisible())
            {
                bulletPool[i].setXY(purpleplane.getX() + purpleplane.getWidth()/2 - 5,
                                    purpleplane.getY() - 10);
                bulletPool[i].setVisible(true);
                bulletPool[i].invalidate();
                break;
            }
        }
    }

    for (int i = 0; i < MAX_BULLETS_3; i++)
    {
        if (bulletPool[i].isVisible())
        {
            bulletPool[i].invalidate();
            bulletPool[i].setY(bulletPool[i].getY() - 8);
            bulletPool[i].invalidate();

            if (bulletPool[i].getY() < -20)
            {
                bulletPool[i].setVisible(false);
                bulletPool[i].invalidate();
            }
        }
    }


    spawnTimer++;
    if (spawnTimer > 60)
    {
    	spawnTimer = 0;

		int countSpawned = 0;
		int batchSize = 3;

        for (int i = 0; i < MAX_ALIENS_3; i++)
        {
            if (!alienPool[i].isVisible())
            {
                int randomY = getRandomNumber(20, 150);

                if (getRandomNumber(0, 100) < 50)
                {
                    alienPool[i].setXY(-alienPool[i].getWidth(), randomY);
                    alienSpeedX[i] = 1 + getRandomNumber(0, 1);
                }
                else
                {
                    alienPool[i].setXY(SCREEN_WIDTH, randomY);
                    alienSpeedX[i] = -1 - getRandomNumber(0, 1);
                }

                alienPool[i].setVisible(true);
                alienPool[i].invalidate();
                alienReloadTimer[i] = 20;
                countSpawned++;
				if (countSpawned >= batchSize)
				{
					break;
				}
            }
        }
    }


    int hitBoxOffset = 4;

    for (int i = 0; i < MAX_ALIENS_3; i++)
    {
        if (alienPool[i].isVisible())
        {
            alienPool[i].invalidate();
            alienPool[i].setX(alienPool[i].getX() + alienSpeedX[i]);
            alienPool[i].invalidate();

            if (alienReloadTimer[i] > 0)
            {
                alienReloadTimer[i]--;
            }
            else{
            	for (int b = 0; b < MAX_ENEMY_BULLETS; b++)
				{
					if (!enemyBulletPool[b].isVisible())
					{
						enemyBulletPool[b].setXY(alienPool[i].getX() + alienPool[i].getWidth()/2,
												 alienPool[i].getY() + alienPool[i].getHeight());
						enemyBulletPool[b].setVisible(true);
						enemyBulletPool[b].invalidate();
						alienReloadTimer[i] = 60 + getRandomNumber(0, 60);
						break;
					}
				}
            }




            for (int b = 0; b < MAX_BULLETS_3; b++)
            {
                if (bulletPool[b].isVisible())
                {
                    if (checkCollision(bulletPool[b].getX(), bulletPool[b].getY(), bulletPool[b].getWidth(), bulletPool[b].getHeight(),
                                       alienPool[i].getX(), alienPool[i].getY(), alienPool[i].getWidth(), alienPool[i].getHeight()))
                    {
                        // Trúng đạn
                        bulletPool[b].setVisible(false);
                        bulletPool[b].invalidate();

                        alienPool[i].setVisible(false);
                        alienPool[i].invalidate();

                        // Tăng điểm
                        currentScore += 1;
                        Unicode::snprintf(scoreBuffer, 20, "%d", currentScore);
                        txtScore.setWildcard(scoreBuffer);
                        txtScore.invalidate();
                        break;
                    }
                }
            }

            // Kiểm tra va chạm với Tàu
            if (alienPool[i].isVisible() && invulnerabilityCounter == 0)
            {
                if (checkCollision(purpleplane.getX() + hitBoxOffset, purpleplane.getY() + hitBoxOffset,
                                   purpleplane.getWidth() - 2*hitBoxOffset, purpleplane.getHeight() - 2*hitBoxOffset,
                                   alienPool[i].getX(), alienPool[i].getY(), alienPool[i].getWidth(), alienPool[i].getHeight()))
                {
                    takeDamage();
                    alienPool[i].setVisible(false);
                    alienPool[i].invalidate();
                }
            }

            if (alienPool[i].getX() > SCREEN_WIDTH + 50 || alienPool[i].getX() < -50)
            {
                alienPool[i].setVisible(false);
                alienPool[i].invalidate();
            }
        }
    }


    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
    {
        if (enemyBulletPool[i].isVisible())
        {
            enemyBulletPool[i].invalidate();
            enemyBulletPool[i].setY(enemyBulletPool[i].getY() + 2);
            enemyBulletPool[i].invalidate();

            if (invulnerabilityCounter == 0)
            {
                if (checkCollision(enemyBulletPool[i].getX(), enemyBulletPool[i].getY(), 10, 10,
                                   purpleplane.getX() + hitBoxOffset, purpleplane.getY() + hitBoxOffset,
                                   purpleplane.getWidth()-2*hitBoxOffset, purpleplane.getHeight()-2*hitBoxOffset))
                {
                    enemyBulletPool[i].setVisible(false);
                    enemyBulletPool[i].invalidate();
                    updateHealthUI();
                    takeDamage();
                }
            }

            if (enemyBulletPool[i].getY() > SCREEN_HEIGHT)
            {
                enemyBulletPool[i].setVisible(false);
                enemyBulletPool[i].invalidate();
            }
        }
    }
}



void Screen3View::takeDamage()
{
    playerHealth--;
    updateHealthUI();

    if (playerHealth <= 0)
    {
        handleGameOver();
    }
    else
    {
        invulnerabilityCounter = 60;
    }
}

void Screen3View::handleGameOver()
{
	if (isGameOver) return;

	isGameOver = true;
	gameOverTickCounter = 0;

	presenter->saveScore(currentScore);

	int bestScore = presenter->getHighScore();

	Unicode::snprintf(scoreBuffer, 20, "%d", bestScore);
	txtHighScore.setWildcard(scoreBuffer);
	txtHighScore.setVisible(true);
	txtHighScore.invalidate();

	Unicode::snprintf(yourScoreBuffer, 20, "%d", currentScore);
	txtYourScore.setWildcard(yourScoreBuffer);
	txtYourScore.setVisible(true);
	txtYourScore.invalidate();

	txtGameLose.setVisible(true);
	txtGameLose.invalidate();


}

void Screen3View::updateHealthUI()
{
	for (int i = 0; i < 3; i++)
	{
		if (i < playerHealth)
		{
			heartIcons[i]->setVisible(true);
		}
		else
		{
			heartIcons[2-i]->setVisible(false);
		}

		heartIcons[i]->invalidate();
	}
}


int Screen3View::getRandomNumber(int min, int max)
{
    randomSeed = (randomSeed * 1103515245 + 12345) & 0x7FFFFFFF;
    if (min == max) return min;
    return min + (randomSeed % (max - min + 1));
}

bool Screen3View::checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}
