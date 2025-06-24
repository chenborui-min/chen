#ifndef UTILS_H    // 防止头文件重复包含
#define UTILS_H    // 定义头文件宏

#include <string>   // 包含字符串处理
#include <vector>   // 包含向量数据结构
#include <map>      // 包含映射数据结构
using namespace std;  // 使用标准命名空间

class Utils {    // 工具类
public:
    // 字符串处理
    static bool isOperator(char c);    // 判断字符是否为运算符
    static bool isNumber(char c);      // 判断字符是否为数字
    static bool isFunction(char c);    // 判断字符是否为函数
    static bool isSpace(char c);       // 判断字符是否为空白字符
    static bool isBracket(char c);     // 判断字符是否为括号
    
    // 表达式验证
    static bool validateExpression(const string& expr);    // 验证表达式合法性
    static bool checkBracketMatch(const string& expr);     // 检查括号匹配
    
    // 表达式转换
    static string standardizeBrackets(const string& expr);  // 标准化括号格式
    static string infixToPostfix(const string& expr);       // 中缀转后缀
    static string infixToPrefix(const string& expr);        // 中缀转前缀
    static string postfixToInfix(const string& expr);       // 后缀转中缀
    static string prefixToInfix(const string& expr);        // 前缀转中缀
    
    // 数学常量
    static const map<string, double> CONSTANTS;    // 数学常量映射表
    
    // 运算符优先级
    static const map<char, int> PRECEDENCE;    // 运算符优先级映射表
    
private:
    static void initConstants();     // 初始化数学常量
    static void initPrecedence();    // 初始化运算符优先级
};

#endif // UTILS_H    // 结束头文件保护 