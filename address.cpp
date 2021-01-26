#include "address.h"
#include "person.h"

void Address::Serialize(Writer<StringBuffer>& writer) const {
    writer.StartObject();
    writer.String("country");
    writer.String(country);
    writer.String("city");
    writer.String(city);
    writer.String("street");
    writer.String(street);

    if (neighbors.size() > 0) {
        writer.String(("neighbors"));
        writer.StartArray();
        for (const auto& neighbor : neighbors) {
            neighbor.Serialize(writer);
        }
        writer.EndArray();
    }

    writer.EndObject();
}

void Address::DeSerialize(const Value& value) {
    if (value.HasMember("country")) {
        country = value["country"].GetString();
    }
    if (value.HasMember("city")) {
        city = value["city"].GetString();
    }
    if (value.HasMember("street")) {
        street = value["street"].GetString();
    }
    if (value.HasMember("neighbors")) {
        for (auto& neighbor_value : value["neighbors"].GetArray()) {
            Person neighbor;
            neighbor.DeSerialize(neighbor_value);
        }
    }
}

bool Address::operator==(const Address& other) const {
    if (country != other.country) {
        return false;
    }
    if (city != other.city) {
        return false;
    }
    if (street != other.street) {
        return false;
    }
    if (neighbors != other.neighbors) {
        return false;
    }
    cout << __FILE__<< ":" <<__LINE__ << endl;
    return true;
}

bool Address::operator!=(const Address& other) const {
    if (*this == other) {
        return false;
    }
    return true;
}