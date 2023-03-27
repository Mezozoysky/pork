#include <Context.hpp>
#include <BaseApplication.hpp>
#include <string_view>

using namespace std::literals;


class Application
: public pork::base::BaseApplication
{
public:
    Application();
    virtual ~Application();

    int onConfigure(std::vector<std::string_view> const & args, pugi::xml_document const & configXml) override;
    int onSetUp() override;
    int onStart() override;
    void onShutDown() override;
    void iterate() override;

private:
};

