#ifndef FERSON_H_
#define FERSON_H_
#include <iostream>
#include <vector> 
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h" 
#include "rapidjson/prettywriter.h"

#include "address.h"
//#include "any.h"
#include "friend.h"

using namespace std;
using namespace json;
using namespace rapidjson;

struct Person {
    std::string name;
    int age;
    Address address;
    std::vector<Friend> _friends;
    json::Any secret;

    void Serialize(Writer<StringBuffer>& writer) const;
    void DeSerialize(const Value& value);
    bool operator==(const Person& other) const;
    bool operator!=(const Person& other) const;
    friend ostream& operator<<(ostream &out, Person &person);
};
#endif // FERSON_H_