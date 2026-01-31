#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "cmsis_os.h"

extern osMessageQueueId_t gameQueueHandle;

Model::Model() : modelListener(0), highScore(0)
{

}

void Model::tick()
{
	uint8_t msg = 0;

	if (osMessageQueueGet(gameQueueHandle, &msg, NULL, 0) == osOK)
	{
		modelListener->moveShip(msg);
	}
}

void Model::saveScore(int newScore)
{
    if (newScore > highScore)
    {
        highScore = newScore;
    }
}

int Model::getHighScore()
{
    return highScore;
}
