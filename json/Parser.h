#pragma once

#include <string>
using namespace std;

#include "Json.h"

namespace pan
{
    namespace json
    {
        // 解析器类
        class Parser
        {
            public:
                // 构造函数析构函数
                Parser();
                ~Parser();

                // 载入json文件和解析为Json类
                void load(const string &str);
                Json parse();

            private:
                void skip_white_space();                                // 解析，跳过空格
                char get_next_token();                                  // 获得下一个字符

                Json parse_null();                                      // 解析空类型
                Json parse_bool();                                      // 解析bool类型
                Json parse_number();                                    // 解析数据：int和double
                string parse_string();                                  // 解析字符串
                Json parse_array();                                     // 解析数组
                Json parse_object();                                    // 解析对象

                // 判断x是否在范围lower和upper之间
                bool in_range(int x, int lower, int upper)
                {
                    return (x >= lower && x <= upper);
                }

            private:
                string m_str;                                           // 保存json文件为字符串
                size_t m_idx;                                           // 指向字符串的位置
        };
    }
}
