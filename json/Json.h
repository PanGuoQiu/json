#pragma once

#include <string>
#include <list>
#include <map>
using std::string;

namespace pan
{
    namespace json
    {
        // Json类
        class Json
        {
            public:
                // 枚举类类型
                enum class JsonType
                {
                    json_null = 0,                      // null value
                    json_bool,                          // bool value
                    json_int,                           // int value
                    json_double,                        // double value
                    json_string,                        // string value
                    json_array,                         // array value
                    json_object                         // object value
                };

                // 构造函数和析构函数
                Json();
                Json(JsonType type);
                Json(bool value);
                Json(int value);
                Json(double value);
                Json(const char* value);
                Json(const string& value);
                Json(const Json& other);
                ~Json();

                // 返回json的类型
                JsonType type() const;

                // 判断Json的类型
                bool isNull() const;                                    // 判断是否为空类型
                bool isBool() const;                                    // 判断是否为布尔类型
                bool isInt() const;                                     // 判断是否为整型
                bool isDouble() const;                                  // 判断是否为双精度浮点型
                bool isString() const;                                  // 判断是否为字符串类型
                bool isArray() const;                                   // 判断是否为数组类型
                bool isObject() const;                                  // 判断是否为对象类型

                // Json对象作为对应的类型
                bool   asBool() const;                                  // 作为bool类型
                int    asInt() const;                                   // 作为int类型
                double asDouble() const;                                // 作为double类型
                string asString() const;                                // 作为string类型

                // number of values in array or object
                int size() const;

                // return true if empty array, empty object, or null, otherwise, false.
                bool empty() const;

                // 清空内存
                void clear();

                // 判断是否存在索引值
                bool has(int index);                                    // 判断索引index是否存在
                bool has(const char * key);                             // 判断map中的key是否存在
                bool has(const string & key);

                // 移除索引节点操作
                void remove(int index);
                void remove(const char * key);
                void remove(const string & key);

                // append value to array at the end.
                void append(const Json & value);

                // 重载运算符
                Json & operator=(const Json & other);
                bool   operator==(const Json & other);
                bool   operator!=(const Json & other);

                // 重载[]运算符
                Json & operator[](int index);
                Json & operator[](const char * key);
                Json & operator[](const string & key);

                // 转换类型
                operator bool();
                operator int();
                operator double();
                operator string();

                // 解析json和反序列化
                void parse(const string & str);                         // 解析json文件
                string str() const;                                     // 转换为字符串

                // 迭代器
                typedef std::list<Json>::iterator iterator;
                iterator begin()
                {
                    return (m_value.m_array)->begin();
                }
                iterator end()
                {
                    return (m_value.m_array)->end();
                }

            private:
                // 复制操作
                void copy(const Json & other);

            private:
                // 存储Json的值
                union Value
                {
                    bool                     m_bool;
                    int                      m_int;
                    double                   m_double;
                    std::string            * m_string;
                    std::list<Json>        * m_array;
                    std::map<string, Json> * m_object;
                };

                JsonType m_type;                                        // Json的类型
                Value    m_value;                                       // Json的值
                
        };
    }
}
