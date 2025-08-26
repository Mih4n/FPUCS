#pragma once

extern "C"
{
    EXPORT const char* logger_getName(endstone::Logger* logger) {
        static thread_local std::string temp;
        temp = std::string(logger->getName()); 
        return temp.c_str();
    }

    EXPORT void logger_setLevel(endstone::Logger* logger, endstone::Logger::Level level) {
        logger->setLevel(level);
    }

    EXPORT bool logger_isEnabledFor(endstone::Logger* logger, endstone::Logger::Level level) {
        return logger->isEnabledFor(level);
    }

    EXPORT void logger_log(endstone::Logger* logger, endstone::Logger::Level level, const char* message) {
        logger->log(level, message);
    }
}