#define ELPP_DISABLE_DEBUG_LOGS
#define ELPP_DISABLE_TRACE_LOGS
#define ELPP_NO_DEFAULT_LOG_FILE

#include "config.h"
#include "easylogging++.h"
#include <string>
#include <iostream>

INITIALIZE_EASYLOGGINGPP

void InitLog(::tools::config::Parser &config_parser) {
    std::string log_path = config_parser["log"]["log_path"];
    std::string format = config_parser["log"]["format"];
    std::string flush_threshold = config_parser["log"]["flush_threshold"];

    ::el::Configurations conf;
    conf.setToDefault();
    std::cout << log_path << " " << format << " " << flush_threshold << std::endl;
    conf.setGlobally(::el::ConfigurationType::Filename, log_path);
    conf.setGlobally(::el::ConfigurationType::Format, format);
    conf.setGlobally(::el::ConfigurationType::ToStandardOutput, "false");
    ::el::Loggers::reconfigureLogger("default", conf);
}


int main() {
    tools::config::Parser config_parser;
    config_parser.Load("config.ini");

    InitLog(config_parser);

    LOG(DEBUG) << "Log using default file DEBUG";
    LOG(TRACE) << "Log using default file TRACE";
    LOG(INFO) << "Log using default file INFO";
    LOG(ERROR) << "Log using default file ERROR";

    return 0;
}
