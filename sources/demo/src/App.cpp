#include "App.hpp"


Application::Application()
: pork::base::BaseApplication("PorkDemo"sv, "PorkDynamics"sv)
{}

Application::~Application()
{}

int Application::onConfigure(std::vector<std::string_view> const & args, pugi::xml_document const & configXml)
{
        return 0;
}

int Application::onSetUp()
{
    return 0;
}
int Application::onStart()
{
    return 0;
}
void Application::onShutDown()
{}

void Application::iterate()
{
    stop();
}
