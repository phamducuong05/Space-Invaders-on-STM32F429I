#include <gui/screen3_screen/Screen3View.hpp>
#include <gui/screen3_screen/Screen3Presenter.hpp>
#include <gui/common/FrontendApplication.hpp>

Screen3Presenter::Screen3Presenter(Screen3View& v)
    : view(v)
{

}

void Screen3Presenter::activate()
{

}

void Screen3Presenter::deactivate()
{

}

void Screen3Presenter::saveScore(int score)
{
    model->saveScore(score);
}

int Screen3Presenter::getHighScore()
{
    return model->getHighScore();
}

void Screen3Presenter::moveShip(uint8_t dir)
{
    view.updateShipPosition(dir);
}

void Screen3Presenter::gotoMenu()
{
	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoScreen2ScreenNoTransition();
}
