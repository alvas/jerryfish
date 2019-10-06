/*
 * Utils.h
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <cxxabi.h>
#include <typeinfo>
#include <vector>
#include <string>
#include <fstream>
#include <sys/syscall.h>
#include <yaml-cpp/yaml.h>

#include "Logger.h"

namespace JerryFish {

    static JerryFish::Logger::ptr getLogger()
    {
        JerryFish::Logger::ptr g_logger = JERRYFISH_LOG_NAME("system");
        return g_logger;
    }

    /**
     * @brief 返回当前线程的ID
     */
    int GetThreadId() {
        return syscall(SYS_gettid);
    }

    /**
     * @brief 返回当前协程的ID
     */
    int GetFiberId() {
        //return JerryFish::Fiber::GetFiberId();
        return 0;
    }

    //static std::string demangle(const char* str) 
    //{
        //size_t size = 0;
        //int status = 0;
        //std::string rt;
        //rt.resize(256);
        //if(1 == sscanf(str, "%*[^(]%*[^_]%255[^)+]", &rt[0])) {
            //char* v = abi::__cxa_demangle(&rt[0], nullptr, &size, &status);
            //if(v) {
                //std::string result(v);
                //free(v);
                //return result;
            //}
        //}
        //if(1 == sscanf(str, "%255s", &rt[0])) {
            //return rt;
        //}
        //return str;
    //}

    /**
     * @brief 获取当前的调用栈
     * @param[out] bt 保存调用栈
     * @param[in] size 最多返回层数
     * @param[in] skip 跳过栈顶的层数
     */
    //void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1)
    //{
        //void** array = (void**)malloc((sizeof(void*) * size));
        //size_t s = ::backtrace(array, size);

        //char** strings = backtrace_symbols(array, s);
        //if(strings == NULL) {
            //JERRYFISH_LOG_ERROR(getLogger()) << "backtrace_synbols error";
            //return;
        //}

        //for(size_t i = skip; i < s; ++i) {
            //bt.push_back(demangle(strings[i]));
        //}

        //free(strings);
        //free(array);
    //}
    /**
     * @brief 获取当前栈信息的字符串
     * @param[in] size 栈的最大层数
     * @param[in] skip 跳过栈顶的层数
     * @param[in] prefix 栈信息前输出的内容
     */
    //std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "")
    //{
        //std::vector<std::string> bt;
        //Backtrace(bt, size, skip);
        //std::stringstream ss;
        //for(size_t i = 0; i < bt.size(); ++i) {
            //ss << prefix << bt[i] << std::endl;
        //}
        //return ss.str();
    //}

    /**
     * @brief 获取当前时间的毫秒
     */
    //uint64_t GetCurrentMS();

    /**
     * @brief 获取当前时间的微秒
     */
    //uint64_t GetCurrentUS();

    //std::string ToUpper(const std::string& name);

    //std::string ToLower(const std::string& name);

    //std::string Time2Str(time_t ts = time(0), const std::string& format = "%Y-%m-%d %H:%M:%S");
    //time_t Str2Time(const char* str, const char* format = "%Y-%m-%d %H:%M:%S");

    //class FSUtil {
        //public:
            //static void ListAllFile(std::vector<std::string>& files
                    //,const std::string& path
                    //,const std::string& subfix);
            //static bool Mkdir(const std::string& dirname);
            //static bool IsRunningPidfile(const std::string& pidfile);
            //static bool Rm(const std::string& path);
            //static bool Mv(const std::string& from, const std::string& to);
            //static bool Realpath(const std::string& path, std::string& rpath);
            //static bool Symlink(const std::string& frm, const std::string& to);
            //static bool Unlink(const std::string& filename, bool exist = false);
            //static std::string Dirname(const std::string& filename);
            //static std::string Basename(const std::string& filename);
            //static bool OpenForRead(std::ifstream& ifs, const std::string& filename
                    //,std::ios_base::openmode mode);
            //static bool OpenForWrite(std::ofstream& ofs, const std::string& filename
                    //,std::ios_base::openmode mode);
    //};

    //template<class V, class Map, class K>
        //V GetParamValue(const Map& m, const K& k, const V& def = V()) {
            //auto it = m.find(k);
            //if(it == m.end()) {
                //return def;
            //}
            //try {
                //return boost::lexical_cast<V>(it->second);
            //} catch (...) {
            //}
            //return def;
        //}

    //template<class V, class Map, class K>
        //bool CheckGetParamValue(const Map& m, const K& k, V& v) {
            //auto it = m.find(k);
            //if(it == m.end()) {
                //return false;
            //}
            //try {
                //v = boost::lexical_cast<V>(it->second);
                //return true;
            //} catch (...) {
            //}
            //return false;
        //}

    //class TypeUtil {
        //public:
            //static int8_t ToChar(const std::string& str);
            //static int64_t Atoi(const std::string& str);
            //static double Atof(const std::string& str);
            //static int8_t ToChar(const char* str);
            //static int64_t Atoi(const char* str);
            //static double Atof(const char* str);
    //};

    //class Atomic {
        //public:
            //template<class T, class S>
                //static T addFetch(volatile T& t, S v = 1) {
                    //return __sync_add_and_fetch(&t, (T)v);
                //}

            //template<class T, class S>
                //static T subFetch(volatile T& t, S v = 1) {
                    //return __sync_sub_and_fetch(&t, (T)v);
                //}

            //template<class T, class S>
                //static T orFetch(volatile T& t, S v) {
                    //return __sync_or_and_fetch(&t, (T)v);
                //}

            //template<class T, class S>
                //static T andFetch(volatile T& t, S v) {
                    //return __sync_and_and_fetch(&t, (T)v);
                //}

            //template<class T, class S>
                //static T xorFetch(volatile T& t, S v) {
                    //return __sync_xor_and_fetch(&t, (T)v);
                //}

            //template<class T, class S>
                //static T nandFetch(volatile T& t, S v) {
                    //return __sync_nand_and_fetch(&t, (T)v);
                //}

            //template<class T, class S>
                //static T fetchAdd(volatile T& t, S v = 1) {
                    //return __sync_fetch_and_add(&t, (T)v);
                //}

            //template<class T, class S>
                //static T fetchSub(volatile T& t, S v = 1) {
                    //return __sync_fetch_and_sub(&t, (T)v);
                //}

            //template<class T, class S>
                //static T fetchOr(volatile T& t, S v) {
                    //return __sync_fetch_and_or(&t, (T)v);
                //}

            //template<class T, class S>
                //static T fetchAnd(volatile T& t, S v) {
                    //return __sync_fetch_and_and(&t, (T)v);
                //}

            //template<class T, class S>
                //static T fetchXor(volatile T& t, S v) {
                    //return __sync_fetch_and_xor(&t, (T)v);
                //}

            //template<class T, class S>
                //static T fetchNand(volatile T& t, S v) {
                    //return __sync_fetch_and_nand(&t, (T)v);
                //}

            //template<class T, class S>
                //static T compareAndSwap(volatile T& t, S old_val, S new_val) {
                    //return __sync_val_compare_and_swap(&t, (T)old_val, (T)new_val);
                //}

            //template<class T, class S>
                //static bool compareAndSwapBool(volatile T& t, S old_val, S new_val) {
                    //return __sync_bool_compare_and_swap(&t, (T)old_val, (T)new_val);
                //}
    //};

    //template<class T>
        //void nop(T*) {}

    //template<class T>
        //void delete_array(T* v) {
            //if(v) {
                //delete[] v;
            //}
        //}

    //class StringUtil {
        //public:
            //static std::string Format(const char* fmt, ...);
            //static std::string Formatv(const char* fmt, va_list ap);

            //static std::string UrlEncode(const std::string& str, bool space_as_plus = true);
            //static std::string UrlDecode(const std::string& str, bool space_as_plus = true);

            //static std::string Trim(const std::string& str, const std::string& delimit = " \t\r\n");
            //static std::string TrimLeft(const std::string& str, const std::string& delimit = " \t\r\n");
            //static std::string TrimRight(const std::string& str, const std::string& delimit = " \t\r\n");


            //static std::string WStringToString(const std::wstring& ws);
            //static std::wstring StringToWString(const std::string& s);

    //};

    //std::string GetHostName();
    //std::string GetIPv4();

    //bool YamlToJson(const YAML::Node& ynode, Json::Value& jnode);
    //bool JsonToYaml(const Json::Value& jnode, YAML::Node& ynode);

    template<class T>
        const char* TypeToName() {
            static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
            return s_name;
        }

    //std::string PBToJsonString(const google::protobuf::Message& message);

    //template<class Iter>
        //std::string Join(Iter begin, Iter end, const std::string& tag) {
            //std::stringstream ss;
            //for(Iter it = begin; it != end; ++it) {
                //if(it != begin) {
                    //ss << tag;
                //}
                //ss << *it;
            //}
            //return ss.str();
        //}

}

#endif /* !UTILS_H */
