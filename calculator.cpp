#include "calculator.h"    // 包含计算器头文件
#include <iostream>      
#include <iomanip>      
#include <sstream>       
#include <algorithm>     
#include "utils.h"       // 工具函数

using namespace std;     

Calculator::Calculator() {   
    initPrecedence();      
    initConstants();         
    initPriorityTable();     
    clearError();           
}

void Calculator::initPrecedence() {    
    precedence['+'] = 1;    
    precedence['-'] = 1;    
    precedence['*'] = 2;    
    precedence['/'] = 2;    
    precedence['%'] = 2;    
    precedence['^'] = 3;    
    precedence['&'] = 1;   
    precedence['|'] = 1;   
    precedence['s'] = 4;    
    precedence['c'] = 4;    
    precedence['t'] = 4;    
    precedence['l'] = 4;    
}

void Calculator::initConstants() {    // 初始化数学常量
    constants["pi"] = 3.14159265358979323846;    
    constants["e"] = 2.71828182845904523536;    
}

void Calculator::initPriorityTable() {    // 初始化优先级表（无使用）
    // 设置运算符优先级关系
    // '>' 表示优先级高，'<' 表示优先级低，'=' 表示优先级相等
    vector<char> ops = {'+', '-', '*', '/', '%', '^', '(', ')', '&', '|'};    // 所有运算符
    for (char op1 : ops) {   
        for (char op2 : ops) {    
            if (op1 == '(' && op2 == ')') {    
                priorityTable[{op1, op2}] = '=';
            } else if (op1 == '(' || op2 == '(') {    
                priorityTable[{op1, op2}] = '<';
            } else if (op1 == ')' || op2 == ')') {    
                priorityTable[{op1, op2}] = '>';
            } else {    
                if (precedence[op1] > precedence[op2]) {
                    priorityTable[{op1, op2}] = '>';
                } else if (precedence[op1] < precedence[op2]) {
                    priorityTable[{op1, op2}] = '<';
                } else {
                    priorityTable[{op1, op2}] = '=';
                }
            }
        }
    }
}

bool Calculator::isOperator(char c) const {    
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
           c == '^' || c == '&' || c == '|' || c == 's' || c == 'c' || 
           c == 't' || c == 'l';
}

bool Calculator::isNumber(char c) const {   
    return (c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E';
}

bool Calculator::isFunction(char c) const {   
    return c == 's' || c == 'c' || c == 't' || c == 'l';
}

double Calculator::evaluateOperation(double a, double b, char op) {    // 执行运算操作
    switch (op) {
        case '+': return a + b;               
        case '-': return a - b;                
        case '*': return a * b;                
        case '/':                            
            if (b == 0) throw runtime_error("除数不可为零。Division by zero");    
            return a / b;
        case '%': return fmod(a, b);           
        case '^': return pow(a, b);          
        case '&': return static_cast<int>(a) & static_cast<int>(b);   
        case '|': return static_cast<int>(a) | static_cast<int>(b);   
        case 's': return sin(b);              
        case 'c': return cos(b);            
        case 't': return tan(b);               
        case 'l': return log(b);             
        default: throw runtime_error("未知运算符。Unknown operator");  
    }
}

void Calculator::displayStacks(const string& remainingExpr) const {    // 显示栈的状态
    cout << "\n当前状态 (Current State):" << endl;
    cout << "剩余表达式 (Remaining): " << remainingExpr << endl;   
    
    // 显示数字栈
    cout << "数字栈 (Numbers): |";
    stack<double> tempNum = numberStack;
    vector<double> nums;
    while (!tempNum.empty()) {    // 将栈中元素复制到向量中
        nums.push_back(tempNum.top());
        tempNum.pop();
    }
    for (auto it = nums.rbegin(); it != nums.rend(); ++it) {    // 按原始顺序显示
        cout << fixed << setprecision(2) << *it << "|";
    }
    cout << endl;
    
    // 显示运算符栈
    cout << "运算符栈 (Operators): |";
    stack<char> tempOp = operatorStack;
    vector<char> ops;
    while (!tempOp.empty()) {    // 将栈中元素复制到向量中
        ops.push_back(tempOp.top());
        tempOp.pop();
    }
    for (auto it = ops.rbegin(); it != ops.rend(); ++it) {    // 按原始顺序显示
        cout << *it << "|";
    }
    cout << endl;
    cout << "----------------------------------------" << endl;
}

void Calculator::displayStep(const string& remainingExpr, const string& operation) {    // 显示计算步骤
    cout << "\n执行操作 (Operation): " << operation << endl;
    displayStacks(remainingExpr);
}

void Calculator::setError(const string& message) {    // 设置错误信息
    hasError = true;
    errorMessage = message;
    cout << "\n错误 (Error): " << message << endl;    // 立即显示错误信息
}

void Calculator::clearError() {    // 清除错误状态
    hasError = false;
    errorMessage.clear();
}

double Calculator::evaluate(const string& expression, ExpressionType type) {    // 根据类型求值表达式
    clearError();    // 清除之前的错误状态
    
    try {
        // 按表达式类型分别验证
        bool valid = false;
        string errorType = "";
        
        switch (type) {
            case ExpressionType::INFIX:
                valid = infixEvaluator.validateExpression(expression);
                if (!valid) {
                    // 检查具体的错误类型
                    if (hasConsecutiveOperators(expression)) {
                        setError("连续运算符错误 (Consecutive operators error)");
                    } else if (hasMismatchedParentheses(expression)) {
                        setError("括号不匹配错误 (Mismatched parentheses error)");
                    } else if (hasMissingOperator(expression)) {
                        setError("缺少运算符错误 (Missing operator error)");
                    } else {
                        setError("无效的表达式格式 (Invalid expression format)");
                    }
                    return 0.0;
                }
                return infixEvaluator.evaluate(expression);
            case ExpressionType::PREFIX:
                valid = prefixEvaluator.validateExpression(expression);
                if (!valid) {
                    setError("无效的前缀表达式格式 (Invalid prefix expression format)");
                    return 0.0;
                }
                return prefixEvaluator.evaluate(expression);
            case ExpressionType::POSTFIX:
                valid = postfixEvaluator.validateExpression(expression);
                if (!valid) {
                    setError("无效的后缀表达式格式 (Invalid postfix expression format)");
                    return 0.0;
                }
                return postfixEvaluator.evaluate(expression);
            default:
                setError("未知的表达式类型 (Unknown expression type)");
                return 0.0;
        }
    } catch (const runtime_error& e) {
        setError(e.what());
        return 0.0;
    } catch (...) {
        setError("计算过程中发生未知错误 (Unknown error occurred during calculation)");
        return 0.0;
    }
}

double Calculator::evaluate(const string& expression) {    // 默认求值中缀表达式
    return evaluate(expression, ExpressionType::INFIX);
}

bool Calculator::validateExpression(const string& expr) const {
    int parentheses = 0;
    bool lastWasOperator = true;
    bool lastWasNumber = false;
    bool hasOperator = false;
    bool hasNumber = false;
    bool hasDecimalPoint = false;
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        if (isspace(c)) continue;
        
        if (c == '(') {
            if (lastWasNumber) {
                return false;
            }
            parentheses++;
            lastWasOperator = true;
            lastWasNumber = false;
            hasDecimalPoint = false;
        }
        else if (c == ')') {
            if (lastWasOperator) {
                return false;
            }
            parentheses--;
            if (parentheses < 0) return false;
            lastWasOperator = false;
            lastWasNumber = true;
            hasDecimalPoint = false;
        }
        else if (isOperator(c)) {
            if (lastWasOperator && c != '-') {
                return false;
            }
            hasOperator = true;
            lastWasOperator = true;
            lastWasNumber = false;
            hasDecimalPoint = false;
        }
        else if (isdigit(c)) {
            if (c == '.' && hasDecimalPoint) {
                return false;
            }
            if (c == '.') {
                hasDecimalPoint = true;
            }
            hasNumber = true;
            lastWasOperator = false;
            lastWasNumber = true;
        }
        else if (isFunction(c)) {
            if (lastWasNumber) {
                return false;
            }
            hasOperator = true;
            lastWasOperator = true;
            lastWasNumber = false;
            hasDecimalPoint = false;
        }
        else if (c == 'p' && i + 1 < expr.length() && expr[i + 1] == 'i') {
            hasNumber = true;
            lastWasOperator = false;
            lastWasNumber = true;
            i++; // 跳过'i'
            hasDecimalPoint = false;
        }
        else if (c == 'e' && (i == 0 || !isdigit(expr[i-1]))) {
            hasNumber = true;
            lastWasOperator = false;
            lastWasNumber = true;
            hasDecimalPoint = false;
        }
        else {
            return false;
        }
    }
    
    return parentheses == 0 && !lastWasOperator && hasNumber;
}

const map<string, double>& Calculator::getConstants() const {    // 获取数学常量映射
    return constants;
}

const map<char, int>& Calculator::getPrecedence() const {    // 获取运算符优先级映射
    return precedence;
}

// 检查是否有连续运算符
bool Calculator::hasConsecutiveOperators(const string& expr) const {
    bool lastWasOperator = true;
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (isspace(c)) continue;
        // 如果是函数名且后面是'('，不算连续运算符
        if (isFunction(c) && i + 1 < expr.length() && expr[i + 1] == '(') {
            lastWasOperator = false;
            continue;
        }
        if (isOperator(c)) {
            if (lastWasOperator && c != '-') {
                return true;
            }
            lastWasOperator = true;
        } else if (isdigit(c) || c == '.' || c == '(' || c == 'p' || c == 'e') {
            lastWasOperator = false;
        }
    }
    return false;
}

// 检查括号是否匹配
bool Calculator::hasMismatchedParentheses(const string& expr) const {
    int parentheses = 0;
    for (char c : expr) {
        if (c == '(') parentheses++;
        else if (c == ')') parentheses--;
        if (parentheses < 0) return true;
    }
    return parentheses != 0;
}

// 检查是否缺少运算符
bool Calculator::hasMissingOperator(const string& expr) const {
    bool hasOperator = false;
    bool hasNumber = false;
    bool lastWasNumber = false;
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (isspace(c)) continue;
        
        if (isdigit(c) || c == '.' || c == 'p' || c == 'e') {
            hasNumber = true;
            if (lastWasNumber) {
                return true; // 两个数字之间没有运算符
            }
            lastWasNumber = true;
        } else if (isOperator(c)) {
            hasOperator = true;
            lastWasNumber = false;
        } else if (c == '(' || c == ')') {
            lastWasNumber = false;
        }
    }
    
    return hasNumber && !hasOperator;
}