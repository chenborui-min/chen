#ifndef PREFIX_EVALUATOR_H    // 防止头文件重复包含
#define PREFIX_EVALUATOR_H    // 定义头文件宏

#include <string>    // 包含字符串处理
#include <stack>     // 包含栈数据结构
#include <vector>    // 包含向量容器
using namespace std; // 使用标准命名空间

class PrefixEvaluator {    // 前缀表达式求值器类
private:
    stack<double> numberStack;    // 数字栈
    
    double evaluateOperation(double a, double b, char op);  // 执行运算操作
    void displayStack(const string& remainingExpr) const;   // 显示栈的状态
    void displayStep(const string& remainingExpr, const string& operation);  // 显示求值步骤
    bool isOperator(char c) const; // 判断是否为运算符
    bool isNumber(char c) const;   // 判断是否为数字
    bool isFunction(char c) const; // 判断是否为函数

public:
    double evaluate(const string& expression);  // 求值前缀表达式
    bool validateExpression(const string& expr) const;  // 验证前缀表达式格式
    std::string joinTokens(const std::vector<std::string>& tokens, int begin, int end) const;
};

#endif // PREFIX_EVALUATOR_H    // 结束头文件保护 