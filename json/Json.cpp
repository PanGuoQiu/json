#include <sstream>
#include <stdexcept>
#include "Json.h"
#include "Parser.h"
using std::stringstream;
using namespace pan::json;

// 构造函数
Json::Json() : m_type(JsonType::json_null)
{
}

Json::Json(JsonType type) : m_type(type)
{
    // 辨别JsonType的类型
    switch (m_type)
    {
        case JsonType::json_null:
            break;
        case JsonType::json_bool:
            m_value.m_bool = false;
            break;
        case JsonType::json_int:
            m_value.m_int = 0;
            break;
        case JsonType::json_double:
            m_value.m_double = 0.0;
            break;
        case JsonType::json_string:
            m_value.m_string = new string("");
            break;
        case JsonType::json_array:
            m_value.m_array = new std::list<Json>();
            break;
        case JsonType::json_object:
            m_value.m_object = new std::map<string, Json>();
        default:
            break;
    }
}

Json::Json(bool value) : m_type(JsonType::json_bool)
{
    m_value.m_bool = value;
}

Json::Json(int value) : m_type(JsonType::json_int)
{
    m_value.m_int = value;
}

Json::Json(double value) : m_type(JsonType::json_double)
{
    m_value.m_double = value;
}

Json::Json(const char * value) : m_type(JsonType::json_string)
{
    m_value.m_string = new string(value);
}

Json::Json(const string & value) : m_type(JsonType::json_string)
{
    m_value.m_string = new string(value);
}

Json::Json(const Json & other) : m_type(JsonType::json_null)
{
    // 复制
    copy(other);
}

// 析构函数
Json::~Json()
{
}

// 返回Json的类型
Json::JsonType Json::type() const
{
    return m_type;
}

// 判断是否为空类型
bool Json::isNull() const
{
    return m_type == JsonType::json_null;
}

// 判断是否为布尔类型
bool Json::isBool() const
{
    return m_type == JsonType::json_bool;
}

// 判断是否为整型
bool Json::isInt() const
{
    return m_type == JsonType::json_int;
}

// 判断是否为双精度浮点型
bool Json::isDouble() const
{
    return m_type == JsonType::json_double;
}

// 判断是否为字符串类型
bool Json::isString() const
{
    return m_type == JsonType::json_string;
}

// 判断是否为数组类型
bool Json::isArray() const
{
    return m_type == JsonType::json_array;
}

// 判断是否为对象类型
bool Json::isObject() const
{
    return m_type == JsonType::json_object;
}

// 作为bool类型
bool Json::asBool() const
{
    if (m_type == JsonType::json_bool)
    {
        return m_value.m_bool;
    }

    throw std::logic_error("function Json::asBool value type error");
}

// 作为int类型
int Json::asInt() const
{
    if (m_type == JsonType::json_int)
    {
        return m_value.m_int;
    }

    throw std::logic_error("function Json::asInt value type error");
}

// 作为double类型
double Json::asDouble() const
{
    if (m_type == JsonType::json_double)
    {
        return m_value.m_double;
    }

    throw std::logic_error("function Json::asDouble value type error");
}

// 作为string类型
string Json::asString() const
{
    if (m_type == JsonType::json_string)
    {
        return *(m_value.m_string);
    }

    throw std::logic_error("function Json::asString value type error");
}

// 复制操作
void Json::copy(const Json & other)
{
    m_type  = other.m_type;
    m_value = other.m_value;
}

// 数组元素个数和对象个数
int Json::size() const
{
    switch (m_type)
    {
        case JsonType::json_array:
            return (m_value.m_array)->size();
        case JsonType::json_object:
            return (m_value.m_object)->size();
        default:
            break;
    }

    throw std::logic_error("function Json::size value type error");
}

// 判断类型是否为空
bool Json::empty() const
{
    switch (m_type)
    {
        case JsonType::json_null:
            return true;
        case JsonType::json_array:
            return (m_value.m_array)->empty();
        case JsonType::json_object:
            return (m_value.m_object)->empty();
        default:
            break;
    }

    return false;
}

// 清空内存
void Json::clear()
{
    switch (m_type)
    {
        case JsonType::json_null:
        case JsonType::json_bool:
        case JsonType::json_int:
        case JsonType::json_double:
            break;
        case JsonType::json_string:
            {
                delete m_value.m_string;
            }
            break;
        case JsonType::json_array:
            {
                for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
                {
                    it->clear();
                }

                delete m_value.m_array;
            }
            break;
        case JsonType::json_object:
            {
                for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
                {
                    it->second.clear();
                }

                delete m_value.m_object;
            }
            break;
        default:
            break;
    }

    m_type = JsonType::json_null;
}

// 判断数组中是否存在index索引
bool Json::has(int index)
{
    if (m_type != JsonType::json_array)
    {
        return false;
    }

    int size = (m_value.m_array)->size();
    return (index >= 0) && (index < size);
}

// 判断map中是否存在key的键
bool Json::has(const char * key)
{
    string name(key);
    return has(name);
}

bool Json::has(const string & key)
{
    if (m_type != JsonType::json_object)
    {
        return false;
    }

    return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

// 移除数组中的下标为index的节点
void Json::remove(int index)
{
    // 判断是否为数组
    if (m_type != JsonType::json_array)
    {
        return;
    }

    // 判断索引是否为合法
    int size = (m_value.m_array)->size();
    if (index < 0 || index >= size)
    {
        return;
    }

    // 遍历找到index索引下标，并释放
    auto it = (m_value.m_array)->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }
    it->clear();
    (m_value.m_array)->erase(it);
}

// 移除map中key的键值对
void Json::remove(const char * key)
{
    string name = key;
    remove(name);
}

void Json::remove(const string & key)
{
    // 判断类型是否json类型
    if (m_type != JsonType::json_object)
    {
        return;
    }

    // 找到key，并移除
    auto it = (m_value.m_object)->find(key);
    if (it != (m_value.m_object)->end())
    {
        it->second.clear();
        (m_value.m_object)->erase(key);
    }
}

// 在数组后面添加一个值
void Json::append(const Json & value)
{
    if (m_type != JsonType::json_array)
    {
        clear();
        m_type = JsonType::json_array;
        m_value.m_array = new std::list<Json>();
    }

    (m_value.m_array)->push_back(value);
}

// 重载赋值运算符
Json & Json::operator=(const Json & other)
{
    clear();
    copy(other);
    return *this;
}

// 重载==运算符
bool Json::operator==(const Json & other)
{
    // 判断类型是否相等
    if (m_type != other.m_type)
    {
        return false;
    }

    // 判断类型
    switch (m_type)
    {
        case JsonType::json_null:
            return true;
        case JsonType::json_bool:
            return (m_value.m_bool == other.m_value.m_bool);
        case JsonType::json_int:
            return (m_value.m_int == other.m_value.m_int);
        case JsonType::json_double:
            return (m_value.m_double == other.m_value.m_double);
        case JsonType::json_string:
            return *(m_value.m_string) == *(other.m_value.m_string);
        case JsonType::json_array:
            return m_value.m_array == other.m_value.m_array;
        case JsonType::json_object:
            return m_value.m_object == other.m_value.m_object;
        default:
            break;
    }

    return false;
}

// 重载!=运算符
bool Json::operator!=(const Json & other)
{
    return !(*this == other);
}

// 重载[]运算符
Json & Json::operator[](int index)
{
    // 判断下标是否<0
    if (index < 0)
    {
        throw std::logic_error("function Json::operator [int] index less than 0");
    }

    // 判断类型是否为数组
    if (m_type != JsonType::json_array)
    {
        clear();
        m_type = JsonType::json_array;
        m_value.m_array = new std::list<Json>();
    }

    // 返回Json节点，下标为index
    int size = (m_value.m_array)->size();
    if (index >= 0 && index < size)
    {
        auto it = (m_value.m_array)->begin();
        for (int i = 0; i < index; i++)
        {
            it++;
        }

        return *it;
    }

    // 如果下标溢出，则创建并返回
    if (index >= size)
    {
        for (int i = size; i <= index; i++)
        {
            // 存储一个空的Json对象
            (m_value.m_array)->push_back(Json());
        }
    }
    
    return (m_value.m_array)->back();
}

// 重载[]运算符
Json & Json::operator[](const char * key)
{
    string name = key;
    return (*this)[name];
}

// 重载[]运算符
Json & Json::operator[](const string & key)
{
    if (m_type != JsonType::json_object)
    {
        clear();
        m_type = JsonType::json_object;
        m_value.m_object = new std::map<string, Json>();
    }

    return (*(m_value.m_object))[key];
}

// 转换为bool类型
Json::operator bool()
{
    if (m_type != JsonType::json_bool)
    {
        throw std::logic_error("function Json::operator (bool) requires bool value");
    }

    return m_value.m_bool;
}

// 转换为int类型
Json::operator int()
{
    if (m_type != JsonType::json_int)
    {
        throw std::logic_error("function Json::operator (int) requires int value");
    }

    return m_value.m_int;
}

Json::operator double()
{
    if (m_type != JsonType::json_double)
    {
        throw std::logic_error("function Json::operator (double) requires double value");
    }

    return m_value.m_double;
}

// 转换为string类型
Json::operator string()
{
    if (m_type != JsonType::json_string)
    {
        throw std::logic_error("function Json::operator (string) requires string value");
    }

    return *(m_value.m_string);
}

// 解析json文件
void Json::parse(const string & str)
{
    Parser parser;
    parser.load(str);
    *this = parser.parse();
}

// 反解析
string Json::str() const
{
    stringstream ss;
    // 判断类型
    switch (m_type)
    {
        case JsonType::json_null:
            ss << "null";
            break;
        case JsonType::json_bool:
            if (m_value.m_bool)
            {
                ss << "true";
            }
            else
            {
                ss << "false";
            }
            break;
        case JsonType::json_int:
            ss << m_value.m_int;
            break;
        case JsonType::json_double:
            ss << m_value.m_double;
            break;
        case JsonType::json_string:
            ss << "\"" << *(m_value.m_string) << "\"";
            break;
        case JsonType::json_array:
            {
                ss << "[";
                for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
                {
                    if (it != (m_value.m_array)->begin())
                    {
                        ss << ",";
                    }

                    ss << (*it).str();
                }
                ss << "]";
            }
            break;
        case JsonType::json_object:
            {
                ss << "{";
                for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
                {
                    if (it != (m_value.m_object)->begin())
                    {
                        ss << ",";
                    }
                    ss << "\"" << it->first << "\":" << it->second.str();
                }
                ss << "}";
            }
            break;
        default:
            break;
    }

    return ss.str();
}
