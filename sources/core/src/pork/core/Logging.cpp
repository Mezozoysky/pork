#include "Logging.hpp"
#include <chrono>
#include <cassert>

namespace pork::core
{

void configureLogging(std::vector<Sink> const & sinks, std::string const & pattern, spdlog::level::level_enum level)
{
    Logger root = std::make_shared<spdlog::logger>("app", sinks.begin(), sinks.end());
    spdlog::register_logger(root);
    spdlog::set_default_logger(root);

    spdlog::set_pattern(pattern);
    spdlog::set_level(level);

    spdlog::flush_every(std::chrono::seconds{ 1 });
}

Logger createLogger(std::string const & name)
{
    Logger logger;

    Logger root = getLogger();
    assert(root && "No default logger present");
    if (!root)
    {
        return nullptr;
    }
    logger = std::make_shared<spdlog::logger>(name, root->sinks().begin(), root->sinks().end());
    spdlog::register_logger(logger);
    return logger;
}

Logger getLogger(std::string const & name)
{
    Logger logger = spdlog::get(name);
    assert(logger && "Requested logger doesn't registered yet");
    if (!logger)
    {
        logger = createLogger(name);

    }
    return logger;
}

} // namespace pork::core

