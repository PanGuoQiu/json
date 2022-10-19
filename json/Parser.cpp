#include <string.h>
#include <stdexcept>
#include <cstdlib>
#include "Parser.h"
using namespace std;
using namespace pan::json;

// 构造函数
Parser::Parser() : m_idx(0)
{
}

// 析构函数
Parser::~Parser()
{
}

// 载入json文件
void Parser::load(const string &str)
{
    m_str = str;
    m_idx = 0;
}

// 跳过空格
void Parser::skip_white_space()
{
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
        m_idx++;
}

// 获得下一个字符
char Parser::get_next_token()
{
    skip_white_space();
    // 判断指向是否为最后一个字符
    if (m_idx == m_str.size())
        throw std::logic_error("unexpected end of input");

    return m_str[m_idx++];
}

// 解析json文件
Json Parser::parse()
{
    char ch = get_next_token();
    // 判断字符的类型
    switch (ch)
    {
        case 'n':                                               // null
            m_idx--;
            return parse_null();
        case 't':                                               // true
        case 'f':                                               // false
            m_idx--;
            return parse_bool();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            m_idx--;
            return parse_number();
        case '"':
            return Json(parse_string());
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        default:
            break;
    }

    throw std::logic_error("unexpected character in parse json");
}

// 解析空类型
Json Parser::parse_null()
{
    // 判断指向的位置后4个字符是否null
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }

    throw std::logic_error("parse null error");
}

// 解析bool类型
Json Parser::parse_bool()
{
    // 判断指向的位置后4个字符是否为true
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    }

    // 判断指向的位置后5个字符是否为false
    if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }

    throw std::logic_error("parse bool error");
}

// 解析数据：int和double
Json Parser::parse_number()
{
    size_t pos = m_idx;

    if (m_str[m_idx] == '-')
        m_idx++;

    // interger part
    if (m_str[m_idx] == '0')
    {
        m_idx++;
    }
    else if (in_range(m_str[m_idx], '1', '9'))
    {
        m_idx++;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx++;
        }
    }
    else
    {
        throw std::logic_error("invalid character in number");
    }
    // 如果没有小数点，则返回整型
    if (m_str[m_idx] != '.')
    {
        return Json(std::atoi(m_str.c_str() + pos));
    }

    // decimal part
    m_idx++;
    if (!in_range(m_str[m_idx], '0', '9'))
    {
        throw std::logic_error("at least one digit required in fractional part");
    }

    while (in_range(m_str[m_idx], '0', '9'))
    {
        m_idx++;
    }

    // 返回双精度浮点型：double
    return Json(std::atof(m_str.c_str() + pos));
}

// 解析字符串
string Parser::parse_string()
{
    int pos = m_idx;
    while (true)
    {
        // 判断字符串是否结束
        if (m_idx == m_str.size())
        {
            throw std::logic_error("unexpected end of input in string");
        }

        // 判断字符的类型
        char ch = m_str[m_idx++];
        if (ch == '"')
        {
            break;
        }

        // The usual case: non-escaped characters
        if (ch == '\\')
        {
            ch = m_str[m_idx++];
            switch (ch)
            {
                case 'b':
                case 't':
                case 'n':
                case 'f':
                case 'r':
                case '"':
                case '\\':
                    break;
                case 'u':
                    m_idx += 4;
                    break;
                default:
                    break;
            }
        }
    }

    // 返回字符串
    return m_str.substr(pos, m_idx - pos - 1);
}

// 解析数组
Json Parser::parse_array()
{
    Json arr(Json::JsonType::json_array);
    // 获得下一个字符串，并判断类型
    char ch = get_next_token();
    if (ch == ']')
    {
        return arr;
    }

    m_idx--;
    while (true)
    {
        arr.append(parse());
        ch = get_next_token();
        if (ch == ']')
        {
            break;
        }

        if (ch != ',')
        {
            throw std::logic_error("expected ',' in array");
        }
    }

    return arr;
}

// 解析对象
Json Parser::parse_object()
{
    Json obj(Json::JsonType::json_object);
    char ch = get_next_token();
    if (ch == '}')
    {
        return obj;
    }

    m_idx--;
    while (true)
    {
        ch = get_next_token();
        if (ch != '"')
        {
            throw std::logic_error("expected '\"' in object");
        }

        string key = parse_string();
        ch = get_next_token();
        if (ch != ':')
        {
            throw std::logic_error("expected ':' in object");
        }

        obj[key] = parse();
        ch = get_next_token();
        if (ch == '}')
        {
            break;
        }

        if (ch != ',')
        {
            throw std::logic_error("expected ',' in object");
        }
    }

    return obj;
}
