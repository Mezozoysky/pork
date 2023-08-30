#include "App.hpp"
#include <pork/core/Logging.hpp>


App::App()
: pork::base::Application("Demo"s, "PorkDynamics"s)
{
}

App::~App()
{
}

int App::onConfigure(std::vector<std::string_view> const & args,
                     pugi::xml_document const & configXml)
{
    return 0;
}

int App::onSetUp()
{
    return 0;
}

int App::onStart()
{
    return 0;
}

void App::onShutDown()
{
}

void App::iterate()
{
    stop();
}
