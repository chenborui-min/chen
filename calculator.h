#ifndef CALCULATOR_H    // 防止头文件重复包含
#define CALCULATOR_H    // 定义头文件宏

#include "infix_evaluator.h"     // 包含中缀表达式求值器
#include "prefix_evaluator.h"    // 包含前缀表达式求值器
#include "postfix_evaluator.h"   // 包含后缀表达式求值器
#include <string>               // 包含字符串处理
#include <stack>               // 包含栈数据结构
#include <map>                // 包含映射数据结构
#include <vector>             // 包含向量数据结构
#include <cmath>              // 包含数学函数

using namespace std;          // 使用标准命名空间

enum class ExpressionType {   // 表达式类型枚举类
    INFIX = 1,               // 中缀表达式
    PREFIX = 2,              // 前缀表达式
    POSTFIX = 3              // 后缀表达式
};

class Calculator {           // 计算器类
private:
    InfixEvaluator infixEvaluator;        // 中缀表达式求值器
    PrefixEvaluator prefixEvaluator;      // 前缀表达式求值器
    PostfixEvaluator postfixEvaluator;    // 后缀表达式求值器
    
    // 数字和运算符的栈
    stack<double> numberStack;    // 数字栈
    stack<char> operatorStack;    // 运算符栈
    
    // 运算符优先级和常量的映射
    map<char, int> precedence;           // 运算符优先级映射
    map<string, double> constants;       // 数学常量映射
    map<pair<char, char>, char> priorityTable;  // 运算符优先级表

    // 错误处理相关
    string errorMessage;                  // 错误信息
    bool hasError;                        // 是否有错误

    // 辅助函数
    void initPrecedence();      // 初始化运算符优先级
    void initConstants();       // 初始化数学常量
    void initPriorityTable();   // 初始化优先级表
    bool isNumber(char c) const;      // 判断是否为数字
    bool isFunction(char c) const;    // 判断是否为函数
    double evaluateOperation(double a, double b, char op);  // 执行运算操作
    void displayStacks(const string& remainingExpr) const;  // 显示栈的状态
    bool validateExpression(const string& expr) const;      // 验证表达式合法性
    
    // 错误处理函数
    void setError(const string& message);  // 设置错误信息
    void clearError();                     // 清除错误状态
    
    // 具体错误检查函数
    bool hasConsecutiveOperators(const string& expr) const;  // 检查连续运算符
    bool hasMismatchedParentheses(const string& expr) const; // 检查括号不匹配
    bool hasMissingOperator(const string& expr) const;       // 检查缺少运算符

public:
    Calculator();    // 构造函数
    
    // 表达式求值函数
    double evaluate(const string& expression, ExpressionType type);  // 根据类型求值表达式
    double evaluate(const string& expression);  // 默认使用中缀表达式求值
    void displayStep(const string& remainingExpr, const string& operation);  // 显示求值步骤
    
    // 辅助函数
    bool isOperator(char c) const;    // 判断是否为运算符
    
    // 获取支持的数学常量和运算符优先级
    const map<string, double>& getConstants() const;    // 获取数学常量映射
    const map<char, int>& getPrecedence() const;        // 获取运算符优先级映射

    // 错误处理相关函数
    bool hasErrorOccurred() const { return hasError; }  // 检查是否有错误
    string getErrorMessage() const { return errorMessage; }  // 获取错误信息
};

#endif // CALCULATOR_H    // 结束头文件保护 