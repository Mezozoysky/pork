#pragma once

#include "Service.hpp"
#include <spdlog/spdlog.h>
#include <fmt/ostream.h>
#include <memory>
#include <vector>

namespace pork::base
{

using Logger = std::shared_ptr<spdlog::logger>;
using Sink = spdlog::sink_ptr;

class Logging
: public Service<Logging>
{
public:
	Logging() noexcept = default;
	virtual ~Logging() noexcept = default;

	void configure(std::vector<Sink> const & sinks, std::string const & pattern, spdlog::level::level_enum level);

	Logger logger() const noexcept;
	Logger logger(std::string const & name) const noexcept;
	Logger createLogger(std::string const & name) const;

private:
	Logger mRootLogger;
};

inline Logger Logging::logger() const noexcept
{
	return mRootLogger;
}

inline Logger Logging::logger(std::string const & name) const noexcept
{
    return spdlog::get(name);
}

} // namespace pork::base
