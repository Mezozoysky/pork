#include "Application.hpp"
#include <pork/base/Config.hpp>
#include <pork/core/StrUtils.hpp>
#include <pork/core/Errors.hpp>
#include <cassert>
#include <SDL3/SDL.h>
#include <SDL3/SDL_system.h>
#include <SDL3/SDL_log.h>
#include <pork/core/Logging.hpp>
#if defined(PORK_PLAFORM_ANDROID)
#include <spdlog/sinks/android_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#endif
#include <spdlog/sinks/basic_file_sink.h>
#include <pugixml.hpp>

//#if defined(PORK_PLATFORM_LINUX)
////#include <limits.h>
////#include <libgen.h>
////#include <unistd.h>
//#if defined(__sun)
//#define SELF_EXECUTABLE "/proc/self/path/a.out"
//#else
//#define SELF_EXECUTABLE "/proc/self/exe"
//#endif
//#endif

namespace fs = std::filesystem;
using namespace std::literals;
using pork::core::trim;
using pork::core::toLowerIn;
using pork::core::Logger;

namespace pork::base
{

Application::Application(std::string_view appName, std::optional<std::string_view> orgName)
: mStopping{false}
, mAppName{appName}
, mOrgName{orgName}
{
}

int Application::run(int argc, char ** argv)
{
    int error{0};

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

    if (0 != SDL_Init(SDL_INIT_TIMER))
    {
        auto errorMessage = fmt::format("Failed to initialize SDL core: {}", SDL_GetError());
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, errorMessage.data());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Ctitical error", errorMessage.data(), NULL);
        error = -1;
    }

    {
#if not defined(PORK_PLATFORM_ANDROID)
        char * basePath = SDL_GetBasePath();
        if (basePath == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Failed to obtain application base path: %s",
                         SDL_GetError());
        }
        else
        {
            mBasePath = basePath;
            SDL_free(basePath);
        }
#endif

        char * prefPath = SDL_GetPrefPath(mOrgName.has_value() ? mOrgName->data() : "",
                                          mAppName.data());
        if (prefPath == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Failed to obtain application pref path: %s",
                         SDL_GetError());
        }
        else
        {
            mPrefPath = prefPath;
            SDL_free(prefPath);
        }
    }

    Logger logger;
    try
    {
        if (!error)
        {
            error = configure(argc, argv);
            if (!error)
            {
                logger = core::getLogger();
            }
        }
    }
    catch (std::system_error const & ex)
    {
        auto errorMessage = fmt::format("Exception during configuring: {} (code: {})",
                                        ex.what(),
                                        ex.code().value());
        if (logger)
        {
            logger->critical(errorMessage);
        }
        else
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, errorMessage.data());
        }
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical error", errorMessage.data(), NULL);
        error = ex.code().value();
    }
    catch (std::exception const & ex)
    {
        auto errorMessage = fmt::format("Exception during configuration: {}", ex.what());
        if (logger)
        {
            logger->critical(errorMessage);
        }
        else
        {
            SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, errorMessage.data());
        }
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical error", errorMessage.data(), NULL);
        error = -1;
    }

    try // if configure was unsuccessfull these block wouldnt execute otherwise logger exists
    {
        if (!error)
        {
            error = setUp();
        }
        if (!error)
        {
            error = start();
        }
        if (!error)
        {
            while (!mStopping)
            {
                iterate();
            }
        }
    }
    catch (std::system_error const & ex)
    {
        auto errorMessage
                = fmt::format("Unhandled exception: {} (code: {})", ex.what(), ex.code().value());
        logger->critical(errorMessage);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical error", errorMessage.data(), NULL);
        error = ex.code().value();
    }
    catch (std::exception const & ex)
    {
        auto errorMessage = fmt::format("Unhandled exception: {}", ex.what());
        logger->critical(errorMessage);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical error", errorMessage.data(), NULL);
        error = -1;
    }

    shutDown();
    SDL_Quit();
    return error;
}

std::optional<fs::path> Application::findConfig()
{
    std::string configName{fmt::format("{}.xml", mAppName)};

    fs::path configPath;

    try
    {
        // checking for config in cwd
        fs::path cwd = fs::current_path();
        if (!cwd.empty())
        {
            configPath = cwd;
            configPath.append(configName);
            if (fs::exists(configPath)
                and (fs::is_regular_file(configPath) or (fs::is_symlink(configPath))))
            {
                return configPath;
            }
        }
    }
    catch (fs::filesystem_error const & ex)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Error while searching for config in current directory: %s",
                     ex.what());
    }

    // checking for config in base
    if (mBasePath.has_value())
    {
        try
        {
            configPath = mBasePath.value();
            configPath.append(configName);
            if (fs::exists(configPath)
                and (fs::is_regular_file(configPath) or (fs::is_symlink(configPath))))
            {
                return configPath;
            }
        }
        catch (fs::filesystem_error const & ex)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Error while searching for config in base directory: %s",
                         ex.what());
        }

        // checking for base parent
        try
        {
            configPath = mBasePath.value().parent_path();
            if (configPath != mBasePath.value())
            {
                configPath.append(configName);

                if (fs::exists(configPath)
                    and (fs::is_regular_file(configPath) or (fs::is_symlink(configPath))))
                {
                    return configPath;
                }
            }
        }
        catch (fs::filesystem_error const & ex)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Error while searching for config in base parent directory: %s",
                         ex.what());
        }
    }

    // checking for config in pref
    if (mPrefPath.has_value())
    {
        try
        {
            // checking for <AppName>.xml
            configPath = mPrefPath.value();
            configPath.append(configName);

            if (fs::exists(configPath)
                and (fs::is_regular_file(configPath) or (fs::is_symlink(configPath))))
            {
                return configPath;
            }

            // checking for config.xml
            configPath = mPrefPath.value();
            configPath.append("config.xml");

            if (fs::exists(configPath)
                and (fs::is_regular_file(configPath) or (fs::is_symlink(configPath))))
            {
                return configPath;
            }
        }
        catch (fs::filesystem_error const & ex)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Error while searching for config in pref directory: %s",
                         ex.what());
        }
    }

    // checking for config in etc
    if (mBasePath.has_value())
    {
        try
        {
            if (fs::is_directory(mBasePath.value()) and mBasePath.value().has_parent_path()
                and mBasePath.value().filename().string() == "bin")
            {
                fs::path etcPath = mBasePath.value().parent_path();
                etcPath.append("etc");

                // checking for etc/<AppName>.xml
                configPath = etcPath;
                configPath.append(configName);

                if (fs::exists(configPath)
                    and (fs::is_regular_file(configPath) or (fs::is_symlink(configPath))))
                {
                    return configPath;
                }

                // checking for etc/<AppName>/config.xml
                configPath = etcPath;
                configPath.append(mAppName);
                configPath.append("config.xml");

                if (fs::exists(configPath)
                    and (fs::is_regular_file(configPath) or (fs::is_symlink(configPath))))
                {
                    return configPath;
                }
            }
        }
        catch (fs::filesystem_error const & ex)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Error while searching for config in etc/ directory: %s",
                         ex.what());
        }
    }

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Config file isnt found");
    return std::nullopt;
}

int Application::configure(int argc, char ** argv)
{
    auto * config = context().addService<Config>();
    config->setValue("app.config-path", ""s);

    std::vector<std::string_view> args;
    for (int idx = 0; idx < argc; ++idx)
    {
        args.emplace_back(argv[idx]);
    }

#if defined(PORK_PLATFORM_ANDROID)
    mConfigFilePath = fmt::format("{}.xml", mAppName);
#else
    for (std::size_t idx = 0u; idx < args.size(); ++idx)
    {
        if (args[idx] == "--config"sv and idx < args.size() - 1u)
        {
            mConfigFilePath = {args[++idx]};
            break;
        }
    }

    if (!mConfigFilePath.has_value())
    {
        mConfigFilePath = findConfig();
    }
#endif

    pugi::xml_document configXml;
    bool configLoaded{false};
    if (mConfigFilePath.has_value())
    {
        auto result = configXml.load_file(mConfigFilePath.value().native().c_str());
        if (result.status != pugi::xml_parse_status::status_ok)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Failed to load config file '%s': %s",
                         mConfigFilePath.value().native().data(),
                         result.description());
        }
        else
        {
            configLoaded = true;
            std::ostringstream ss;
            configXml.save(ss, "  ");
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Config loaded:\n%s", ss.str().data());

            config->setValue("app.config-path", std::move(mConfigFilePath.value()));
        }
    }

    spdlog::level::level_enum level = spdlog::level::info;
    std::string pattern = "%Y-%m-%d %H:%M:%S:%e | %n | %l |: %v";
    std::vector<spdlog::sink_ptr> sinks;
    bool multithreaded{true};

    if (configLoaded && configXml.empty())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Config is malformed: document is empty");
    }
    else if (configLoaded)
    {
        auto rootNode = configXml.child("config");
        if (rootNode.empty())
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Config is malformed: no root node with named 'config'");
        }
        else
        {
            auto logNode = rootNode.child("log");
            if (logNode)
            {
                auto levelAttr = logNode.attribute("level");
                if (!levelAttr.empty())
                {
                    std::string levelStr{levelAttr.as_string()};
                    trim(levelStr);
                    level = spdlog::level::from_str(levelStr);
                    if (level == spdlog::level::off && levelStr != "off")
                    {
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                                     "Log config is malformed: unexpected level value: '%s'",
                                     levelStr.data());
                        level = spdlog::level::info;
                    }
                }
            }
            // 			auto * pattern_attr = log_node->first_attribute("pattern");
            // 			if (pattern_attr)
            // 			{
            // 				pattern.assign(pattern_attr->value(), pattern_attr->value_size());
            // 			}

            // 			auto * mt_attr = log_node->first_attribute("multithreaded");
            // 			if (mt_attr)
            // 			{
            // 				std::string mtStr{ mt_attr->value(), mt_attr->value_size() };
            // 				stru::trim(mtStr);
            // 				if (!stru::parseBool(mtStr, multithreaded))
            // 				{
            // 					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Log config is malformed:
            // unexpected multithreaded value: '%s'", mtStr.data());
            // 				}
            // 			}

            // #if defined(PORK_PLATFORM_ANDROID)
            // 			multithreaded
            // 			? sinks.push_back(std::make_shared<spdlog::sinks::android_sink_mt>())
            // 			: sinks.push_back(std::make_shared<spdlog::sinks::android_sink_st>());

            // 			if (mPrefPath.has_value())
            // 			{
            // 				fs::path path = mPrefPath.value() / fmt::format("{}.log", mAppName);
            // 				multithreaded
            // 				?
            // sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.generic_string(),
            // true)) 				:
            // sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>(path.generic_string(),
            // true));
            // 			}
            // #else
            // 			auto * sinks_node = log_node->first_node("sinks");
            // 			if (sinks_node)
            // 			{
            // 				auto * sink_node = sinks_node->first_node("stdout");

            // 				if (sink_node)
            // 				{
            // 					bool colored{ false };
            // 					auto * color_attr = sink_node->first_attribute("color");

            // 					if (color_attr)
            // 					{
            // 						std::string colorStr{ color_attr->value(),
            // color_attr->value_size()
            // };

            // 						if (!stru::parseBool(colorStr, colored))
            // 						{
            // 							SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Log/sinks/stdout
            // config is malformed: unexpected color value: '%s'", colorStr.data());
            // 						}
            // 					}

            // 					if (colored)
            // 					{
            // 						multithreaded
            // 						?
            // sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::always))
            // 						:
            // sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>(spdlog::color_mode::always));
            // 					}
            // 					else
            // 					{
            // 						multithreaded
            // 						?
            // sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>()) :
            // sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
            // 					}
            // 				}

            // 				sink_node = sinks_node->first_node("stderr");

            // 				if (sink_node)
            // 				{
            // 					bool colored{ false };
            // 					auto * color_attr = sink_node->first_attribute("color");

            // 					if (color_attr)
            // 					{
            // 						std::string colorStr{ color_attr->value(),
            // color_attr->value_size()
            // };

            // 						if (!stru::parseBool(colorStr, colored))
            // 						{
            // 							SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Log/sinks/stderr
            // config is malformed: unexpected color value: '%s'", colorStr.data());
            // 						}
            // 					}

            // 					if (colored)
            // 					{
            // 						multithreaded
            // 						?
            // sinks.push_back(std::make_shared<spdlog::sinks::stderr_color_sink_mt>())
            // : sinks.push_back(std::make_shared<spdlog::sinks::stderr_color_sink_st>());
            // 					}
            // 					else
            // 					{
            // 						multithreaded
            // 						?
            // sinks.push_back(std::make_shared<spdlog::sinks::stderr_sink_mt>()) :
            // sinks.push_back(std::make_shared<spdlog::sinks::stderr_sink_st>());
            // 					}
            // 				}

            // 				sink_node = sinks_node->first_node("file");

            // 				if (sink_node)
            // 				{
            // 					std::string fileName{ fmt::format("{}.log", mAppName) };
            // 					auto * name_attr = sink_node->first_attribute("name");

            // 					if (name_attr)
            // 					{
            // 						std::string name{ name_attr->value(), name_attr->value_size() };
            // 						stru::trim(name);

            // 						if (name.empty())
            // 						{
            // 							SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Log/sinks/file
            // config is malformed: name is empty");
            // 						}
            // 						else
            // 						{
            // 							fileName = name;
            // 						}
            // 					}

            // 					bool truncate{ true };
            // 					auto * truncate_attr = sink_node->first_attribute("truncate");

            // 					if (truncate_attr)
            // 					{
            // 						std::string truncateStr{ truncate_attr->value(),
            // truncate_attr->value_size()
            // };

            // 						if (!stru::parseBool(truncateStr, truncate))
            // 						{
            // 							SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Log/sinks/file
            // config is malformed: unexpected color value: '%s'", truncateStr.data());
            // 						}
            // 					}

            // 					fs::path filePath{ fileName };

            // 					if (filePath.is_relative())
            // 					{
            // 						if (mPrefPath.has_value())
            // 						{
            // 							filePath = mPrefPath.value() / filePath;
            // 							filePath = filePath.lexically_normal();
            // 						}
            // 						else if (mBasePath.has_value())
            // 						{
            // 							if (fs::is_directory(mBasePath.value()) and
            // mBasePath.value().has_parent_path() and mBasePath.value().filename().string() ==
            // "bin")
            // 							{
            // 								filePath = mBasePath.value().parent_path() / filePath;
            // 							}
            // 							else
            // 							{
            // 								filePath = mBasePath.value() / filePath;
            // 							}
            // 						}
            // 						else
            // 						{
            // 							filePath = fs::current_path() / filePath;
            // 						}
            // 					}

            // 					multithreaded
            // 					?
            // sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath.generic_string(),
            // truncate)) 					:
            // sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>(filePath.generic_string(),
            // truncate));
            // 				}
            // 			}
            // #endif
            // 		}
        } // rootNode
    }     // configXml
#if not defined(PORK_PLATFORM_ANDROID)
    if (sinks.empty())
    {
        fs::path logPath;
        if (mPrefPath.has_value())
        {
            logPath = mPrefPath.value();
        }
#if defined(PORK_PLATFORM_WINDOWS)
        else if (mBasePath.has_value())
        {
            logPath = mBasePath.value();
        }
#endif
        else
        {
            logPath = fs::current_path();
        }
        logPath /= fmt::format("{}.log", mAppName);
        multithreaded ? sinks.push_back(
                std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath.generic_string(), true))
                      : sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>(
                              logPath.generic_string(), true));
#if not defined(PORK_PLATFORM_WINDOWS)
        multithreaded ? sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>())
                      : sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
#endif
    }
#endif

    try
    {
        for (std::size_t idx = 0u; idx < args.size(); ++idx)
        {
            if (args[idx] == "--log-level"sv and idx < args.size() - 1u)
            {
                std::string levelStr{args[idx + 1]};
                trim(levelStr);
                toLowerIn(levelStr);
                level = spdlog::level::from_str(levelStr);
            }
        }
    }
    catch (std::exception const & ex)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Exception while parsing log level from command line: %s",
                     ex.what());
    }

    try
    {
        core::configureLogging(sinks, pattern, level);
    }
    catch (std::exception const & ex)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Exception while configuring logging service: %s",
                     ex.what());
        return -2;
    }

    // since that logging exists

    Logger logger = core::getLogger();
    logger->info("Log initialized");
    logger->info("Base path: {}",
                 mBasePath.has_value() ? mBasePath.value().generic_string() : "none");
    logger->info("Pref path: {}",
                 mPrefPath.has_value() ? mPrefPath.value().generic_string() : "none");
    logger->info("Config file: {}",
                 mConfigFilePath.has_value() ? mConfigFilePath.value().generic_string() : "none");

    int error{-2};
    try
    {
        error = onConfigure(args, configXml);
    }
    catch (std::system_error const & ex)
    {
        logger->error("Error while configuring application: {} (code: {})",
                      ex.what(),
                      ex.code().value());
        error = ex.code().value();
    }
    catch (std::exception const & ex)
    {
        logger->error("Error while configuring application: {}", ex.what());
    }

    return error;
}

int Application::setUp()
{
    int error{0};

    if (!error)
    {
        error = onSetUp();
    }

    return error;
}

int Application::start()
{
    int error{0};

    if (!error)
    {
        error = onStart();
    }
    return error;
}

void Application::shutDown()
{
    // shutdown executes even if configure was unsuccessfull and logging isnt exist
    Logger logger = core::getLogger();

    try
    {
        onShutDown();
    }
    catch (std::system_error const & ex)
    {
        auto errorMessage = fmt::format("Exception while onShutdown: {} (code: {})",
                                        ex.what(),
                                        ex.code().value());
        if (logger)
        {
            logger->critical(errorMessage);
        }
        else
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, errorMessage.data());
        }
    }
    catch (std::exception const & ex)
    {
        auto errorMessage = fmt::format("Exception while onShutdown: {}", ex.what());
        if (logger)
        {
            logger->critical(errorMessage);
        }
        else
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, errorMessage.data());
        }
    }

    try
    {
        // TODO: base::Application-level shutdown
    }
    catch (std::system_error const & ex)
    {
        auto errorMessage = fmt::format("Exception while shutdown: {} (code: {})",
                                        ex.what(),
                                        ex.code().value());
        if (logger)
        {
            logger->critical(errorMessage);
        }
        else
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, errorMessage.data());
        }
    }
    catch (std::exception const & ex)
    {
        auto errorMessage = fmt::format("Exception while shutdown: {}", ex.what());
        if (logger)
        {
            logger->critical(errorMessage);
        }
        else
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, errorMessage.data());
        }
    }
}

// void Application::findBasePath()
//{
//#if defined(PORK_PLATFORM_LINUX)
//    char exePathStr[PATH_MAX];
//    if (NULL == realpath(SELF_EXECUTABLE, exePathStr))
//    {
//        sf::err() << "ERROR: Failed to fetch executable path" << std::endl;
//    }

//    fs::path path{exePathStr};
//    path = path.parent_path();

//    mBasePath = std::move(path);
//#elif defined(PORK_PLATFORM_WINDOWS)
//    throw core::NotImplementedError("pork::base::Application::findBasePath isn't implemented on
//    Windows platform");
//#elif defined(PORK_PLATFORM_MACOS)
//    throw core::NotImplementedError("pork::base::Application::findBasePath isn't implemented on
//    Mac OS X platform");
//#elif defined(PORK_PLATFORM_ANDROID)
//    throw core::NotImplementedError("pork::base::Application::findBasePath isn't implemented on
//    Android platform");
//#endif
//    throw core::NotImplementedError("pork::base::Application::findBasePath isn't implemented on
//    given platform");
//}

// void Application::findPrefPath()
//{
//#if defined(PORK_PLATFORM_LINUX)
//    fs::path path;
//    char * configHomePath = getenv("XDG_CONFIG_PATH");
//    if (configHomePath == NULL)
//    {
//        configHomePath = getenv("HOME");
//        if (configHomePath == NULL)
//        {
//            sf::err() << "ERROR: Failed to obtain home directory path" << std::endl;
//            return;
//        }

//        path = configHomePath;
//        path /= ".config";
//    }
//    else
//    {
//        path = configHomePath;
//    }

//    if (mOrgName.has_value())
//    {
//        path /= mOrgName.value();
//    }
//    path /= mAppName;

//    mPrefPath = std::move(path);
//#elif defined(PORK_PLATFORM_WINDOWS)
//    throw core::NotImplementedError("pork::base::Application::findPrefPath isn't implemented on
//    Windows platform");
//#elif defined(PORK_PLATFORM_MACOS)
//    throw core::NotImplementedError("pork::base::Application::findPrefPath isn't implemented on
//    Mac OS X platform");
//#elif defined(PORK_PLATFORM_ANDROID)
//    throw core::NotImplementedError("pork::base::Application::findPrefPath isn't implemented on
//    Android platform");
//#endif
//    throw core::NotImplementedError("pork::base::Application::findPrefPath isn't implemented on
//    given platform");
//}

} // namespace pork::base
