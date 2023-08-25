#include <pork/base/Context.hpp>
#include <pork/base/Application.hpp>
#include <string_view>

using namespace std::literals;


class App : public pork::base::Application
{
public:
    App();
    virtual ~App();

    int onConfigure(std::vector<std::string_view> const & args,
                    pugi::xml_document const & configXml) override;
    int onSetUp() override;
    int onStart() override;
    void onShutDown() override;
    void iterate() override;

private:
};
