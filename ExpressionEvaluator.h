#ifndef EXPRESSION_EVALUATOR_H    // 防止头文件重复包含
#define EXPRESSION_EVALUATOR_H    // 定义头文件宏

#include <iostream>   // 输入输出流
#include <stack>      // 栈数据结构
#include <string>     // 字符串处理
#include <vector>     // 向量数据结构
#include <cmath>      // 数学函数
#include <map>        // 映射数据结构
#include <sstream>    // 字符串流
#include <iomanip>    // 输入输出格式控制
#include <algorithm>  // 算法库
#include <cctype>     // 字符类型判断
#include <memory>     // 智能指针
#include <stdexcept>  // 标准异常

using namespace std;  // 使用标准命名空间

// 数学常量定义
const double PI = 3.14159265358979323846;    // 圆周率
const double E = 2.71828182845904523536;     // 自然对数的底

// 运算符优先级定义
enum Priority {
    LOW = 1,        // 低优先级 (+, -, &, |)
    MEDIUM = 2,     // 中优先级 (*, /, %)
    HIGH = 3,       // 高优先级 (^)
    VERY_HIGH = 4   // 最高优先级 (函数: sin, cos, tan, log)
};

// 表达式类型枚举
enum ExpressionType {
    INFIX,      // 中缀表达式（例如：3 + 4）
    PREFIX,     // 前缀表达式（例如：+ 3 4）
    POSTFIX     // 后缀表达式（例如：3 4 +）
};

// 错误类型枚举
enum ErrorType {
    NO_ERROR,                // 无错误
    MISMATCHED_PARENTHESES, // 括号不匹配
    INVALID_CHARACTER,      // 非法字符
    CONSECUTIVE_OPERATORS,  // 连续运算符
    DIVISION_BY_ZERO,      // 除零错误
    INVALID_EXPRESSION,    // 非法表达式
    EMPTY_EXPRESSION,      // 空表达式
    INVALID_NEGATIVE_NUMBER, // 无效的负数格式
    INSUFFICIENT_OPERANDS,   // 操作数不足
    FUNCTION_ARGUMENT_ERROR, // 函数参数错误
    MISSING_OPERATOR        // 缺少运算符
};

// 基础表达式求值类
class ExpressionEvaluator {
protected:
    stack<double> numberStack;    // 数字栈
    string remainingExpr;         // 剩余表达式
    
public:
    virtual ~ExpressionEvaluator() = default;  // 虚析构函数
    
    // 纯虚函数，由子类实现具体的求值逻辑
    virtual double evaluate(const string& expr) = 0;  // 表达式求值接口
    virtual bool validateExpression(const string& expr) const = 0;  // 验证表达式合法性
    
protected:
    // 工具方法
    virtual bool isOperator(char c) const;    // 判断是否为运算符
    virtual bool isNumber(char c) const;      // 判断是否为数字
    virtual bool isFunction(char c) const;    // 判断是否为函数
    virtual double evaluateOperation(double a, double b, char op) const;  // 执行运算操作
    
    // 显示方法
    virtual void displayStack(const string& remainingExpr) const;   // 显示栈状态
    virtual void displayStep(const string& remainingExpr, const string& operation);  // 显示计算步骤
    
    // 数字解析方法
    virtual bool isValidNumber(const string& numStr) const;  // 验证数字格式
    virtual double parseNumber(const string& numStr) const;  // 解析数字字符串
};

// 中缀表达式求值类
class InfixEvaluator : public ExpressionEvaluator {
private:
    stack<char> operatorStack;    // 运算符栈
    
public:
    double evaluate(const string& expr) override;  // 中缀表达式求值
    bool validateExpression(const string& expr) const override;  // 验证中缀表达式合法性
    
private:
    int precedence(char op) const;    // 获取运算符优先级
    void displayStacks(const string& remainingExpr) const;  // 显示双栈状态
    bool isConstant(const string& expr, size_t& pos) const;  // 检查是否为常量
    bool isFunctionCall(const string& expr, size_t& pos) const;  // 检查是否为函数调用
    void processFunction(char func);  // 处理函数调用
};

// 前缀表达式求值类
class PrefixEvaluator : public ExpressionEvaluator {
public:
    double evaluate(const string& expr) override;  // 前缀表达式求值
    bool validateExpression(const string& expr) const override;  // 验证前缀表达式合法性
    
private:
    string joinTokens(const vector<string>& tokens, int begin, int end) const;  // 连接tokens
    vector<string> tokenize(const string& expr) const;  // 分割表达式为tokens
    bool isValidToken(const string& token) const;  // 验证token有效性
};

// 后缀表达式求值类
class PostfixEvaluator : public ExpressionEvaluator {
public:
    double evaluate(const string& expr) override;  // 后缀表达式求值
    bool validateExpression(const string& expr) const override;  // 验证后缀表达式合法性
    
private:
    bool parseNumberFromExpression(const string& expr, size_t& pos, double& num) const;  // 从表达式中解析数字
};

// 表达式类型检测器
class ExpressionTypeDetector {
public:
    static ExpressionType detectType(const string& expr);          // 检测表达式类型
    static bool isInfixExpression(const string& expr);            // 判断是否为中缀表达式
    static bool isPrefixExpression(const string& expr);           // 判断是否为前缀表达式
    static bool isPostfixExpression(const string& expr);          // 判断是否为后缀表达式
    
private:
    static bool containsOperators(const string& expr);            // 检查是否包含运算符
    static bool hasBalancedParentheses(const string& expr);       // 检查括号是否平衡
    static bool startsWithOperator(const string& expr);           // 检查是否以运算符开始
};

// 表达式验证器
class ExpressionValidator {
public:
    static bool validateInfixExpression(const string& expr);      // 验证中缀表达式
    static bool validatePrefixExpression(const string& expr);     // 验证前缀表达式
    static bool validatePostfixExpression(const string& expr);    // 验证后缀表达式
    
private:
    static bool hasMismatchedParentheses(const string& expr);     // 检查括号不匹配
    static bool hasConsecutiveOperators(const string& expr);      // 检查连续运算符
    static bool hasMissingOperator(const string& expr);           // 检查缺少运算符
    static bool hasInvalidCharacters(const string& expr);         // 检查非法字符
    static bool hasValidNumberFormat(const string& expr);         // 检查数字格式
};

// 表达式转换器
class ExpressionConverter {
public:
    static string infixToPostfix(const string& infix);           // 中缀转后缀
    static string infixToPrefix(const string& infix);            // 中缀转前缀
    static string postfixToInfix(const string& postfix);         // 后缀转中缀
    static string prefixToInfix(const string& prefix);           // 前缀转中缀
    
private:
    static int getPrecedence(char op);                           // 获取运算符优先级
    static bool isOperator(char c);                              // 判断是否为运算符
    static bool isNumber(char c);                                // 判断是否为数字
    static bool isLeftAssociative(char op);                      // 判断运算符左结合性
};

// 数学函数管理器
class MathFunctionManager {
public:
    static double evaluateFunction(char func, double arg);       // 计算函数值
    static bool isValidFunction(char func);                      // 验证函数有效性
    static string getFunctionName(char func);                    // 获取函数名称
    static int getFunctionArity(char func);                      // 获取函数参数个数
    
private:
    static const map<char, string> functionNames;                // 函数名称映射
    static const map<char, int> functionArity;                   // 函数参数个数映射
};

// 常量管理器
class ConstantManager {
public:
    static double getConstant(const string& name);               // 获取常量值
    static bool isValidConstant(const string& name);             // 验证常量有效性
    static vector<string> getAllConstants();                     // 获取所有常量名称
    
private:
    static const map<string, double> constants;                  // 常量映射
};

// 主控制器类
class ExpressionController {
private:
    unique_ptr<ExpressionEvaluator> evaluator;  // 表达式求值器指针
    ExpressionType currentType;                 // 当前表达式类型
    
public:
    ExpressionController();   // 构造函数
    ~ExpressionController();  // 析构函数
    
    // 核心功能
    double evaluateExpression(const string& expr, ExpressionType type);  // 求值表达式
    bool validateExpression(const string& expr, ExpressionType type);    // 验证表达式
    string convertExpression(const string& expr, ExpressionType from, ExpressionType to);  // 转换表达式
    
    // 交互功能
    void runInteractiveMode();   // 运行交互模式
    void displayHelp();          // 显示帮助信息
    void displayExamples();      // 显示示例
    void displaySupportedFeatures();  // 显示支持的功能
    
    // 设置功能
    void setPrecision(int precision);  // 设置输出精度
    void setDisplayMode(bool showSteps);  // 设置是否显示计算步骤
    
private:
    int outputPrecision;         // 输出精度
    bool showCalculationSteps;   // 是否显示计算步骤
    
    void initializeEvaluator(ExpressionType type);  // 初始化求值器
    void displayResult(double result);              // 显示结果
    void displayError(const string& error);         // 显示错误
};

// 工具函数
namespace ExpressionUtils {
    // 字符串处理
    string trim(const string& str);                    // 去除首尾空格
    string toLowerCase(const string& str);             // 转换为小写
    vector<string> split(const string& str, char delimiter);  // 分割字符串
    
    // 数字处理
    bool isInteger(const string& str);                 // 判断是否为整数
    bool isDecimal(const string& str);                 // 判断是否为小数
    bool isScientificNotation(const string& str);      // 判断是否为科学计数法
    bool isNegativeNumber(const string& str);          // 判断是否为负数
    
    // 表达式处理
    string removeSpaces(const string& expr);           // 移除空格
    string standardizeBrackets(const string& expr);    // 标准化括号
    bool hasBalancedBrackets(const string& expr);      // 检查括号平衡
    
    // 格式化输出
    string formatNumber(double num, int precision = 2);  // 格式化数字
    string formatExpression(const string& expr);         // 格式化表达式
}

#endif // EXPRESSION_EVALUATOR_H    // 结束头文件保护