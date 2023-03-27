#include "Logging.hpp"
#include <chrono>

namespace pork::base
{

void Logging::configure(std::vector<Sink> const & sinks, std::string const & pattern, spdlog::level::level_enum level)
{
	mRootLogger = std::make_shared<spdlog::logger>("app", sinks.begin(), sinks.end());
	spdlog::register_logger(mRootLogger);
	spdlog::set_default_logger(mRootLogger);

	spdlog::set_pattern(pattern);
	spdlog::set_level(level);

	spdlog::flush_every(std::chrono::seconds{ 1 });
}

Logger Logging::createLogger(std::string const & name) const
{
	Logger logger = std::make_shared<spdlog::logger>(name, mRootLogger->sinks().begin(), mRootLogger->sinks().end());
	spdlog::register_logger(logger);
	return logger;
}

} // namespace pork::base
