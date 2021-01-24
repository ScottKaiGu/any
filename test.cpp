#include <iostream>
#include <vector> 
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h" 
#include "rapidjson/prettywriter.h"

#include "Any.h"

using namespace std;
using namespace json;
using namespace rapidjson;

template<>
void json::Any::Derived<string>::Serialize (Writer<StringBuffer>& writer) const {
    writer.String(m_value);
}

template<>
void json::Any::Derived<const char*>::Serialize (Writer<StringBuffer>& writer) const {
    writer.String(m_value);
}

template<>
void json::Any::Derived<int>::Serialize (Writer<StringBuffer>& writer) const {
    writer.Uint(m_value);
}

struct Person;

struct Singer {
    std::string type;
    int age;

    void Serialize(Writer<StringBuffer>& writer) const {
        writer.StartObject();
        writer.String("type");
        writer.String(type);
        writer.String("age");
        writer.Uint(age);
        writer.EndObject();
    }
    void DeSerialize(const string& json) {
        Document d;
        d.Parse(json);

        if (d.HasMember("type")) {
            type = d["type"].GetString();
        }
        if (d.HasMember("age")) {
            age = d["age"].GetUint();
        }
    }
};

struct Address {
    std::string country;
    std::string city;
    std::string street;
    std::vector<Person> neighbors;

    void Serialize(Writer<StringBuffer>& writer) const;
    void DeSerialize(const string& json);
};

struct Friend {
    std::string relation;
    json::Any secret;
  
    void Serialize(Writer<StringBuffer>& writer) const {  
        writer.StartObject();
        writer.String("relation");
        writer.String(relation.c_str());
        if(!secret.isNull()) {
            writer.String("secret");
            secret.Serialize(writer);
        }
        writer.EndObject();
    }

    void DeSerialize(const string& json) {
        //  assert(document.HasMember("hello"));
        // assert(document["hello"].IsString());
        Document d;
        d.Parse(json);
        if (d.HasMember("relation")) {
            relation = d["relation"].GetString();
        }
        if (d.HasMember("secret")) {
            // const string& tmp = d["secret"].GetString();
            // secret.DeSerialize(tmp);
        }
    }

    void DeSerialize(rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject object) {
        if (object.HasMember("relation")) {
            relation = object["relation"].GetString();
        }
        if (object.HasMember("secret")) {
            // if (object["secret"].IsUint()) {
            //     secret = object["secret"].GetUint();
            // }
            // if (object["secret"].IsString()) {
            //     secret = object["secret"].GetString();
            // }
            // if (object["secret"].IsObject()) {
            //     //secret.DeSerialize(object["secret"].GetObject());
            // }
        }
    }
};

struct Person {
    std::string name;
    int age;
    Address address;
    std::vector<Friend> _friends;
    json::Any secret;

    void Serialize(Writer<StringBuffer>& writer) const;
    void DeSerialize(const string& json);
    friend ostream& operator<<(ostream &out, Person &person) {
        StringBuffer s;
        Writer<StringBuffer> writer(s);
        person.Serialize(writer);
        out<<s.GetString();
        return out;
    }
};

// template <typename Writer>
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

void Address::DeSerialize(const string& json) {
    Document d;
    d.Parse(json);
    if (d.HasMember("country")) {
        country = d["country"].GetString();
    }
    if (d.HasMember("city")) {
        city = d["city"].GetString();
    }
    if (d.HasMember("street")) {
        street = d["street"].GetString();
    }
    if (d.HasMember("neighbors")) {
        const Value& neighbors_array = d["neighbors"];
        for (rapidjson::SizeType i = 0; i < neighbors_array.Size(); ++i) {
            Person person;
            person.DeSerialize(neighbors_array[i].GetString());
            neighbors.push_back(person);
        }
    }
}

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


void Person::DeSerialize(const string& json) {
    //  assert(document.HasMember("hello"));
    // assert(document["hello"].IsString());
    Document d;
    d.Parse(json);

    if (d.HasMember("name")) {
        name = d["name"].GetString();
    }
    if (d.HasMember("age")) {
        age = d["age"].GetUint();
    }
    // if (d.HasMember("address")) {
    //     address.DeSerialize(d["address"].GetString());
    // }
    if (d.HasMember("friends")) {
        const Value& friends = d["friends"];
        for (rapidjson::SizeType i = 0; i < friends.Size(); ++i) {
            Friend ff;
            ff.DeSerialize(friends[i].GetObject());
            _friends.push_back(ff);
        }
    }
}

std::string dump(Person p)
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    p.Serialize(writer);
    return s.GetString();
}

std::string dump(Singer p)
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    p.Serialize(writer);
    return s.GetString();
}

std::string dump(Friend p)
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    p.Serialize(writer);
    return s.GetString();
}

std::string dump(Any p)
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    p.Serialize(writer);
    return s.GetString();
}

// 实现一个支持 json 序列化/反序列化、支持 std::cout 输出的 any 类型。json 序列化库采用rapidjson。
int main() {
    json::Any aaa;
    json::Any a(string("s123"));
	cout << a  << endl;
    Friend f1{"my best friend", Singer{"rocker", 18}};
    Friend f2{"new friend", "little girl"};
    Friend f3{"third friend", 3};
    Person p2{"p2", 3, Address{"china", "shanghai", "putuo"}};
    Address addr1{"china", "beijing", "wangjing", {p2}};
    Person p1{"p1", 4, addr1, {f1, f2, f3}, "the kind!"};
    // // TODO. 以下是伪代码，需要笔试者具体实现 
    auto singer = Singer{"rocker"};
    Singer singer1;
    Singer Singer2{"rocker", 18};
    singer1.DeSerialize("{\"type\":\"rocker\",\"age\":18}");

    auto json = dump(p1);// 序列化
    std::cout << json << std::endl; // 打印序列化结果
    std::cout << p1 << std::endl; // 打印 Person 对象 
    aaa = p1;
    json = dump(aaa);// 序列化
    std::cout << json << std::endl; // 打印序列化结果
 
    // Person p22;
    // p22.DeSerialize(json);
    // std::cout << p22 << std::endl; // 打印 Person 对象 
    // auto pp = parse(json);
    // assert(p1 == pp)// 反序列化的结果是对的
}

