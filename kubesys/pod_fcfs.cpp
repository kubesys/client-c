/*
 Copyright (2023, )  Institute of Software, Chinese Academy of Sciences
 Author:    lvxin22@otcaix.iscas.ac.cn
            wuheng@iscas.ac.cn
*/

#include "../include/pscheduler.h"
#include <nlohmann/json.hpp>
#include <iostream>
namespace kubesys{
bool compareTimeStrings(const std::string& datetimeStr1, const std::string& datetimeStr2) {
    struct std::tm tm1, tm2;

    std::istringstream iss1(datetimeStr1);
    std::istringstream iss2(datetimeStr2);

    iss1 >> std::get_time(&tm1, "%Y-%m-%dT%H:%M:%SZ");
    iss2 >> std::get_time(&tm2, "%Y-%m-%dT%H:%M:%SZ");

    if (!iss1.fail() && !iss2.fail()) {
        std::time_t time1 = std::mktime(&tm1);
        std::time_t time2 = std::mktime(&tm2);

        return time1 < time2;
    } else {
        std::cerr << "Failed to parse datetime strings." << std::endl;
        return false; 
    }
}
class PodFCFS  : public PodSchedule {
public:
    std::string schedule(const nlohmann::json &pod,const nlohmann::json &nodes) override {
        std::string nodename;
        std::string mintime;
        for(const nlohmann::json &node  : nodes){
            auto name = node["metedata"]["name"].get<std::string>();
            auto timevalue = node["metedata"]["creationTimestamp"].get<std::string>();
            if(mintime.empty()) {
                mintime = timevalue;
                nodename = name;
            } else if(compareTimeStrings(timevalue,mintime)){
                mintime = timevalue;
                nodename = name;
            }
        }
        return nodename;
    }
};
}