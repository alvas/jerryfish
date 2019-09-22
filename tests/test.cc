/*
 * test.cc
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

//#include "test.h"
#include "src/Logger.h"
#include "src/Utils.h"

int main(int argc, char** argv)
{
    JerryFish::Logger::ptr logger(new JerryFish::Logger);
    logger->addAppender(JerryFish::LogAppender::ptr(new JerryFish::StdoutLogAppender));
    //JerryFish::LogEvent::ptr event(new JerryFish::LogEvent(logger, JerryFish::LogLevel::DEBUG, __FILE__, __LINE__, 0, 1, 2, time(0), ""));
    //logger->log(JerryFish::LogLevel::DEBUG, event);

    JERRYFISH_LOG_INFO(logger) << "test macro";
    JERRYFISH_LOG_ERROR(logger) << "test macro error";

    JerryFish::FileLogAppender::ptr file_appender(new JerryFish::FileLogAppender("./log.txt"));
    JerryFish::LogFormatter::ptr fmt(new JerryFish::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(JerryFish::LogLevel::ERROR);

    logger->addAppender(file_appender);

    JERRYFISH_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    auto l = JerryFish::LoggerMgr::GetInstance()->getLogger("xx");
    JERRYFISH_LOG_INFO(l) << "xxx";

    return 0;
}



