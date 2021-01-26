#include "any.h"
#include "person.h"

using namespace std;
using namespace rapidjson;

//http://rapidjson.org/zh-cn/md_doc_tutorial_8zh-cn.html
//https://gotask.net/programming/serialize-and-deserialize-array-in-cpp-using-rapidjson/
//https://gotask.net/programming/serialize-and-deserialize-object-in-cpp-using-rapidjson/
//https://stackoverflow.com/questions/31383852/parsing-object-inside-array-in-rapidjson

std::string dump(const Any& any)
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    any.Serialize(writer);
    return s.GetString();
}

Person parse(const string& json, const Person& person_aux)
{
    Document document;
    document.Parse(json);
    Person person(person_aux);
    person.DeSerialize(document.GetObject());
    return person;
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
    Person p3{"p3", 5, addr1, {f1, f2, f3}, "smell"};
 

    std::cout << "================Serialize p1 begin================" << std::endl;
    auto json = dump(p1);// 序列化
    std::cout << std::endl;
    std::cout << json << std::endl; // 打印序列化结果
    std::cout << std::endl;
    std::cout << "================Serialize p1 end================" << std::endl;
    std::cout << std::endl;
    
    
    std::cout << "================print p1 begin================" << std::endl;
    std::cout << std::endl;
    std::cout << p1 << std::endl; // 打印 Person 对象
    std::cout << std::endl;
    std::cout << "================print p1 end================" << std::endl;
    std::cout << std::endl;


    std::cout << "================DeSerialize pp begin================" << std::endl;
    auto pp = parse(json, p3);
    std::cout << std::endl;
    std::cout << pp << std::endl; // 打印 Person 对象
    std::cout << std::endl;
    std::cout << "================DeSerialize pp end================" <<std::endl;
    std::cout << std::endl;
 
    assert(p1 == pp); // 反序列化的结果是对的

    std::cout << "assert(p1 == pp), all cases passed!!" <<std::endl;
}
