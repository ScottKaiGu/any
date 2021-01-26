#ifndef FRIEND_H_
#define FRIEND_H_
#include <iostream>
#include <vector> 
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h" 
#include "rapidjson/prettywriter.h"

#include "any.h"

struct Friend {
    std::string relation;
    json::Any secret;

    void Serialize(Writer<StringBuffer>& writer) const;
    void DeSerialize(const Value& value);
    bool operator==(const Friend& other) const;
    bool operator!=(const Friend& other) const;
};
#endif  // FRIEND_H_