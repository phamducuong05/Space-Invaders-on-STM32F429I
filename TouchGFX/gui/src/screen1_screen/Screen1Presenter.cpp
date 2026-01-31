#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <gui/common/FrontendApplication.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

void Screen1Presenter::moveShip(uint8_t dir)
{
    view.updateShipPosition(dir);
}

void Screen1Presenter::gameOver()
{
	static_cast<FrontendApplication*>(touchgfx::Application::getInstance())->gotoScreen2ScreenNoTransition();
}
