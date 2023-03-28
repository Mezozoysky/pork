#pragma once

// #include <Service.hpp>
#include <spdlog/spdlog.h>
#include <fmt/ostream.h>
#include <memory>
#include <vector>

namespace pork::core
{

using Logger = std::shared_ptr<spdlog::logger>;
using Sink = spdlog::sink_ptr;

void configureLogging(std::vector<Sink> const & sinks, std::string const & pattern, spdlog::level::level_enum level);
Logger createLogger(std::string const & name);

inline Logger getLogger()
{
    return spdlog::default_logger();
}

Logger getLogger(std::string const & name);

} // namespace pork::core

