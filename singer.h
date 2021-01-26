#ifndef SINGER_H_
#define SINGER_H_
#include <iostream>
#include <memory>
#include <typeindex>
#include <type_traits>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

struct Singer {
    std::string type;
    int age;

    void Serialize(Writer<StringBuffer>& writer) const;
    void DeSerialize(const Value& value);
	bool operator==(const Singer& other) const;
    bool operator!=(const Singer& other) const;
};
#endif // SINGER_H_