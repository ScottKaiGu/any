#include "person.h"

void Person::Serialize(Writer<StringBuffer>& writer) const {
    writer.StartObject();
    writer.String("name");
    writer.String(name);
    writer.String("age");
    writer.Uint(age);

    writer.String("address");
    address.Serialize(writer);

    if (_friends.size() > 0) {
        writer.String("friends");
        writer.StartArray();
        for (auto ite = _friends.begin(); ite != _friends.end(); ++ite) {
            ite->Serialize(writer);
        }
        writer.EndArray();
    }

    if(!secret.isNull()){
        writer.String("secret");
        secret.Serialize(writer);
    }
    writer.EndObject();
}

void Person::DeSerialize(const Value& value) {
    if (value.HasMember("name")) {
        name = value["name"].GetString();
    }
    if (value.HasMember("age")) {
        age = value["age"].GetUint();
    }

    if (value.HasMember("friends")) {
        const Value& friends = value["friends"];
        for (auto& friend_value : friends.GetArray()) {
            Friend f;
            f.DeSerialize(friend_value.GetObject());
        }
    }
    if (value.HasMember("address")) {
        address.DeSerialize(value["address"]);
    }
    if (value.HasMember("secret")) {
        secret.DeSerialize(value["secret"]);
    }
}

ostream& operator<<(ostream &out, Person &person) {
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    person.Serialize(writer);
    out<<s.GetString();
    return out;
}

bool Person::operator==(const Person& other) const {
    if (name != other.name) {
        return false;
    }
    if (age != other.age) {
        return false;
    }
    if (address != other.address) {
        return false;
    }
    if (_friends != other._friends) {
        return false;
    }
    if (secret != other.secret) {
        return false;
    }
    cout << __FILE__<< ":" <<__LINE__ << endl;
    return true;
}

bool Person::operator!=(const Person& other) const {
    if (*this == other) {
        return false;
    }
    return true;
}