/*
 * test_config.cc
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "src/Config.h"
#include "src/Logger.h"
#include "yaml-cpp/yaml.h"

//JerryFish::ConfigVar<int>::ptr g_int_value_config = JerryFish::Config::Lookup("system.port", (int)8080, "system port");
//JerryFish::ConfigVar<float>::ptr g_float_value_config = JerryFish::Config::Lookup("system.value", (float)2.75, "system value");
//JerryFish::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config = JerryFish::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vec");

//void print_yaml(const YAML::Node& node, int level) 
//{
    //if(node.IsScalar()) {
        //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << std::string(level * 4, ' ')
            //<< node.Scalar() << " - " << node.Type() << " - " << level;
    //} else if(node.IsNull()) {
        //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << std::string(level * 4, ' ')
            //<< "NULL - " << node.Type() << " - " << level;
    //} else if(node.IsMap()) {
        //for(auto it = node.begin();
                //it != node.end(); ++it) {
            //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << std::string(level * 4, ' ')
                    //<< it->first << " - " << it->second.Type() << " - " << level;
            //print_yaml(it->second, level + 1);
        //}
    //} else if(node.IsSequence()) {
        //for(size_t i = 0; i < node.size(); ++i) {
            //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << std::string(level * 4, ' ')
                //<< i << " - " << node[i].Type() << " - " << level;
            //print_yaml(node[i], level + 1);
        //}
    //}
//}

//void test_config()
//{
    //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
    //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "after: " << g_float_value_config->toString();

    //auto v = g_int_vec_value_config->getValue();

    //for (auto& i : v)
    //{
        //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "before int_vec: " << i;
    //}

    //YAML::Node root = YAML::LoadFile("conf/log.yml");
    //JerryFish::Config::LoadFromYaml(root);

    //v = g_int_vec_value_config->getValue();

    //for (auto& i : v)
    //{
        //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "after int_vec: " << i;
    //}

    //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
    //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "after: " << g_float_value_config->toString();
//}

//void test_yaml()
//{
    //YAML::Node root = YAML::LoadFile("conf/log.yml");
    ////JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << root;
    //print_yaml(root, 0);
//}


class Person
{
public:
    Person() {}
    std::string m_name;
    int m_age = 0;
    bool m_sex = 0;

    std::string toString() const
    {
        std::stringstream ss;
        ss << "[Person name=" << m_name
            << " age=" << m_age
            << " sex=" << m_sex
            << "]";
        return ss.str();
    }

    bool operator==(const Person& a) const
    {
        return m_name == a.m_name && m_age == a.m_age && m_sex == a.m_sex;
    }
};

namespace JerryFish
{
    template<>
    class LexicalCast<std::string, Person>
    {
        public:
            Person operator()(const std::string& v)
            {
                YAML::Node node = YAML::Load(v);
                Person p;
                p.m_name = node["name"].as<std::string>();
                p.m_age = node["age"].as<int>();
                p.m_sex = node["sex"].as<bool>();
                return p;
            }
    };

    template<>
    class LexicalCast<Person, std::string>
    {
        public:
            std::string operator()(const Person& p)
            {
                YAML::Node node;
                node["name"] = p.m_name;
                node["age"] = p.m_age;
                node["sex"] = p.m_sex;
                std::stringstream ss;
                ss << node;
                return ss.str();
            }
    };
}

//JerryFish::ConfigVar<Person>::ptr g_person = JerryFish::Config::Lookup("class.person", Person(), "system person");

//JerryFish::ConfigVar<std::map<std::string, Person>>::ptr g_person_map = JerryFish::Config::Lookup("class.map", std::map<std::string, Person>(), "system person");

//JerryFish::ConfigVar<std::map<std::string, std::vector<Person>>>::ptr g_person_vec_map = JerryFish::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person>>(), "system person");

//void test_class()
//{
    //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "before: " << g_person->getValue().toString() << " - " << g_person->toString();


//#define XX_PM(g_var, prefix) \
    //{\
        //auto m = g_person_map->getValue(); \
        //for (auto& i: m)\
        //{\
            //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << prefix << ": " << i.first << " - " << i.second.toString();\
        //}\
    //}

    //g_person->addListener([](const Person& o, const Person& n){
        
        //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "old_value = " << o.toString()
        //<< "new_value = " << n.toString();
    //});

    //XX_PM(g_person_map, "class.map before")
    //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "before: " << g_person_vec_map->toString();
    //YAML::Node root = YAML::LoadFile("conf/log.yml");
    //JerryFish::Config::LoadFromYaml(root);

    //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "after: " << g_person->getValue().toString() << " - " << g_person->toString();
    //XX_PM(g_person_map, "class.map after")
    //JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << "after: " << g_person_vec_map->toString();
//}

void test_log()
{
    static JerryFish::Logger::ptr system_log = JERRYFISH_LOG_NAME("system");
    JERRYFISH_LOG_INFO(system_log) << "hello system" << std::endl;
    YAML::Node root = YAML::LoadFile("conf/log.yml");
    std::cout << "before toYamlString " << std::endl;
    std::cout << JerryFish::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    std::cout << "before load yaml" << std::endl;
    JerryFish::Config::LoadFromYaml(root);
    std::cout << "~~~~~~~~~~~~~~~" << std::endl;
    std::cout << JerryFish::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    std::cout << "~~~~~~~~~~~~~~~" << std::endl;
    std::cout << root << std::endl;
    JERRYFISH_LOG_INFO(system_log) << "hello system" << std::endl;
}
int main(int argc, char** argv)
{
    //test_yaml();
    //test_config();
    JerryFish::getLogInit();
    //test_class();
    test_log();
    return 0;
}
