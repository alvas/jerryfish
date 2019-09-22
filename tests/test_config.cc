/*
 * test_config.cc
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "src/Config.h"
#include "src/Logger.h"
#include "yaml-cpp/yaml.h"

JerryFish::ConfigVar<int>::ptr g_int_value_config = JerryFish::Config::Lookup("system.prot", (int)8080, "system port");
JerryFish::ConfigVar<int>::ptr g_float_value_config = JerryFish::Config::Lookup("system.value", (float)8080, "system value");

void test_yaml()
{

}

int main(int argc, char** argv)
{
    JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << g_int_value_config->getValue();
    JERRYFISH_LOG_INFO(JERRYFISH_LOG_ROOT()) << g_float_value_config->toString();
    return 0;
}
