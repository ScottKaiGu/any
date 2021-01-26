#include "friend.h"

void Friend::Serialize(Writer<StringBuffer>& writer) const {  
    writer.StartObject();
    writer.String("relation");
    writer.String(relation.c_str());
    if(!secret.isNull()) {
        writer.String("secret");
        secret.Serialize(writer);
    }
    writer.EndObject();
}

void Friend::DeSerialize(const Value& value) {
    if (value.HasMember("relation")) {
        relation = value["relation"].GetString();
    }
    if (value.HasMember("secret")) {
        secret.DeSerialize(value["secret"]);
    }
}

bool Friend::operator==(const Friend& other) const {
    if (relation != other.relation) {
        return false;
    }
    if (secret != other.secret) {
        return false;
    }
    cout << __FILE__<< ":" <<__LINE__ << endl;
    return true;
}

bool Friend::operator!=(const Friend& other) const {
    if (*this == other) {
        return false;
    }
    return true;
}