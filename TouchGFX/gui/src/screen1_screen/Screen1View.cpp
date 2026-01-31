#include <gui/screen1_screen/Screen1View.hpp>



Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
	Screen1ViewBase::setupScreen();
	randomSeed = 12345;
	playerHealth = 3;
	invulnerabilityCounter = 0;

	tickCounter = 0;

	currentLevel = 1;
	enemiesDefeated = 0;
	enemiesToNextLevel = 30;

	currentAlienSpeed = 2;
	spawnSpeedRange = 25;

	nextSpawnTime = 30;
	gameWinTimer = 0;

	Unicode::snprintf(levelTextBuffer, 10, "%d", currentLevel);
	txtLevel.setWildcard(levelTextBuffer);
    txtLevel.invalidate();
    levelUpTimer = 0;

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bulletPool[i].setBitmap(touchgfx::Bitmap(BITMAP_BULLET_DOUBLE_ID));

		bulletPool[i].setVisible(false);

		add(bulletPool[i]);
	}
	currentAlienBitmapId = BITMAP_ALIEN1_ID;

	alienSpawnCounter = 0;
	for (int i = 0; i < MAX_ALIENS; i++)
	{
		alienPool[i].setBitmap(touchgfx::Bitmap(currentAlienBitmapId));
		alienPool[i].setVisible(false);
		add(alienPool[i]);
	}
	heartIcons[0] = &heart1;
	heartIcons[1] = &heart2;
	heartIcons[2] = &heart3;

	playerHealth = 3;
	updateHealthUI();

}

void Screen1View::nextLevel()
{
    currentLevel++;
    enemiesDefeated = 0;

    if (currentLevel == 2)
    {
    	currentAlienBitmapId = BITMAP_ALIEN2_ID;
        enemiesToNextLevel = 35;
        currentAlienSpeed = 3;
        spawnSpeedRange = 20;
    }
    else if (currentLevel == 3)
    {
    	currentAlienBitmapId = BITMAP_ALIEN3_ID;
        enemiesToNextLevel = 40;
        currentAlienSpeed = 4;
        spawnSpeedRange = 15;
    }
    else if (currentLevel == 4)
    {
    	currentAlienBitmapId = BITMAP_ALIEN6_ID;
        enemiesToNextLevel = 45;
        currentAlienSpeed = 5;
        spawnSpeedRange = 5;
    }
    else if (currentLevel > 4)
    {
    	txtGameWin.setVisible(true);
		txtGameWin.invalidate();

		txtLevelUpMsg.setVisible(false);
		txtLevelUpMsg.invalidate();

		gameWinTimer = 300;

        return;
    }

    Unicode::snprintf(levelTextBuffer, 10, "%d", currentLevel);
    txtLevel.setWildcard(levelTextBuffer);
    txtLevel.invalidate();
    txtLevelUpMsg.setVisible(true);
	txtLevelUpMsg.invalidate();

	levelUpTimer = 100;
}

void Screen1View::updateHealthUI()
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

bool Screen1View::checkCollision(int x1, int y1, int w1, int h1,
                                 int x2, int y2, int w2, int h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::updateShipPosition(uint8_t dir)
{
    int step = 5;

    purpleplane.invalidate();

    if (dir == 'L' && purpleplane.getX() > 0)
		purpleplane.setX(purpleplane.getX() - step);
	else if (dir == 'R' && purpleplane.getX() < (240 - purpleplane.getWidth()))
		purpleplane.setX(purpleplane.getX() + step);


	else if (dir == 'U' && purpleplane.getY() > 0)
		purpleplane.setY(purpleplane.getY() - step);
	else if (dir == 'D' && purpleplane.getY() < (320 - purpleplane.getHeight()))
		purpleplane.setY(purpleplane.getY() + step);

	purpleplane.invalidate();
}

void Screen1View::handleTickEvent()
{
    tickCounter++;

    randomSeed += 1;

    if (levelUpTimer > 0)
	{
		levelUpTimer--;
		if (levelUpTimer == 0)
		{
			txtLevelUpMsg.setVisible(false);
			txtLevelUpMsg.invalidate();
		}
	}

    if (gameWinTimer > 0)
	{
		gameWinTimer--;

		if (gameWinTimer == 0)
		{
			presenter->gameOver();
		}

		return;
	}

    if (tickCounter % 10 == 0)
    {
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!bulletPool[i].isVisible())
            {
                int startX = purpleplane.getX() + (purpleplane.getWidth() / 2) - (bulletPool[i].getWidth() / 2);
                int startY = purpleplane.getY() - bulletPool[i].getHeight();

                bulletPool[i].setXY(startX, startY);
                bulletPool[i].setVisible(true);
                bulletPool[i].invalidate();
                break;
            }
        }
    }

    int bulletSpeed = 8;

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bulletPool[i].isVisible())
        {
            bulletPool[i].invalidate();

            bulletPool[i].setY(bulletPool[i].getY() - bulletSpeed);

            bulletPool[i].invalidate();

            if (bulletPool[i].getY() < -20)
            {
                bulletPool[i].setVisible(false);
                bulletPool[i].invalidate();
            }
        }
    }

    alienSpawnCounter++;

	if (alienSpawnCounter > nextSpawnTime)
	{
		alienSpawnCounter = 0;
		nextSpawnTime = 15 + (getRandomNumber(0, spawnSpeedRange));

		for (int i = 0; i < MAX_ALIENS; i++)
		{
			if (!alienPool[i].isVisible())
			{

				alienPool[i].setBitmap(touchgfx::Bitmap(currentAlienBitmapId));

				int maxX = 240 - alienPool[i].getWidth();
				int randomX = getRandomNumber(0, maxX);

				alienPool[i].setXY(randomX, -alienPool[i].getHeight());
				alienPool[i].setVisible(true);
				alienPool[i].invalidate();
				break;
			}
		}
	}

	if (invulnerabilityCounter > 0)
	{
		invulnerabilityCounter--;

		// Nhấp nháy
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


	int hitBoxOffset = 4;

	for (int i = 0; i < MAX_ALIENS; i++)
	{
		if (alienPool[i].isVisible())
		{
			alienPool[i].invalidate();
			alienPool[i].setY(alienPool[i].getY() + currentAlienSpeed);
			alienPool[i].invalidate();

			int alienX = alienPool[i].getX();
			int alienY = alienPool[i].getY();
			int alienW = alienPool[i].getWidth();
			int alienH = alienPool[i].getHeight();
			int alienOffset = 2;


			for(int b = 0; b < MAX_BULLETS; b++)
			{
				if(bulletPool[b].isVisible())
				{
					if(checkCollision(bulletPool[b].getX(), bulletPool[b].getY(),
									  bulletPool[b].getWidth(), bulletPool[b].getHeight(),
									  alienX + alienOffset, alienY + alienOffset,
									  alienW - 2*alienOffset, alienH - 2*alienOffset))
					{
						// Bắn trúng
						bulletPool[b].setVisible(false);
						bulletPool[b].invalidate();

						// 2. Ẩn con quái đi
						alienPool[i].setVisible(false);
						alienPool[i].invalidate();

						enemiesDefeated++; // Tăng số kill

						if (enemiesDefeated >= enemiesToNextLevel)
						{
							nextLevel(); // Chuyển màn
						}
						break;
					}
				}
			}

			if (invulnerabilityCounter == 0 && alienPool[i].isVisible())
			{
				if (checkCollision(purpleplane.getX() + hitBoxOffset,
								   purpleplane.getY() + hitBoxOffset,
								   purpleplane.getWidth() - 2*hitBoxOffset,
								   purpleplane.getHeight() - 2*hitBoxOffset,
								   alienPool[i].getX(),
								   alienPool[i].getY(),
								   alienPool[i].getWidth(),
								   alienPool[i].getHeight()))
				{


					playerHealth--;
					updateHealthUI();
					alienPool[i].setVisible(false);
					alienPool[i].invalidate();

					if (playerHealth <= 0)
					{
						presenter->gameOver();
						return;
					}
					else
					{
						invulnerabilityCounter = 100;
					}
				}
			}


			 if (alienPool[i].getY() > 320)
			{
				playerHealth--;

				updateHealthUI();
				alienPool[i].setVisible(false);
				alienPool[i].invalidate();
				if (playerHealth <= 0)
				{
					presenter->gameOver();
					return;
				}
				else
				{
					invulnerabilityCounter = 100;
				}
			}
		}
	}


}
