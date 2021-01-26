#ifndef ANY_H_
#define ANY_H_

#include <iostream>
#include <memory>
#include <typeindex>
#include <type_traits>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "singer.h"

using namespace std;
using namespace rapidjson;

namespace json {
class Any {
public:
    //默认构造函数
    Any() : m_tpIndex(std::type_index(typeid(void))) {}
    Any(const Any& other) : m_ptr(other.clone()), m_tpIndex(other.m_tpIndex) {}
    Any(Any&& other) : m_ptr(std::move(other.m_ptr)), m_tpIndex(std::move(other.m_tpIndex)) {}

    //通用的右值构造
    template<class T, class = typename std::enable_if<!std::is_same<typename std::decay<T>::type, Any>::value, T>::type>
    Any(T&& t)
        : m_ptr(new Derived<typename std::decay<T>::type>(std::forward<T>(t)))
        , m_tpIndex(typeid(typename std::decay<T>::type)) {}
 
    //判断是否为空
    bool isNull() const
    {
        return !bool(m_ptr);
    }

    // template <typename Writer>
    void Serialize(Writer<StringBuffer>& writer) const {
        if(m_ptr){
            m_ptr->Serialize(writer);
        }
    }

    void DeSerialize(const Value& value) {
        if (m_ptr) {
            m_ptr->DeSerialize(value);
        }
    }

    bool operator ==(const Any& other) const {
        cout << __FILE__<< ":" <<__LINE__ << endl;
        if (m_ptr == other.m_ptr) {
            return true;
        }
        if (m_ptr) {
            return m_ptr->equal(other.m_ptr.get());
        }
        return false;
    }

    bool operator !=(const Any& other) const {
        if (*this == other) {
            return false;
        }
        return true;
     }

    //是否可以类型转换
    template<class T>
    bool is()
    {
        return m_tpIndex == type_index(typeid(T));
    }
     //类型转换
    template<class T>
    T& cast()
    {
        if (!is<T>())
        {
            cout << "can not cast " << typeid(T).name() << " to "
                << m_tpIndex.name() << endl;
            throw bad_cast();
        }
        auto ptr = dynamic_cast<Derived<T>*>(m_ptr.get());
        return ptr->m_value;
    }

    Any& operator=(const Any& other)
    {
        if (m_ptr == other.m_ptr)
        {
            return *this;
        }
        m_ptr = other.clone();
        m_tpIndex = other.m_tpIndex;
        return *this;
    }

    friend ostream& operator<<(ostream &out, Any &any) {
        if (!any.isNull()) {
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            any.m_ptr->Serialize(writer);
            out<<s.GetString();
        }
        return out;
    }

private:
    struct Base;
    using BasePtr = std::unique_ptr<Base>;

    //非模板擦除类型
    struct Base
    {
        virtual BasePtr clone() const = 0;
        virtual void Serialize(Writer<StringBuffer>& writer) const = 0;
        virtual void DeSerialize(const Value& value) = 0;
        virtual bool equal(const Base* other) const= 0;
        virtual ~Base() {};
    };

    template<typename T>
    struct Derived : public Base
    {
        template<typename...Args>
        Derived(Args&&...args) : m_value(std::forward<Args>(args)...) {
        }
        BasePtr clone() const override {
            return BasePtr(new Derived(m_value));
        }

        void Serialize(Writer<StringBuffer>& writer) const override {        
            m_value.Serialize(writer);
        }
        void DeSerialize(const Value& value) override {
            cout << "type: " << value.GetType() << endl;
            if (value.IsObject()) {
                m_value.DeSerialize(value);
                return;
            }
            return;
        }
        bool equal(const Base* other) const override {
            if (this == other) {
                return true;
            }
            cout << __FILE__<< ":" <<__LINE__ << endl;
            const Derived<T>* p = dynamic_cast<const Derived<T>*>(other);
            if (p == nullptr) {
                return false;
            }
            cout << __FILE__<< ":" <<__LINE__ << endl;
            return m_value == p->m_value;
        }
 
        T m_value;
    };

    //拷贝使用
    BasePtr clone() const {
        if (m_ptr) {
            return m_ptr->clone();
        }
        return nullptr;
    }

    BasePtr         m_ptr;      //具体数据
    std::type_index m_tpIndex;  //数据类型
};


template<>
inline void Any::Derived<string>::Serialize (Writer<StringBuffer>& writer) const {
    writer.String(m_value);
}

template<>
inline void Any::Derived<string>::DeSerialize(const Value& value) {
    m_value = value.GetString();
}

template<>
inline void Any::Derived<const char*>::Serialize (Writer<StringBuffer>& writer) const {
    writer.String(m_value);
}

template<>
inline void Any::Derived<const char*>::DeSerialize(const Value& value) {
    m_value = value.GetString();
}

template<>
inline void Any::Derived<int>::Serialize (Writer<StringBuffer>& writer) const {
    writer.Uint(m_value);
}

template<>
inline void Any::Derived<int>::DeSerialize (const Value& value) {
    m_value = value.GetUint();
}
}
#endif // ANY_H_