
#ifndef ADDRESS_H_
#define ADDRESS_H_

#include <iostream>
#include <vector> 
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h" 
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

struct Person;

struct Address {
    std::string country;
    std::string city;
    std::string street;
    std::vector<Person> neighbors;

    void Serialize(Writer<StringBuffer>& writer) const;
    void DeSerialize(const Value& value);
    bool operator==(const Address& other) const;
    bool operator!=(const Address& other) const;
};
#endif // ADDRESS_H_