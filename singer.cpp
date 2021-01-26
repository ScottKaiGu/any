#include "singer.h"

void Singer::Serialize(Writer<StringBuffer>& writer) const {
    writer.StartObject();
    writer.String("type");
    writer.String(type);
    writer.String("age");
    writer.Uint(age);
    writer.EndObject();
}

void Singer::DeSerialize(const Value& value) {
    if (value.HasMember("type")) {
        type = value["type"].GetString();
    }
    if (value.HasMember("age")) {
        age = value["age"].GetUint();
    }
}

bool Singer::operator==(const Singer& other) const {
    if (type != other.type) {
        return false;
    }
    if (age != other.age) {
        return false;
    }
    cout << __FILE__<< ":" <<__LINE__ << endl;
    return true;
}

bool Singer::operator!=(const Singer& other) const {
    if (*this == other) {
        return false;
    }
    return true;
}