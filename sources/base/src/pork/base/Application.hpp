#pragma once

#include <pork/base/Context.hpp>
#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <filesystem>

namespace pugi
{
class xml_document;
}

namespace pork::base
{

class Application
{
public:
    Application(std::string && appName, std::string && orgName = std::string{});
    virtual ~Application() noexcept = default;
    Application(Application const & other) = delete;
    Application & operator=(Application const & other) = delete;

    int run(int argc, char ** argv);
    void stop();
    bool isStopping() const noexcept;

    std::optional<std::filesystem::path> getBasePath() const noexcept;
    std::optional<std::filesystem::path> getPrefPath() const noexcept;

    Context & context() noexcept;

protected:
    virtual int onConfigure(std::vector<std::string_view> const & args,
                            pugi::xml_document const & configXml)
            = 0;
    virtual int onSetUp() = 0;
    virtual int onStart() = 0;
    virtual void onShutDown() = 0;

    virtual void iterate() = 0;

    virtual std::optional<std::filesystem::path> findConfig();

private:
    int configure(int argc, char ** argv);
    int setUp();
    int start();
    void shutDown();

private:
    //    void findBasePath();
    //    void findPrefPath();

    bool mStopping;

    std::optional<std::filesystem::path> mBasePath;
    std::optional<std::filesystem::path> mPrefPath;
    std::optional<std::filesystem::path> mConfigFilePath;

    Context mContext;
};

inline void Application::stop()
{
    mStopping = true;
}

inline bool Application::isStopping() const noexcept
{
    return mStopping;
}

inline std::optional<std::filesystem::path> Application::getBasePath() const noexcept
{
    return mBasePath;
}

inline std::optional<std::filesystem::path> Application::getPrefPath() const noexcept
{
    return mPrefPath;
}

inline Context & Application::context() noexcept
{
    return mContext;
}

} // namespace pork::base
