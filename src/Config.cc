/*
 * Config.cc
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Config.h"

namespace JerryFish 
{
    static JerryFish::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

    //"A.B", 10
    //A:
    //  B: 10
    //  C: str

    static void ListAllMember(const std::string& prefix,
            const YAML::Node& node,
            std::list<std::pair<std::string, const YAML::Node> >& output) {
        if(prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678")
                != std::string::npos) {
            SYLAR_LOG_ERROR(g_logger) << "Config invalid name: " << prefix << " : " << node;
            return;
        }
        output.push_back(std::make_pair(prefix, node));
        if(node.IsMap()) {
            for(auto it = node.begin();
                    it != node.end(); ++it) {
                ListAllMember(prefix.empty() ? it->first.Scalar()
                        : prefix + "." + it->first.Scalar(), it->second, output);
            }
        }
    }

    void Config::LoadFromYaml(const YAML::Node& root) {
        std::list<std::pair<std::string, const YAML::Node> > all_nodes;
        ListAllMember("", root, all_nodes);

        for(auto& i : all_nodes) {
            std::string key = i.first;
            if(key.empty()) {
                continue;
            }

            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            ConfigVarBase::ptr var = LookupBase(key);

            if(var) {
                if(i.second.IsScalar()) {
                    var->fromString(i.second.Scalar());
                } else {
                    std::stringstream ss;
                    ss << i.second;
                    var->fromString(ss.str());
                }
            }
        }
    }

    static std::map<std::string, uint64_t> s_file2modifytime;
    static JerryFish::Mutex s_mutex;
}
