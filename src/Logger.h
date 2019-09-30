/*
 * Logger.hpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef LOGGER
#define LOGGER

#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

//#include "Config.h"
#include "Singleton.h"
#include "Debugger.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
//#define JERRYFISH_LOG_LEVEL(logger, level) \
    //if (logger->getLevel() <= level) \
        //JerryFish::LogEventWrap(JerryFish::LogEvent::ptr(new JerryFish::LogEvent(logger, level, \
                        //__FILE__, __LINE__, 0, JerryFish::GetThreadId(),\
                //JerryFish::GetFiberId(), time(0), JerryFish::Thread::GetName()))).getSS()

#define JERRYFISH_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level) \
        JerryFish::LogEventWrap(JerryFish::LogEvent::ptr(new JerryFish::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, JerryFish::GetThreadId(),\
                JerryFish::GetFiberId(), time(0), "thread name"))).getSS()

/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define JERRYFISH_LOG_DEBUG(logger) JERRYFISH_LOG_LEVEL(logger, JerryFish::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define JERRYFISH_LOG_INFO(logger) JERRYFISH_LOG_LEVEL(logger, JerryFish::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define JERRYFISH_LOG_WARN(logger) JERRYFISH_LOG_LEVEL(logger, JerryFish::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define JERRYFISH_LOG_ERROR(logger) JERRYFISH_LOG_LEVEL(logger, JerryFish::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define JERRYFISH_LOG_FATAL(logger) JERRYFISH_LOG_LEVEL(logger, JerryFish::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
//#define JERRYFISH_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    //if (logger->getLevel() <= level) \
        //JerryFish::LogEventWrap(JerryFish::LogEvent::ptr(new JerryFish::LogEvent(logger, level, \
                        //__FILE__, __LINE__, 0, JerryFish::GetThreadId(),\
                //JerryFish::GetFiberId(), time(0), JerryFish::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__)

#define JERRYFISH_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if (logger->getLevel() <= level) \
        JerryFish::LogEventWrap(JerryFish::LogEvent::ptr(new JerryFish::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, JerryFish::GetThreadId(),\
                JerryFish::GetFiberId(), time(0), "thread name"))).getEvent()->format(fmt, __VA_ARGS__)
/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define JERRYFISH_LOG_FMT_DEBUG(logger, fmt, ...) JERRYFISH_LOG_FMT_LEVEL(logger, JerryFish::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define JERRYFISH_LOG_FMT_INFO(logger, fmt, ...)  JERRYFISH_LOG_FMT_LEVEL(logger, JerryFish::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define JERRYFISH_LOG_FMT_WARN(logger, fmt, ...)  JERRYFISH_LOG_FMT_LEVEL(logger, JerryFish::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define JERRYFISH_LOG_FMT_ERROR(logger, fmt, ...) JERRYFISH_LOG_FMT_LEVEL(logger, JerryFish::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define JERRYFISH_LOG_FMT_FATAL(logger, fmt, ...) JERRYFISH_LOG_FMT_LEVEL(logger, JerryFish::LogLevel::FATAL, fmt, __VA_ARGS__)

/**
 * @brief 获取主日志器
 */
#define JERRYFISH_LOG_ROOT() JerryFish::LoggerMgr::GetInstance()->getRoot()

/**
 * @brief 获取name的日志器
 */
#define JERRYFISH_LOG_NAME(name) JerryFish::LoggerMgr::GetInstance()->getLogger(name)

namespace JerryFish
{
    class Logger;
    class LoggerManager;

    /**
     * @brief 日志级别
     */
    class LogLevel 
    {
        public:
            /**
             * @brief 日志级别枚举
             */
            enum Level 
            {
                /// 未知级别
                UNKNOW = 0,
                /// DEBUG 级别
                DEBUG = 1,
                /// INFO 级别
                INFO = 2,
                /// WARN 级别
                WARN = 3,
                /// ERROR 级别
                ERROR = 4,
                /// FATAL 级别
                FATAL = 5
            };

            /**
             * @brief 将日志级别转成文本输出
             * @param[in] level 日志级别
             */
            static const char* ToString(LogLevel::Level level)
            {
                switch(level) 
                {
                #define XX(name) \
                    case LogLevel::name: \
                        return #name; \
                        break;

                    XX(DEBUG);
                    XX(INFO);
                    XX(WARN);
                    XX(ERROR);
                    XX(FATAL);
                #undef XX

                    default:
                    return "UNKNOW";
                }
                return "UNKNOW";
            }

            /**
             * @brief 将文本转换成日志级别
             * @param[in] str 日志级别文本
             */
            static LogLevel::Level FromString(const std::string& str)
            {
                #define XX(level, v) \
                    if (str == #v) \
                    { \
                        return LogLevel::level; \
                    }

                XX(DEBUG, debug);
                XX(INFO, info);
                XX(WARN, warn);
                XX(ERROR, error);
                XX(FATAL, fatal);

                XX(DEBUG, DEBUG);
                XX(INFO, INFO);
                XX(WARN, WARN);
                XX(ERROR, ERROR);
                XX(FATAL, FATAL);
                return LogLevel::UNKNOW;
                #undef XX
            };
    };

    /**
     * @brief 日志事件
     */
    class LogEvent 
    {
        public:
            typedef std::shared_ptr<LogEvent> ptr;

            /**
             * @brief 构造函数
             * @param[in] logger 日志器
             * @param[in] level 日志级别
             * @param[in] file 文件名
             * @param[in] line 文件行号
             * @param[in] elapse 程序启动依赖的耗时(毫秒)
             * @param[in] thread_id 线程id
             * @param[in] fiber_id 协程id
             * @param[in] time 日志事件(秒)
             * @param[in] thread_name 线程名称
             */
            LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
                    ,const char* file, int32_t line, uint32_t elapse
                    ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
                    ,const std::string& thread_name):m_file(file)
                                                     ,m_line(line)
                                                     ,m_elapse(elapse)
                                                     ,m_threadId(thread_id)
                                                     ,m_fiberId(fiber_id)
                                                     ,m_time(time)
                                                     ,m_threadName(thread_name)
                                                     ,m_logger(logger)
                                                     ,m_level(level) { }

            /**
             * @brief 返回文件名
             */
            const char* getFile() const { return m_file;}

            /**
             * @brief 返回行号
             */
            int32_t getLine() const { return m_line;}

            /**
             * @brief 返回耗时
             */
            uint32_t getElapse() const { return m_elapse;}

            /**
             * @brief 返回线程ID
             */
            uint32_t getThreadId() const { return m_threadId;}

            /**
             * @brief 返回协程ID
             */
            uint32_t getFiberId() const { return m_fiberId;}

            /**
             * @brief 返回时间
             */
            uint64_t getTime() const { return m_time;}

            /**
             * @brief 返回线程名称
             */
            const std::string& getThreadName() const { return m_threadName;}

            /**
             * @brief 返回日志内容
             */
            std::string getContent() const { return m_ss.str();}

            /**
             * @brief 返回日志器
             */
            std::shared_ptr<Logger> getLogger() const { return m_logger;}

            /**
             * @brief 返回日志级别
             */
            LogLevel::Level getLevel() const { return m_level;}

            /**
             * @brief 返回日志内容字符串流
             */
            std::stringstream& getSS() { return m_ss;}

            /**
             * @brief 格式化写入日志内容
             */
            void format(const char* fmt, ...)
            {
                va_list al;
                va_start(al, fmt);
                format(fmt, al);
                va_end(al);
            }

            /**
             * @brief 格式化写入日志内容
             */
            void format(const char* fmt, va_list al)
            {
                char* buf = nullptr;
                int len = vasprintf(&buf, fmt, al);

                if (len != -1) {
                    m_ss << std::string(buf, len);
                    free(buf);
                }
            }
        private:
            /// 文件名
            const char* m_file = nullptr;
            /// 行号
            int32_t m_line = 0;
            /// 程序启动开始到现在的毫秒数
            uint32_t m_elapse = 0;
            /// 线程ID
            uint32_t m_threadId = 0;
            /// 协程ID
            uint32_t m_fiberId = 0;
            /// 时间戳
            uint64_t m_time = 0;
            /// 线程名称
            std::string m_threadName;
            /// 日志内容流
            std::stringstream m_ss;
            /// 日志器
            std::shared_ptr<Logger> m_logger;
            /// 日志等级
            LogLevel::Level m_level;
    };

    /**
     * @brief 日志格式化
     */
    class LogFormatter 
    {
        public:
            typedef std::shared_ptr<LogFormatter> ptr;
            /**
             * @brief 构造函数
             * @param[in] pattern 格式模板
             * @details 
             *  %m 消息
             *  %p 日志级别
             *  %r 累计毫秒数
             *  %c 日志名称
             *  %t 线程id
             *  %n 换行
             *  %d 时间
             *  %f 文件名
             *  %l 行号
             *  %T 制表符
             *  %F 协程id
             *  %N 线程名称
             *
             *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
             */
            LogFormatter(const std::string& pattern):m_pattern(pattern) 
            {
                init();
            }

            /**
             * @brief 返回格式化日志文本
             * @param[in] logger 日志器
             * @param[in] level 日志级别
             * @param[in] event 日志事件
             */
            std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
            {
                std::stringstream ss;

                for (auto& i : m_items) 
                {
                    i->format(ss, logger, level, event);
                }

                return ss.str();
            }

            std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event){
                for (auto& i : m_items) 
                {
                    i->format(ofs, logger, level, event);
                }

                return ofs;
            }
        public:

            /**
             * @brief 日志内容项格式化
             */
            class FormatItem 
            {
                public:
                    typedef std::shared_ptr<FormatItem> ptr;
                    /**
                     * @brief 析构函数
                     */
                    virtual ~FormatItem() {}
                    /**
                     * @brief 格式化日志到流
                     * @param[in, out] os 日志输出流
                     * @param[in] logger 日志器
                     * @param[in] level 日志等级
                     * @param[in] event 日志事件
                     */
                    virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
            };

            /**
             * @brief 初始化,解析日志模板
             */
            void init();

            /**
             * @brief 是否有错误
             */
            bool isError() const { return m_error;}

            /**
             * @brief 返回日志模板
             */
            const std::string getPattern() const { return m_pattern;}
        private:
            /// 日志格式模板
            std::string m_pattern;
            /// 日志格式解析后格式
            std::vector<FormatItem::ptr> m_items;
            /// 是否有错误
            bool m_error = false;
    };

    /**
     * @brief 日志输出目标
     */
    class LogAppender 
    {
        friend class Logger;

        public:
            typedef std::shared_ptr<LogAppender> ptr;
            //typedef Spinlock MutexType;

            /**
             * @brief 析构函数
             */
            virtual ~LogAppender() {}

            /**
             * @brief 写入日志
             * @param[in] logger 日志器
             * @param[in] level 日志级别
             * @param[in] event 日志事件
             */
            virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

            /**
             * @brief 将日志输出目标的配置转成YAML String
             */
            virtual std::string toYamlString() = 0;

            /**
             * @brief 更改日志格式器
             */
            void setFormatter(LogFormatter::ptr val)
            {
                //MutexType::Lock lock(m_mutex);
                m_formatter = val;
                if (m_formatter) {
                    m_hasFormatter = true;
                } else {
                    m_hasFormatter = false;
                }
            }

            /**
             * @brief 获取日志格式器
             */
            LogFormatter::ptr getFormatter()
            {
                //MutexType::Lock lock(m_mutex);
                return m_formatter;
            }

            /**
             * @brief 获取日志级别
             */
            LogLevel::Level getLevel() const { return m_level;}

            /**
             * @brief 设置日志级别
             */
            void setLevel(LogLevel::Level val) { m_level = val;}
        protected:
            /// 日志级别
            LogLevel::Level m_level = LogLevel::DEBUG;
            /// 是否有自己的日志格式器
            bool m_hasFormatter = false;
            /// Mutex
            //MutexType m_mutex;
            /// 日志格式器
            LogFormatter::ptr m_formatter;
    };

    /**
     * @brief 日志器
     */
    class Logger : public std::enable_shared_from_this<Logger> 
    {
        friend class LoggerManager;

        public:
            typedef std::shared_ptr<Logger> ptr;
            //typedef Spinlock MutexType;

            /**
             * @brief 构造函数
             * @param[in] name 日志器名称
             */
            Logger(const std::string& name = "root"):m_name(name) ,m_level(LogLevel::DEBUG) 
            { 
                m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
            }

            /**
             * @brief 写日志
             * @param[in] level 日志级别
             * @param[in] event 日志事件
             */
            void log(LogLevel::Level level, LogEvent::ptr event)
            {
                if (level >= m_level) {
                    auto self = shared_from_this();
                    //MutexType::Lock lock(m_mutex);
                    if (!m_appenders.empty()) {
                        for (auto& i : m_appenders) {
                            i->log(self, level, event);
                        }
                    } else if (m_root) {
                        m_root->log(level, event);
                    }
                }
            }

            /**
             * @brief 写debug级别日志
             * @param[in] event 日志事件
             */
            void debug(LogEvent::ptr event)
            {
                log(LogLevel::DEBUG, event);
            }
                

            /**
             * @brief 写info级别日志
             * @param[in] event 日志事件
             */
            void info(LogEvent::ptr event)
            {
                log(LogLevel::INFO, event);
            }

            /**
             * @brief 写warn级别日志
             * @param[in] event 日志事件
             */
            void warn(LogEvent::ptr event)
            {
                log(LogLevel::WARN, event);
            }

            /**
             * @brief 写error级别日志
             * @param[in] event 日志事件
             */
            void error(LogEvent::ptr event)
            {
                log(LogLevel::ERROR, event);
            }

            /**
             * @brief 写fatal级别日志
             * @param[in] event 日志事件
             */
            void fatal(LogEvent::ptr event)
            {
                log(LogLevel::FATAL, event);
            }

            /**
             * @brief 添加日志目标
             * @param[in] appender 日志目标
             */
            void addAppender(LogAppender::ptr appender)
            {
                //MutexType::Lock lock(m_mutex);
                if (!appender->getFormatter()) 
                {
                    //MutexType::Lock ll(appender->m_mutex);
                    appender->m_formatter = m_formatter;
                }

                m_appenders.push_back(appender);
            }

            /**
             * @brief 删除日志目标
             * @param[in] appender 日志目标
             */
            void delAppender(LogAppender::ptr appender)
            {
                //MutexType::Lock lock(m_mutex);
                for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it) 
                {
                    if (*it == appender) 
                    { 
                        m_appenders.erase(it);
                        break;
                    }
                }
            }

            /**
             * @brief 清空日志目标
             */
            void clearAppenders()
            {
                //MutexType::Lock lock(m_mutex);
                m_appenders.clear();
            }

            /**
             * @brief 返回日志级别
             */
            LogLevel::Level getLevel() const { return m_level;}

            /**
             * @brief 设置日志级别
             */
            void setLevel(LogLevel::Level val) { m_level = val;}

            /**
             * @brief 返回日志名称
             */
            const std::string& getName() const { return m_name;}

            /**
             * @brief 设置日志格式器
             */
            void setFormatter(LogFormatter::ptr val)
            {
                //MutexType::Lock lock(m_mutex);
                m_formatter = val;

                for (auto& i : m_appenders) 
                {
                    //MutexType::Lock ll(i->m_mutex);
                    if (!i->m_hasFormatter) 
                    {
                        i->m_formatter = m_formatter;
                    }
                }
            }

            /**
             * @brief 设置日志格式模板
             */
            void setFormatter(const std::string& val)
            {
                std::cout << "---" << val << std::endl;
                JerryFish::LogFormatter::ptr new_val(new JerryFish::LogFormatter(val));

                if (new_val->isError()) 
                {
                    std::cout << "Logger setFormatter name=" << m_name
                        << " value=" << val << " invalid formatter"
                        << std::endl;
                    return;
                }

                //m_formatter = new_val;
                setFormatter(new_val);
            }
            
            /**
             * @brief 获取日志格式器
             */
            LogFormatter::ptr getFormatter()
            {
                //MutexType::Lock lock(m_mutex);
                return m_formatter;
            }

            /**
             * @brief 将日志器的配置转成YAML String
             */
            std::string toYamlString()
            {
                //MutexType::Lock lock(m_mutex);
                YAML::Node node;
                node["name"] = m_name;
                if (m_level != LogLevel::UNKNOW) 
                {
                    node["level"] = LogLevel::ToString(m_level);
                }

                if (m_formatter) 
                {
                    node["formatter"] = m_formatter->getPattern();
                }

                for (auto& i : m_appenders) 
                {
                    node["appenders"].push_back(YAML::Load(i->toYamlString()));
                }

                std::stringstream ss;
                ss << node;
                return ss.str();
            }

        private:
            /// 日志名称
            std::string m_name;
            /// 日志级别
            LogLevel::Level m_level;
            /// Mutex
            //MutexType m_mutex;
            /// 日志目标集合
            std::list<LogAppender::ptr> m_appenders;
            /// 日志格式器
            LogFormatter::ptr m_formatter;
            /// 主日志器
            Logger::ptr m_root;
    };

    class MessageFormatItem : public LogFormatter::FormatItem 
    {
        public:
            MessageFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << event->getContent();
            }
    };

    class LevelFormatItem : public LogFormatter::FormatItem 
    {
        public:
            LevelFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << LogLevel::ToString(level);
            }
    };

    class ElapseFormatItem : public LogFormatter::FormatItem 
    {
        public:
            ElapseFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << event->getElapse();
            }
    };

    class NameFormatItem : public LogFormatter::FormatItem 
    {
        public:
            NameFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << event->getLogger()->getName();
            }
    };

    class ThreadIdFormatItem : public LogFormatter::FormatItem 
    {
        public:
            ThreadIdFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << event->getThreadId();
            }
    };

    class FiberIdFormatItem : public LogFormatter::FormatItem 
    {
        public:
            FiberIdFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << event->getFiberId();
            }
    };

    class ThreadNameFormatItem : public LogFormatter::FormatItem 
    {
        public:
            ThreadNameFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << event->getThreadName();
            }
    };

    class DateTimeFormatItem : public LogFormatter::FormatItem 
    {
        public:
            DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
                :m_format(format) {
                    if (m_format.empty()) {
                        m_format = "%Y-%m-%d %H:%M:%S";
                    }
                }

            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                struct tm tm;
                time_t time = event->getTime();
                localtime_r(&time, &tm);
                char buf[64];
                strftime(buf, sizeof(buf), m_format.c_str(), &tm);
                os << buf;
            }
        private:
            std::string m_format;
    };

    class FilenameFormatItem : public LogFormatter::FormatItem 
    {
        public:
            FilenameFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << event->getFile();
            }
    };

    class LineFormatItem : public LogFormatter::FormatItem {
        public:
            LineFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << event->getLine();
            }
    };

    class NewLineFormatItem : public LogFormatter::FormatItem 
    {
        public:
            NewLineFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << std::endl;
            }
    };


    class StringFormatItem : public LogFormatter::FormatItem 
    {
        public:
            StringFormatItem(const std::string& str)
                :m_string(str) {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << m_string;
            }
        private:
            std::string m_string;
    };

    class TabFormatItem : public LogFormatter::FormatItem 
    {
        public:
            TabFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
                os << "\t";
            }
        private:
            std::string m_string;
    };

    /**
     * @brief 日志事件包装器
     */
    class LogEventWrap 
    {
        public:

            /**
             * @brief 构造函数
             * @param[in] e 日志事件
             */
            LogEventWrap(LogEvent::ptr e):m_event(e) 
            {
            }

            /**
             * @brief 析构函数
             */
            ~LogEventWrap()
            { 
                m_event->getLogger()->log(m_event->getLevel(), m_event); 
            }

            /**
             * @brief 获取日志事件
             */
            LogEvent::ptr getEvent() const { return m_event;}

            /**
             * @brief 获取日志内容流
             */
            std::stringstream& getSS()
            { 
                return m_event->getSS(); 
            } 

        private:
            /**
             * @brief 日志事件
             */
            LogEvent::ptr m_event;
    };

    /**
     * @brief 输出到控制台的Appender
     */
    class StdoutLogAppender : public LogAppender 
    {
        public:
            typedef std::shared_ptr<StdoutLogAppender> ptr;

            void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
            {
                if (level >= m_level) {
                    //MutexType::Lock lock(m_mutex);
                    m_formatter->format(std::cout, logger, level, event);
                }
            }

            std::string toYamlString() override
            {
                //MutexType::Lock lock(m_mutex);
                YAML::Node node;
                node["type"] = "StdoutLogAppender";
                if (m_level != LogLevel::UNKNOW) 
                {
                    node["level"] = LogLevel::ToString(m_level);
                }

                if (m_hasFormatter && m_formatter) 
                {
                    node["formatter"] = m_formatter->getPattern();
                }

                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };

    /**
     * @brief 输出到文件的Appender
     */
    class FileLogAppender : public LogAppender 
    {
        public:
            typedef std::shared_ptr<FileLogAppender> ptr;

            FileLogAppender(const std::string& filename):m_filename(filename) 
            {
                reopen();
            }

            void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
            {
                if (level >= m_level) 
                {
                    uint64_t now = event->getTime();
                    if (now >= (m_lastTime + 3)) 
                    {
                        reopen();
                        m_lastTime = now;
                    }

                    //MutexType::Lock lock(m_mutex);

                    if (!m_formatter->format(m_filestream, logger, level, event)) 
                    {
                        std::cout << "error" << std::endl;
                    }
                }
            }

            std::string toYamlString() override
            {
                //MutexType::Lock lock(m_mutex);
                YAML::Node node;
                node["type"] = "FileLogAppender";
                node["file"] = m_filename;

                if (m_level != LogLevel::UNKNOW) 
                {
                    node["level"] = LogLevel::ToString(m_level);
                }

                if (m_hasFormatter && m_formatter) 
                {
                    node["formatter"] = m_formatter->getPattern();
                }

                std::stringstream ss;
                ss << node;
                return ss.str();
            }

            /**
             * @brief 重新打开日志文件
             * @return 成功返回true
             */
            bool reopen()
            {
                //MutexType::Lock lock(m_mutex);
                if (m_filestream) 
                {
                    m_filestream.close();
                }

                //return FSUtil::OpenForWrite(m_filestream, m_filename, std::ios::app);
                m_filestream.open(m_filename);
                return !m_filestream;
            }
        private:
            /// 文件路径
            std::string m_filename;
            /// 文件流
            std::ofstream m_filestream;
            /// 上次重新打开时间
            uint64_t m_lastTime = 0;
    };

    /**
     * @brief 日志器管理类
     */
    class LoggerManager 
    {
        public:
            //typedef Spinlock MutexType;
            /**
             * @brief 构造函数
             */
            LoggerManager()
            {
                m_root.reset(new Logger);
                m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));

                std::cout << "LoggerManager.m_root->name: " << m_root->m_name << std::endl; 
                m_loggers[m_root->m_name] = m_root;

                init();
            }

            /**
             * @brief 获取日志器
             * @param[in] name 日志器名称
             */
            Logger::ptr getLogger(const std::string& name)
            {
                //MutexType::Lock lock(m_mutex);
                auto it = m_loggers.find(name);
                if (it != m_loggers.end()) 
                {
                    return it->second;
                }

                Logger::ptr logger(new Logger(name));
                logger->m_root = m_root;
                m_loggers[name] = logger;
                return logger;
            }

            /**
             * @brief 初始化
             */
            void init() {}

            /**
             * @brief 返回主日志器
             */
            Logger::ptr getRoot() const { return m_root;}

            /**
             * @brief 将所有的日志器配置转成YAML String
             */
            std::string toYamlString()
            {
                //MutexType::Lock lock(m_mutex);
                YAML::Node node;

                for (auto& i : m_loggers) 
                {
                    node.push_back(YAML::Load(i.second->toYamlString()));
                }

                std::stringstream ss;
                ss << node;
                return ss.str();
            }
        private:
            /// Mutex
            //MutexType m_mutex;
            /// 日志器容器
            std::map<std::string, Logger::ptr> m_loggers;
            /// 主日志器
            Logger::ptr m_root;
    };

    /// 日志器管理类单例模式
    typedef JerryFish::Singleton<LoggerManager> LoggerMgr;

}

#endif
