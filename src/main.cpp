/*
 * main.cpp
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */


#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <string>
#include <fstream>


using namespace rapidjson;
using namespace std;

string fileName;

//void readTestDataJsonArray()
//{
    //string err;
    //std::ifstream data(fileName, std::ifstream::binary);
    //string file((std::istreambuf_iterator<char>(data)), std::istreambuf_iterator<char>());
    //auto jsons = Json::parse(file, err);

    //for (auto j: jsons.array_items())
    //{
        //cout << j.dump() << endl;
    //}

    //data.close();
//}

//void readTestData()
//{
    //string file;
    //std::ifstream data(fileName, std::ifstream::binary);

    //if (data.is_open())
    //{
        //string line;

        //while (getline(data, line))
        //{
            //file += line;
        //}

        //data.close();
    //}

    
    //auto bItr = file.find('{');
    //vector<Json> jsons;
    //string err;

    //while (bItr != string::npos) {
        //auto eItr = file.find('}', bItr);

        //if (eItr != string::npos) {
            //auto str = file.substr(bItr, eItr - bItr + 1);
            //auto json = Json::parse(str, err);
            //jsons.push_back(json);
        //}

        //bItr = file.find('{', eItr);
    //}

    //for (auto j: jsons) {
        //cout << j.dump() << endl;
    //}
//}

int main(int argc, char** argv)
{
    /*
    const std::string simple_test =
        R"({"k1":"v1", "k2":42, "k3":["a",123,true,false,null]})";

    std::string err;
    const auto json = Json::parse(simple_test, err);

    std::cout << "k1: " << json["k1"].string_value() << "\n";
    std::cout << "k3: " << json["k3"].dump() << "\n";

    for (auto &k : json["k3"].array_items()) {
        std::cout << "    - " << k.dump() << "\n";
    }
    */

    //readTestData();
    //readTestDataJsonArray();

    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);
    // 2. 利用 DOM 作出修改。
    Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);
    // 3. 把 DOM 转换（stringify）成 JSON。
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return 0;
}
