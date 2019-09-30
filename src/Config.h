/*
 * Config.h
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <sys/stat.h>

#include "Logger.h"
#include "Utils.h"
#include "Debugger.h"


namespace JerryFish {

    /**
     * @brief 配置变量的基类
     */
    class ConfigVarBase {
        public:
            typedef std::shared_ptr<ConfigVarBase> ptr;
            /**
             * @brief 构造函数
             * @param[in] name 配置参数名称[0-9a-z_.]
             * @param[in] description 配置参数描述
             */
            ConfigVarBase(const std::string& name, const std::string& description = "")
                :m_name(name)
                ,m_description(description) {
                    std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
                }

            /**
             * @brief 析构函数
             */
            virtual ~ConfigVarBase() {}

            /**
             * @brief 返回配置参数名称
             */
            const std::string& getName() const { return m_name;}

            /**
             * @brief 返回配置参数的描述
             */
            const std::string& getDescription() const { return m_description;}

            /**
             * @brief 转成字符串
             */
            virtual std::string toString() = 0;

            /**
             * @brief 从字符串初始化值
             */
            virtual bool fromString(const std::string& val) = 0;

            /**
             * @brief 返回配置参数值的类型名称
             */
            virtual std::string getTypeName() const = 0;
        protected:
            /// 配置参数的名称
            std::string m_name;
            /// 配置参数的描述
            std::string m_description;
    };

    /**
     * @brief 类型转换模板类(F 源类型, T 目标类型)
     */
    template<class F, class T>
    class LexicalCast 
    {
        public:
            /**
             * @brief 类型转换
             * @param[in] v 源类型值
             * @return 返回v转换后的目标类型
             * @exception 当类型不可转换时抛出异常
             */
            T operator()(const F& v) 
            {
                return boost::lexical_cast<T>(v);
            }
    };

    /**
     * @brief 类型转换模板类片特化(YAML String 转换成 std::vector<T>)
     */
    template<class T>
    class LexicalCast<std::string, std::vector<T> > 
    {
        public:
            std::vector<T> operator()(const std::string& v) 
            {
                YAML::Node node = YAML::Load(v);
                typename std::vector<T> vec;
                std::stringstream ss;
                for (size_t i = 0; i < node.size(); ++i) 
                {
                    ss.str("");
                    ss << node[i];
                    vec.push_back(LexicalCast<std::string, T>()(ss.str()));
                }

                return vec;
            }
    };

    /**
     * @brief 类型转换模板类片特化(std::vector<T> 转换成 YAML String)
     */
    template<class T>
    class LexicalCast<std::vector<T>, std::string> 
    {
        public:
            std::string operator()(const std::vector<T>& v) 
            {
                YAML::Node node(YAML::NodeType::Sequence);
                for (auto& i : v) 
                {
                    node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
                }
                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };

    /**
     * @brief 类型转换模板类片特化(YAML String 转换成 std::list<T>)
     */
    template<class T>
    class LexicalCast<std::string, std::list<T> > 
    {
        public:
            std::list<T> operator()(const std::string& v) 
            {
                YAML::Node node = YAML::Load(v);
                typename std::list<T> vec;
                std::stringstream ss;
                for (size_t i = 0; i < node.size(); ++i) 
                {
                    ss.str("");
                    ss << node[i];
                    vec.push_back(LexicalCast<std::string, T>()(ss.str()));
                }
                return vec;
            }
    };

    /**
     * @brief 类型转换模板类片特化(std::list<T> 转换成 YAML String)
     */
    template<class T>
    class LexicalCast<std::list<T>, std::string> 
    {
        public:
            std::string operator()(const std::list<T>& v) 
            {
                YAML::Node node(YAML::NodeType::Sequence);
                for (auto& i : v) 
                {
                    node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
                }
                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };

    /**
     * @brief 类型转换模板类片特化(YAML String 转换成 std::set<T>)
     */
    template<class T>
    class LexicalCast<std::string, std::set<T> > 
    {
        public:
            std::set<T> operator()(const std::string& v) 
            {
                YAML::Node node = YAML::Load(v);
                typename std::set<T> vec;
                std::stringstream ss;
                for (size_t i = 0; i < node.size(); ++i) 
                {
                    ss.str("");
                    ss << node[i];
                    vec.insert(LexicalCast<std::string, T>()(ss.str()));
                }
                return vec;
            }
    };

    /**
     * @brief 类型转换模板类片特化(std::set<T> 转换成 YAML String)
     */
    template<class T>
    class LexicalCast<std::set<T>, std::string> 
    {
        public:
            std::string operator()(const std::set<T>& v) 
            {
                YAML::Node node(YAML::NodeType::Sequence);
                for (auto& i : v) 
                {
                    node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
                }
                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };

    /**
     * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_set<T>)
     */
    template<class T>
    class LexicalCast<std::string, std::unordered_set<T> > 
    {
        public:
            std::unordered_set<T> operator()(const std::string& v) 
            {
                YAML::Node node = YAML::Load(v);
                typename std::unordered_set<T> vec;
                std::stringstream ss;
                for (size_t i = 0; i < node.size(); ++i) 
                {
                    ss.str("");
                    ss << node[i];
                    vec.insert(LexicalCast<std::string, T>()(ss.str()));
                }
                return vec;
            }
    };

    /**
     * @brief 类型转换模板类片特化(std::unordered_set<T> 转换成 YAML String)
     */
    template<class T>
    class LexicalCast<std::unordered_set<T>, std::string>
    {
        public:
            std::string operator()(const std::unordered_set<T>& v) 
            {
                YAML::Node node(YAML::NodeType::Sequence);
                for (auto& i : v) 
                {
                    node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
                }
                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };

    /**
     * @brief 类型转换模板类片特化(YAML String 转换成 std::map<std::string, T>)
     */
    template<class T>
    class LexicalCast<std::string, std::map<std::string, T> > 
    {
        public:
            std::map<std::string, T> operator()(const std::string& v) {
                YAML::Node node = YAML::Load(v);
                typename std::map<std::string, T> vec;
                std::stringstream ss;
                for (auto it = node.begin(); it != node.end(); ++it) 
                {
                    ss.str("");
                    ss << it->second;
                    vec.insert(std::make_pair(it->first.Scalar(), LexicalCast<std::string, T>()(ss.str())));
                }
                return vec;
            }
    };

    /**
     * @brief 类型转换模板类片特化(std::map<std::string, T> 转换成 YAML String)
     */
    template<class T>
    class LexicalCast<std::map<std::string, T>, std::string> 
    {
        public:
            std::string operator()(const std::map<std::string, T>& v) 
            {
                YAML::Node node(YAML::NodeType::Map);
                for (auto& i : v) 
                {
                    node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
                }
                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };

    /**
     * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_map<std::string, T>)
     */
    template<class T>
    class LexicalCast<std::string, std::unordered_map<std::string, T> > 
    {
        public:
            std::unordered_map<std::string, T> operator()(const std::string& v) 
            {
                YAML::Node node = YAML::Load(v);
                typename std::unordered_map<std::string, T> vec;
                std::stringstream ss;
                for (auto it = node.begin(); it != node.end(); ++it) 
                {
                    ss.str("");
                    ss << it->second;
                    vec.insert(std::make_pair(it->first.Scalar(), LexicalCast<std::string, T>()(ss.str())));
                }
                return vec;
            }
    };

    /**
     * @brief 类型转换模板类片特化(std::unordered_map<std::string, T> 转换成 YAML String)
     */
    template<class T>
    class LexicalCast<std::unordered_map<std::string, T>, std::string> 
    {
        public:
            std::string operator()(const std::unordered_map<std::string, T>& v) 
            {
                YAML::Node node(YAML::NodeType::Map);
                for (auto& i : v) 
                {
                    node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
                }
                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };


    struct LogAppenderDefine 
    {
        int type = 0; //1 File, 2 Stdout
        LogLevel::Level level = LogLevel::UNKNOW;
        std::string formatter;
        std::string file;

        bool operator==(const LogAppenderDefine& oth) const 
        {
            return type == oth.type
                && level == oth.level
                && formatter == oth.formatter
                && file == oth.file;
        }
    };

    struct LogDefine 
    {
        std::string name;
        LogLevel::Level level = LogLevel::UNKNOW;
        std::string formatter;
        std::vector<LogAppenderDefine> appenders;

        bool operator==(const LogDefine& oth) const 
        {
            return name == oth.name
                && level == oth.level
                && formatter == oth.formatter
                && appenders == appenders;
        }

        bool operator<(const LogDefine& oth) const 
        {
            return name < oth.name;
        }

        bool isValid() const 
        {
            return !name.empty();
        }
    };

    template<>
    class LexicalCast<std::string, LogDefine> 
    {
        public:
            LogDefine operator()(const std::string& v) 
            {
                YAML::Node n = YAML::Load(v);
                LogDefine ld;

                if (!n["name"].IsDefined()) 
                {
                    std::cout << "log config error: name is null, " << n
                        << std::endl;
                    throw std::logic_error("log config name is null");
                }
                ld.name = n["name"].as<std::string>();
                ld.level = LogLevel::FromString(n["level"].IsDefined() ? n["level"].as<std::string>() : "");
                if (n["formatter"].IsDefined()) 
                {
                    ld.formatter = n["formatter"].as<std::string>();
                }

                if (n["appenders"].IsDefined()) 
                {
                    //std::cout << "==" << ld.name << " = " << n["appenders"].size() << std::endl;
                    for (size_t x = 0; x < n["appenders"].size(); ++x) 
                    {
                        auto a = n["appenders"][x];
                        if (!a["type"].IsDefined()) 
                        {
                            std::cout << "log config error: appender type is null, " << a
                                << std::endl;
                            continue;
                        }
                        std::string type = a["type"].as<std::string>();
                        LogAppenderDefine lad;
                        if (type == "FileLogAppender") 
                        {
                            lad.type = 1;
                            if (!a["file"].IsDefined()) 
                            {
                                std::cout << "log config error: fileappender file is null, " << a
                                    << std::endl;
                                continue;
                            }
                            lad.file = a["file"].as<std::string>();
                            if (a["formatter"].IsDefined()) 
                            {
                                lad.formatter = a["formatter"].as<std::string>();
                            }
                        } 
                        else if (type == "StdoutLogAppender") 
                        {
                            lad.type = 2;
                            if (a["formatter"].IsDefined()) 
                            {
                                lad.formatter = a["formatter"].as<std::string>();
                            }
                        } 
                        else 
                        {
                            std::cout << "log config error: appender type is invalid, " << a
                                << std::endl;
                            continue;
                        }

                        ld.appenders.push_back(lad);
                    }
                }
                return ld;
            }
    };

    template<>
    class LexicalCast<LogDefine, std::string> 
    {
        public:
            std::string operator()(const LogDefine& i) 
            {
                YAML::Node n;
                n["name"] = i.name;

                if (i.level != LogLevel::UNKNOW) 
                {
                    n["level"] = LogLevel::ToString(i.level);
                }
                if (!i.formatter.empty()) 
                {
                    n["formatter"] = i.formatter;
                }

                for (auto& a : i.appenders) 
                {
                    YAML::Node na;
                    if (a.type == 1) 
                    {
                        na["type"] = "FileLogAppender";
                        na["file"] = a.file;
                    } 
                    else if (a.type == 2) 
                    {
                        na["type"] = "StdoutLogAppender";
                    }
                    if (a.level != LogLevel::UNKNOW) 
                    {
                        na["level"] = LogLevel::ToString(a.level);
                    }

                    if (!a.formatter.empty()) 
                    {
                        na["formatter"] = a.formatter;
                    }

                    n["appenders"].push_back(na);
                }
                std::stringstream ss;
                ss << n;
                return ss.str();
            }
    };


    /**
     * @brief 类型转换模板类片特化(YAML String 转换成 std::vector<T>)
     */
    template<>
    class LexicalCast<std::string, std::set<LogDefine>> 
    {
        public:
            std::set<LogDefine> operator()(const std::string& v) 
            {
                YAML::Node node = YAML::Load(v);
                std::set<LogDefine> vec;

                for (size_t i = 0; i < node.size(); ++i) 
                {
                    auto n = node[i];

                    if (!n["name"].IsDefined())
                    {
                        std::cout << "log config error: name is null, " << n << std::endl;
                        continue;
                    }

                    LogDefine ld;
                    ld.name = n["name"].as<std::string>();
                    ld.level = LogLevel::FromString(n["level"].IsDefined() ? n["level"].as<std::string>() : "");

                    if (n["formatter"].IsDefined())
                    {
                        ld.formatter = n["formatter"].as<std::string>();
                    }

                    if (n["appenders"].IsDefined())
                    {
                        for (size_t j = 0; j < n["appenders"].size(); ++j)
                        {
                            auto a = n["appenders"][i];

                            if (!a["type"].IsDefined())
                            {
                                std::cout << "log config error: appender tyep is null, " << a << std::endl;
                                continue;
                            }

                            std::string type = a["type"].as<std::string>();
                            LogAppenderDefine lad;

                            if (type == "FileLogAppender")
                            {
                                lad.type = 1;

                                if (!a["file"].IsDefined())
                                {
                                    std::cout << "log config error: fileappender tyep is null, " << a << std::endl;
                                    continue;
                                }

                                lad.file = a["file"].as<std::string>();

                                if (a["formatter"].IsDefined())
                                {
                                    lad.formatter = a["formatter"].as<std::string>();
                                }
                            }
                            else if (type == "StdoutAppender")
                            {
                                lad.type = 2;
                            }
                            else
                            {
                                std::cout << "log config error: appender tyep is null, " << a << std::endl;
                                continue;
                            }

                            ld.appenders.push_back(lad);
                        }
                    }

                    vec.insert(ld);
                }

                return vec;
            }
    };

    /**
     * @brief 类型转换模板类片特化(std::vector<T> 转换成 YAML String)
     */
    template<>
    class LexicalCast<std::set<LogDefine>, std::string> 
    {
        public:
            std::string operator()(const std::set<LogDefine>& v) 
            {
                YAML::Node node(YAML::NodeType::Sequence);

                for (auto& i : v) 
                {
                    YAML::Node n;
                    n["name"] = i.name;
                    n["level"] = LogLevel::ToString(i.level);

                    if (i.formatter.empty())
                    {
                        n["level"] = i.formatter;
                    }

                    for (auto& a: i.appenders)
                    {
                        YAML::Node na;

                        if (a.type == 1)
                        {
                            na["type"] = "FileLogAppender";
                            na["file"] = a.file;
                        }
                        else if (a.type == 2)
                        {
                            na["type"] = "StdoutAppender";
                        }

                        na["level"] = LogLevel::ToString(a.level);

                        if (!a.formatter.empty())
                        {
                            na["formatter"] = a.formatter;
                        }

                        n["appenders"].push_back(na);
                    }

                    node.push_back(n);
                }

                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };
    /**
     * @brief 配置参数模板子类,保存对应类型的参数值
     * @details T 参数的具体类型
     *          FromStr 从std::string转换成T类型的仿函数
     *          ToStr 从T转换成std::string的仿函数
     *          std::string 为YAML格式的字符串
     */
    template<class T, class FromStr = LexicalCast<std::string, T>
        ,class ToStr = LexicalCast<T, std::string> >
    class ConfigVar : public ConfigVarBase 
    {
        public:
            ////typedef RWMutex RWMutexType;
            typedef std::shared_ptr<ConfigVar> ptr;
            typedef std::function<void (const T& old_value, const T& new_value)> on_change_cb;

            /**
             * @brief 通过参数名,参数值,描述构造ConfigVar
             * @param[in] name 参数名称有效字符为[0-9a-z_.]
             * @param[in] default_value 参数的默认值
             * @param[in] description 参数的描述
             */
            ConfigVar(const std::string& name
                    ,const T& default_value
                    ,const std::string& description = "")
                :ConfigVarBase(name, description)
                ,m_val(default_value) {
                }

            /**
             * @brief 将参数值转换成YAML String
             * @exception 当转换失败抛出异常
             */
            std::string toString() override {
                try {
                    //return boost::lexical_cast<std::string>(m_val);
                    ////RWMutexType::ReadLock lock(m_mutex);
                    return ToStr()(m_val);
                } catch (std::exception& e) {
                    JERRYFISH_LOG_ERROR(JERRYFISH_LOG_ROOT()) << "ConfigVar::toString exception "
                        << e.what() << " convert: " << TypeToName<T>() << " to string"
                        << " name=" << m_name;
                }
                return "";
            }

            /**
             * @brief 从YAML String 转成参数的值
             * @exception 当转换失败抛出异常
             */
            bool fromString(const std::string& val) override {
                try {
                    setValue(FromStr()(val));
                } catch (std::exception& e) {
                    JERRYFISH_LOG_ERROR(JERRYFISH_LOG_ROOT()) << "ConfigVar::fromString exception "
                        << e.what() << " convert: string to " << TypeToName<T>()
                        << " name=" << m_name
                        << " - " << val;
                }
                return false;
            }

            /**
             * @brief 获取当前参数的值
             */
            const T getValue() 
            {
                ////RWMutexType::ReadLock lock(m_mutex);
                return m_val;
            }

            /**
             * @brief 设置当前参数的值
             * @details 如果参数的值有发生变化,则通知对应的注册回调函数
             */
            void setValue(const T& v) 
            {
                {
                    ////RWMutexType::ReadLock lock(m_mutex);
                    if (v == m_val) 
                    {
                        return;
                    }

                    for (auto& i : m_cbs) 
                    {
                        i.second(m_val, v);
                    }
                }
                ////RWMutexType::WriteLock lock(m_mutex);
                m_val = v;
            }

            /**
             * @brief 返回参数值的类型名称(typeinfo)
             */
            std::string getTypeName() const override { return TypeToName<T>();}

            /**
             * @brief 添加变化回调函数
             * @return 返回该回调函数对应的唯一id,用于删除回调
             */
            uint64_t addListener(on_change_cb cb) 
            {
                static uint64_t s_fun_id = 0;
                ////RWMutexType::WriteLock lock(m_mutex);
                ++s_fun_id;
                m_cbs[s_fun_id] = cb;
                return s_fun_id;
            }

            /**
             * @brief 删除回调函数
             * @param[in] key 回调函数的唯一id
             */
            void delListener(uint64_t key) 
            {
                ////RWMutexType::WriteLock lock(m_mutex);
                m_cbs.erase(key);
            }

            /**
             * @brief 获取回调函数
             * @param[in] key 回调函数的唯一id
             * @return 如果存在返回对应的回调函数,否则返回nullptr
             */
            on_change_cb getListener(uint64_t key) 
            {
                ////RWMutexType::ReadLock lock(m_mutex);
                auto it = m_cbs.find(key);
                return it == m_cbs.end() ? nullptr : it->second;
            }

            /**
             * @brief 清理所有的回调函数
             */
            void clearListener() 
            {
                ////RWMutexType::WriteLock lock(m_mutex);
                m_cbs.clear();
            }
        private:
            ////RWMutexType m_mutex;
            T m_val;
            //变更回调函数组, uint64_t key,要求唯一，一般可以用hash
            std::map<uint64_t, on_change_cb> m_cbs;
    };

    /**
     * @brief ConfigVar的管理类
     * @details 提供便捷的方法创建/访问ConfigVar
     */
    class Config {
        public:
            typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;
            ////typedef RWMutex RWMutexType;

            /**
             * @brief 获取/创建对应参数名的配置参数
             * @param[in] name 配置参数名称
             * @param[in] default_value 参数默认值
             * @param[in] description 参数描述
             * @details 获取参数名为name的配置参数,如果存在直接返回
             *          如果不存在,创建参数配置并用default_value赋值
             * @return 返回对应的配置参数,如果参数名存在但是类型不匹配则返回nullptr
             * @exception 如果参数名包含非法字符[^0-9a-z_.] 抛出异常 std::invalid_argument
             */
            template<class T>
            static typename ConfigVar<T>::ptr Lookup(const std::string& name,
                    const T& default_value, const std::string& description = "") 
            {
                ////RWMutexType::WriteLock lock(GetMutex());
                auto it = GetDatas().find(name);

                if (it != GetDatas().end()) 
                {
                    auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
                    if (tmp) 
                    {
                        JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "Lookup name=" << name << " exists";
                        return tmp;
                    } 
                    else 
                    {
                        JERRYFISH_LOG_ERROR(JERRYFISH_LOG_ROOT()) << "Lookup name=" << name << " exists but type not "
                            << TypeToName<T>() << " real_type=" << it->second->getTypeName()
                            << " " << it->second->toString();
                        return nullptr;
                    }
                }

                if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678")
                        != std::string::npos) 
                {
                    JERRYFISH_LOG_ERROR(JERRYFISH_LOG_ROOT()) << "Lookup name invalid " << name;
                    throw std::invalid_argument(name);
                }

                typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
                GetDatas()[name] = v;
                return v;
            }

            /**
             * @brief 查找配置参数
             * @param[in] name 配置参数名称
             * @return 返回配置参数名为name的配置参数
             */
            template<class T>
            static typename ConfigVar<T>::ptr Lookup(const std::string& name) 
            {
                ////RWMutexType::ReadLock lock(GetMutex());
                auto it = GetDatas().find(name);

                if (it == GetDatas().end()) 
                {
                    return nullptr;
                }

                return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
            }

            /**
             * @brief 使用YAML::Node初始化配置模块
             */
            static void LoadFromYaml(const YAML::Node& root)
            {
                std::list<std::pair<std::string, const YAML::Node> > all_nodes;
                ListAllMember("", root, all_nodes);

                for (auto& i : all_nodes) 
                {
                    std::string key = i.first;

                    if (key.empty()) 
                    {
                        continue;
                    }

                    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
                    ConfigVarBase::ptr var = LookupBase(key);

                    if (var) 
                    {
                        if (i.second.IsScalar()) 
                        {
                            var->fromString(i.second.Scalar());
                        } 
                        else 
                        {
                            std::stringstream ss;
                            ss << i.second;
                            var->fromString(ss.str());
                        }
                    }
                }
            }

            /**
             * @brief 加载path文件夹里面的配置文件
             */
            //static void LoadFromConfDir(const std::string& path, bool force = false){
            //std::string absoulte_path = JerryFish::EnvMgr::GetInstance()->getAbsolutePath(path);
            //std::vector<std::string> files;
            //FSUtil::ListAllFile(files, absoulte_path, ".yml");

            //for (auto& i : files) {
            //{
            //struct stat st;
            //lstat(i.c_str(), &st);
            ////JerryFish::Mutex::Lock lock(s_mutex);
            //if (!force && s_file2modifytime[i] == (uint64_t)st.st_mtime) {
            //continue;
            //}
            //s_file2modifytime[i] = st.st_mtime;
            //}
            //try {
            //YAML::Node root = YAML::LoadFile(i);
            //LoadFromYaml(root);
            //JERRYFISH_LOG_INFO(g_logger) << "LoadConfFile file="
            //<< i << " ok";
            //} catch (...) {
            //JERRYFISH_LOG_ERROR(g_logger) << "LoadConfFile file="
            //<< i << " failed";
            //}
            //}
            //}

            /**
             * @brief 查找配置参数,返回配置参数的基类
             * @param[in] name 配置参数名称
             */
            static ConfigVarBase::ptr LookupBase(const std::string& name)
            {
                ////RWMutexType::ReadLock lock(GetMutex());
                auto it = GetDatas().find(name);
                return it == GetDatas().end() ? nullptr : it->second;
            }

            /**
             * @brief 遍历配置模块里面所有配置项
             * @param[in] cb 配置项回调函数
             */
            static void Visit(std::function<void(ConfigVarBase::ptr)> cb)
            {
                ////RWMutexType::ReadLock lock(GetMutex());
                ConfigVarMap& m = GetDatas();

                for (auto it = m.begin(); it != m.end(); ++it) 
                {
                    cb(it->second);
                }
            }
        private:

            /**
             * @brief 返回所有的配置项
             */
            static ConfigVarMap& GetDatas() 
            {
                static ConfigVarMap s_datas;
                return s_datas;
            }

            /**
             * @brief 配置项的RWMutex
             */
            //static RWMutexType& GetMutex() {
            ////static RWMutexType s_mutex;
            //return s_mutex;
            //}

            static void ListAllMember(const std::string& prefix,
                    const YAML::Node& node,
                    std::list<std::pair<std::string, const YAML::Node> >& output) 
            {
                if (prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678") != std::string::npos) 
                {
                    JERRYFISH_LOG_ERROR(getLogger()) << "Config invalid name: " << prefix << " : " << node;
                    return;
                }

                output.push_back(std::make_pair(prefix, node));

                if (node.IsMap()) 
                {
                    for (auto it = node.begin(); it != node.end(); ++it) 
                    {
                        ListAllMember(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(), it->second, output);
                    }
                }
            }

            static JerryFish::Logger::ptr getLogger()
            {
                static JerryFish::Logger::ptr g_logger = JERRYFISH_LOG_NAME("system");
                return g_logger;
            }

            static std::map<std::string, uint64_t> getFile2ModifyTime()
            {
                static std::map<std::string, uint64_t> s_file2modifytime;
                return s_file2modifytime;
            }

            //static JerryFish::Mutex getMutex()
            //{
            //static JerryFish::Mutex s_mutex;
            //return s_mutex;
            //}
    };

    static ConfigVar<std::set<LogDefine> >::ptr getLogDefines()
    {
        static ConfigVar<std::set<LogDefine> >::ptr g_log_defines = JerryFish::Config::Lookup("logs", std::set<LogDefine>(), "logs config");
        return g_log_defines;
    }

    struct LogIniter 
    {
        LogIniter() 
        {
            getLogDefines()->addListener([](const std::set<LogDefine>& old_value, const std::set<LogDefine>& new_value)
            {
                    JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "on_logger_conf_changed";

                    for (auto& i : new_value) 
                    {
                        auto it = old_value.find(i);
                        JerryFish::Logger::ptr logger;

                        if (it == old_value.end()) 
                        {
                            logger = JERRYFISH_LOG_NAME(i.name);
                        } 
                        else 
                        {
                            if (!(i == *it))
                            {
                                //修改的logger
                                logger = JERRYFISH_LOG_NAME(i.name);
                            } 
                            else 
                            {
                                continue;
                            }
                        }

                        logger->setLevel(i.level);

                        if (!i.formatter.empty()) 
                        {
                            logger->setFormatter(i.formatter);
                        }

                        logger->clearAppenders();

                        for (auto& a : i.appenders) 
                        {
                            JerryFish::LogAppender::ptr ap;

                            if (a.type == 1) 
                            {
                                ap.reset(new FileLogAppender(a.file));
                            } 
                            else if (a.type == 2) 
                            {
                                ap.reset(new StdoutLogAppender);

                                //if (!JerryFish::EnvMgr::GetInstance()->has("d")) 
                                //{
                                    //ap.reset(new StdoutLogAppender);
                                //} 
                                //else 
                                //{
                                    //continue;
                                //}
                            }

                            ap->setLevel(a.level);

                            if (!a.formatter.empty()) 
                            {
                                LogFormatter::ptr fmt(new LogFormatter(a.formatter));

                                if (!fmt->isError()) 
                                {
                                    ap->setFormatter(fmt);
                                } 
                                else 
                                {
                                    std::cout << "log.name=" << i.name << " appender type=" << a.type
                                        << " formatter=" << a.formatter << " is invalid" << std::endl;
                                }
                            }

                            logger->addAppender(ap);
                        }
                    }

                    for (auto& i : old_value) 
                    {
                        auto it = new_value.find(i);
                        if (it == new_value.end()) 
                        {
                            //删除logger
                            auto logger = JERRYFISH_LOG_NAME(i.name);
                            logger->setLevel((LogLevel::Level)0);
                            logger->clearAppenders();
                        }
                    }
            });
        }
    };

    static JerryFish::LogIniter getLogInit()
    {
        static JerryFish::LogIniter __log_init;
        return __log_init;
    }
}

#endif /* !CONFIG_H */
