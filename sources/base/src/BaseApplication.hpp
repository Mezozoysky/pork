#pragma once

#include "Context.hpp"
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

class BaseApplication
{
public:
    BaseApplication(std::string_view appName, std::optional<std::string_view> orgName = std::nullopt);
    virtual ~BaseApplication() noexcept = default;
    BaseApplication(BaseApplication const & other) = delete;
    BaseApplication & operator=(BaseApplication const & other) = delete;

    int run(int argc, char ** argv);
    void stop();
    bool isStopping() const noexcept;

    std::string_view getAppName() const noexcept;
    std::optional<std::string_view> getOrgName() const noexcept;
    std::optional<std::filesystem::path> getBasePath() const noexcept;
    std::optional<std::filesystem::path> getPrefPath() const noexcept;

    Context & context() noexcept;

protected:
    virtual int onConfigure(std::vector<std::string_view> const & args, pugi::xml_document const & configXml) = 0;
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
    void findBasePath();
    void findPrefPath();

    bool mStopping;

    std::string mAppName;
    std::optional<std::string> mOrgName;

    std::optional<std::filesystem::path> mBasePath;
    std::optional<std::filesystem::path> mPrefPath;
    std::optional<std::filesystem::path> mConfigFilePath;

    Context mContext;
};

inline void BaseApplication::stop()
{
    mStopping = true;
}

inline bool BaseApplication::isStopping() const noexcept
{
    return mStopping;
}

inline std::optional<std::string_view> BaseApplication::getOrgName() const noexcept
{
    return mOrgName;
}

inline std::string_view BaseApplication::getAppName() const noexcept
{
    return mAppName;
}

inline std::optional<std::filesystem::path> BaseApplication::getBasePath() const noexcept
{
    return mBasePath;
}

inline std::optional<std::filesystem::path> BaseApplication::getPrefPath() const noexcept
{
    return mPrefPath;
}

inline Context & BaseApplication::context() noexcept
{
    return mContext;
}

} // namespace pork::base
