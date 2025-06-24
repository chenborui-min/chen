#include "infix_evaluator.h"    // 包含中缀表达式求值器头文件
#include <iostream>          
#include <iomanip>           
#include <sstream>           
#include <cmath>             
#include <stack>            
#include <vector>          

using namespace std;         

double InfixEvaluator::evaluateOperation(double a, double b, char op) const {    // 执行运算操作
    switch (op) {
        case '+': return a + b;                
        case '-': return a - b;                
        case '*': return a * b;                
        case '/':                              
            if (b == 0) throw runtime_error("除数不能为零 (Division by zero)");    
            return a / b;
        case '%':                              
            if (b == 0) throw runtime_error("除数不能为零 (Division by zero)");    
            return fmod(a, b);
        case '^': return pow(a, b);         
        case '&': return static_cast<int>(a) & static_cast<int>(b);   
        case '|': return static_cast<int>(a) | static_cast<int>(b);    
        case 's': return sin(b);               
        case 'c': return cos(b);               
        case 't': return tan(b);               
        case 'l':                              
            if (b <= 0) throw runtime_error("对数函数的参数必须大于零 (Logarithm argument must be positive)");    
            return log(b);
        default: throw runtime_error("未知运算符 (Unknown operator)");    
    }
}

void InfixEvaluator::displayStacks(const string& remainingExpr) const {    // 显示栈的状态
    cout << "剩余表达式（Remaining expression）: " << remainingExpr << endl;    // 显示剩余表达式
    
    // 显示数字栈
    cout << "数字栈（Number stack）: "<< endl;
    stack<double> tempNum = numberStack;
    vector<double> nums;
    while (!tempNum.empty()) {    // 将栈中元素复制到向量中
        nums.push_back(tempNum.top());
        tempNum.pop();
    }
    for (auto it = nums.begin(); it != nums.end(); ++it) {    // 从栈顶到栈底显示
        cout << "|" << fixed << setprecision(2) << *it << "|" << endl;
    }
    cout << endl;
    
    // 显示运算符栈
    cout << "运算符栈（Operator stack）: "<< endl;
    stack<char> tempOp = operatorStack;
    vector<char> ops;
    while (!tempOp.empty()) {    // 将栈中元素复制到向量中
        ops.push_back(tempOp.top());
        tempOp.pop();
    }
    for (auto it = ops.begin(); it != ops.end(); ++it) {    // 从栈顶到栈底显示
        cout << "|" << *it << "|" << endl;
    }
    cout << endl;
    cout << "----------------------------------------" << endl;
}

void InfixEvaluator::displayStep(const string& remainingExpr, const string& operation) {    // 显示计算步骤
    cout << "执行操作 (Operation)："<< operation << endl;
    displayStacks(remainingExpr);
}

double InfixEvaluator::evaluate(const string& expression) {    // 求值中缀表达式
    // 清空栈
    while (!numberStack.empty()) numberStack.pop();
    while (!operatorStack.empty()) operatorStack.pop();
    string expr = expression;
    string remainingExpr = expr;    // 用于显示剩余表达式
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        remainingExpr = expr.substr(i);    // 更新剩余表达式
        
        if (isspace(c)) continue;
        
        // 处理函数调用
        if (isFunction(c)) {
            if (i + 1 < expr.length() && expr[i + 1] == '(') {
                operatorStack.push(c);
                displayStep(remainingExpr, string("压入函数 (Push function): ") + c);
                continue;
            }
        }
        
        // 处理常量
        if (c == 'p' && i + 1 < expr.length() && expr[i + 1] == 'i') {
            numberStack.push(3.14159265358979323846);
            i++; // 跳过'i'
            displayStep(remainingExpr, "压入常量 (Push constant): pi");
            continue;
        }
        if (c == 'e' && (i == 0 || !isNumber(expr[i-1]))) {
            numberStack.push(2.71828182845904523536);
            displayStep(remainingExpr, "压入常量 (Push constant): e");
            continue;
        }
        
        // 处理数字（包括负数和小数）
        if (isdigit(c) || c == '.' || 
            (c == '-' && (i == 0 || (i > 0 && (isspace(expr[i-1]) || expr[i-1] == '(' || expr[i-1] == '{' || expr[i-1] == '[' || isOperator(expr[i-1])))))) {
            string numStr;
            if (c == '-') {
                numStr += c;
                i++;
                if (i >= expr.length() || (!isdigit(expr[i]) && expr[i] != '.')) {
                    i--;
                    operatorStack.push(c);    // 如果不是负数，而是减号，则作为运算符处理
                    displayStep(remainingExpr, string("压入运算符 (Push operator): ") + c);
                    continue;
                }
            }
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.' || expr[i] == 'e' || expr[i] == 'E' || 
                   (expr[i] == '-' && i > 0 && (expr[i-1] == 'e' || expr[i-1] == 'E')))) {
                numStr += expr[i++];    
            }
            i--;   
            numberStack.push(stod(numStr));    
            displayStep(remainingExpr, "压入数字 (Push number): " + numStr);
        }
        // 处理运算符（包括位运算）
        else if (isOperator(c)) {
            while (!operatorStack.empty() && operatorStack.top() != '(' && operatorStack.top() != '{' && operatorStack.top() != '[' && precedence(operatorStack.top()) >= precedence(c)) {
                double b = numberStack.top(); numberStack.pop();    
                double a = numberStack.top(); numberStack.pop();    
                char op = operatorStack.top(); operatorStack.pop(); 
                numberStack.push(evaluateOperation(a, b, op));    
                displayStep(remainingExpr, string("执行运算 (Calculate): ") + to_string(a) + string(1, op) + to_string(b));
            }
            operatorStack.push(c);    // 将当前运算符压入栈
            displayStep(remainingExpr, string("压入运算符 (Push operator): ") + c);
        }
        // 处理括号
        else if (c == '(' || c == '{' || c == '[') {    // 左括号直接压栈
            operatorStack.push(c);
            string msg = "压入左括号 (Push left parenthesis)";
            if (c == '{') msg = "压入左大括号 (Push left curly bracket)";
            if (c == '[') msg = "压入左中括号 (Push left square bracket)";
            displayStep(remainingExpr, msg);
        }
        else if (c == ')' || c == '}' || c == ']') {    // 处理右括号
            char match = (c == ')') ? '(' : (c == '}') ? '{' : '[';
            while (!operatorStack.empty() && operatorStack.top() != match) {
                double b = numberStack.top(); numberStack.pop();    
                double a = numberStack.top(); numberStack.pop();    
                char op = operatorStack.top(); operatorStack.pop(); 
                numberStack.push(evaluateOperation(a, b, op));    
                displayStep(remainingExpr, string("执行运算 (Calculate): ") + to_string(a) + string(1, op) + to_string(b));
            }
            if (!operatorStack.empty() && operatorStack.top() == match) {
                operatorStack.pop();    // 移除左括号
                string msg = "移除左括号 (Remove left parenthesis)";
                if (match == '{') msg = "移除左大括号 (Remove left curly bracket)";
                if (match == '[') msg = "移除左中括号 (Remove left square bracket)";
                displayStep(remainingExpr, msg);
            } else {
                throw runtime_error("括号不匹配错误 (Mismatched parentheses)");
            }
            // 检查是否有函数符号在栈顶
            if (!operatorStack.empty() && isFunction(operatorStack.top())) {
                char func = operatorStack.top(); operatorStack.pop();
                if (numberStack.empty()) throw runtime_error("函数参数缺失 (Missing function argument)");
                double arg = numberStack.top(); numberStack.pop();
                double res = evaluateOperation(0, arg, func); // 一元函数只用b
                numberStack.push(res);
                displayStep(remainingExpr, string("执行函数 (Function): ") + func + "(" + to_string(arg) + ") = " + to_string(res));
            }
        }
    }
    
    // 处理剩余的运算符
    while (!operatorStack.empty()) {
        if (operatorStack.top() == '(' || operatorStack.top() == '{' || operatorStack.top() == '[') {
            throw runtime_error("括号不匹配错误 (Mismatched parentheses)");
        }
        double b = numberStack.top(); numberStack.pop();    
        double a = numberStack.top(); numberStack.pop();    
        char op = operatorStack.top(); operatorStack.pop(); 
        numberStack.push(evaluateOperation(a, b, op));    
        displayStep("", string("执行最终运算 (Final calculation): ") + to_string(a) + string(1, op) + to_string(b));
    }

    if (numberStack.size() != 1) {
        if (numberStack.size() > 1) {
            throw runtime_error("缺少运算符错误 (Missing operator)");
        } else if (numberStack.empty()) {
            throw runtime_error("表达式为空 (Empty expression)");
        }
    }

    double result = numberStack.top();
    displayStep("", "计算完成 (Calculation completed), 结果 (Result): " + to_string(result));
    return result;
}

int InfixEvaluator::precedence(char op) {    // 获取运算符优先级
    switch (op) {
        case '+':    
        case '-':   
            return 1;
        case '*':    
        case '/':    
        case '%':    
            return 2;
        case '^':    
            return 3;
        case '&':    
        case '|':    
            return 4;
        default:    
            return 0;
    }
}

bool InfixEvaluator::isOperator(char c) const {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
           c == '^' || c == '&' || c == '|' || c == 's' || c == 'c' || 
           c == 't' || c == 'l';
}

bool InfixEvaluator::isNumber(char c) const {
    return (c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E';
}

bool InfixEvaluator::isFunction(char c) const {
    return c == 's' || c == 'c' || c == 't' || c == 'l';
}

bool InfixEvaluator::validateExpression(const string& expr) const {
    int paren = 0, brace = 0, bracket = 0;
    bool lastWasOperator = true;
    bool lastWasNumber = false;
    bool hasOperator = false;
    bool hasNumber = false;
    bool hasDecimalPoint = false;
    size_t i = 0;
    while (i < expr.length()) {
        char c = expr[i];
        if (isspace(c)) { i++; continue; }
        if (isFunction(c) && i + 1 < expr.length() && expr[i + 1] == '(') {
            if (lastWasNumber) return false;
            hasOperator = true;
            lastWasOperator = true;
            lastWasNumber = false;
            hasDecimalPoint = false;
            paren++;
            i += 2;
            continue;
        }
        if (c == '(') { if (lastWasNumber) return false; paren++; lastWasOperator = true; lastWasNumber = false; hasDecimalPoint = false; i++; continue; }
        if (c == '{') { if (lastWasNumber) return false; brace++; lastWasOperator = true; lastWasNumber = false; hasDecimalPoint = false; i++; continue; }
        if (c == '[') { if (lastWasNumber) return false; bracket++; lastWasOperator = true; lastWasNumber = false; hasDecimalPoint = false; i++; continue; }
        if (c == ')') { if (lastWasOperator) return false; paren--; if (paren < 0) return false; lastWasOperator = false; lastWasNumber = true; hasDecimalPoint = false; i++; continue; }
        if (c == '}') { if (lastWasOperator) return false; brace--; if (brace < 0) return false; lastWasOperator = false; lastWasNumber = true; hasDecimalPoint = false; i++; continue; }
        if (c == ']') { if (lastWasOperator) return false; bracket--; if (bracket < 0) return false; lastWasOperator = false; lastWasNumber = true; hasDecimalPoint = false; i++; continue; }
        if (isOperator(c)) { if (lastWasOperator && c != '-') return false; hasOperator = true; lastWasOperator = true; lastWasNumber = false; hasDecimalPoint = false; i++; continue; }
        if (isNumber(c)) { if (c == '.' && hasDecimalPoint) return false; if (c == '.') hasDecimalPoint = true; hasNumber = true; lastWasOperator = false; lastWasNumber = true; i++; continue; }
        if (c == 'p' && i + 1 < expr.length() && expr[i + 1] == 'i') { hasNumber = true; lastWasOperator = false; lastWasNumber = true; i += 2; hasDecimalPoint = false; continue; }
        if (c == 'e' && (i == 0 || !isNumber(expr[i-1]))) { hasNumber = true; lastWasOperator = false; lastWasNumber = true; hasDecimalPoint = false; i++; continue; }
        return false;
    }
    if (paren != 0 || brace != 0 || bracket != 0) return false;
    if (lastWasOperator) return false;
    if (!hasNumber) return false;
    return true;
} 