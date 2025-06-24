#ifndef POSTFIX_EVALUATOR_H    // 防止头文件重复包含
#define POSTFIX_EVALUATOR_H    // 定义头文件宏

#include <string>    // 包含字符串处理
#include <stack>     // 包含栈数据结构
using namespace std; // 使用标准命名空间

class PostfixEvaluator {    // 后缀表达式求值器类
private:
    stack<double> numberStack;    // 数字栈
    
    double evaluateOperation(double a, double b, char op);  // 执行运算操作
    void displayStack(const string& remainingExpr) const;   // 显示栈的状态
    void displayStep(const string& remainingExpr, const string& operation);  // 显示求值步骤
    bool isOperator(char c) const; // 判断是否为运算符
    bool isNumber(char c) const;   // 判断是否为数字
    bool isFunction(char c) const; // 判断是否为函数

public:
    double evaluate(const string& expression);  // 求值后缀表达式
    bool validateExpression(const string& expr) const;  // 验证后缀表达式格式
};

#endif // POSTFIX_EVALUATOR_H    // 结束头文件保护 