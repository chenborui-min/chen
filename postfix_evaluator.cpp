#include "postfix_evaluator.h"    // 包含后缀表达式求值器头文件
#include <iostream>           
#include <iomanip>          
#include <sstream>          
#include <cmath>            
#include <stack>            
#include <vector>            

using namespace std;        

double PostfixEvaluator::evaluateOperation(double a, double b, char op) {    // 执行运算操作
    switch (op) {
        case '+': return a + b;                
        case '-': return a - b;                
        case '*': return a * b;                
        case '/':                             
            if (b == 0) throw runtime_error("除数不能为零 (Division by zero)");    
            return a / b;
        case '%': return fmod(a, b);           
        case '^': return pow(a, b);           
        case '&': return static_cast<int>(a) & static_cast<int>(b);    
        case '|': return static_cast<int>(a) | static_cast<int>(b);   
        case 's': return sin(b);               
        case 'c': return cos(b);              
        case 't': return tan(b);               
        case 'l': return log(b);               
        default: throw runtime_error("未知运算符 (Unknown operator)");    
    }
}

void PostfixEvaluator::displayStack(const string& remainingExpr) const {    // 显示栈的状态
    cout << "剩余表达式（Remaining expression）: " << remainingExpr << endl;    
    
    cout << "数字栈（Number stack）: " << endl;    // 显示数字栈
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
    cout << "----------------------------------------" << endl;
}

void PostfixEvaluator::displayStep(const string& remainingExpr, const string& operation) {    // 显示计算步骤
    cout << "执行操作 (Operation)： " << operation << endl;
    displayStack(remainingExpr);
}

double PostfixEvaluator::evaluate(const string& expression) {    // 求值后缀表达式
    // 清空栈
    while (!numberStack.empty()) numberStack.pop();
    string expr = expression;
    string remainingExpr = expr;
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        remainingExpr = expr.substr(i);
        
        if (isspace(c)) continue;
        
        // 处理数字（包括负数和小数）
        if (isdigit(c) || c == '-' || c == '.') {
            string numStr;
            bool isNegative = false;
            
            // 处理负号
            if (c == '-') {
                isNegative = true;
                i++;
                if (i >= expr.length() || (!isdigit(expr[i]) && expr[i] != '.')) {
                    throw runtime_error("无效的负数格式 (Invalid negative number format)");
                }
            }
            
            // 收集数字
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.' || expr[i] == 'e' || expr[i] == 'E' || 
                   (expr[i] == '-' && i > 0 && (expr[i-1] == 'e' || expr[i-1] == 'E')))) {
                numStr += expr[i++];
            }
            i--;
            
            double num = stod(numStr);
            if (isNegative) {
                num = -num;
            }
            numberStack.push(num);
            displayStep(remainingExpr, "压入数字 (Push number): " + to_string(num));
        }
        // 处理运算符
        else if (isOperator(c)) {
            if (numberStack.size() < 2) {
                throw runtime_error("表达式有误，操作数不足 (Insufficient operands)");
            }
            double b = numberStack.top(); numberStack.pop();
            double a = numberStack.top(); numberStack.pop();
            numberStack.push(evaluateOperation(a, b, c));
            displayStep(remainingExpr, string("执行运算 (Calculate): ") + to_string(a) + string(1, c) + to_string(b));
        }
    }
    
    if (numberStack.size() != 1) {
        throw runtime_error("表达式有误，栈中剩余多余元素 (Invalid expression, extra elements in stack)");
    }
    
    double result = numberStack.top();
    displayStep("", "计算完成 (Calculation completed), 结果 (Result): " + to_string(result));
    return result;
}

bool PostfixEvaluator::isOperator(char c) const {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
           c == '^' || c == '&' || c == '|' || c == 's' || c == 'c' || 
           c == 't' || c == 'l';
}

bool PostfixEvaluator::isNumber(char c) const {
    return (c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E';
}

bool PostfixEvaluator::isFunction(char c) const {
    return c == 's' || c == 'c' || c == 't' || c == 'l';
}

bool PostfixEvaluator::validateExpression(const string& expr) const {
    int numCount = 0;
    int opCount = 0;
    size_t i = 0;
    
    while (i < expr.length()) {
        // 跳过空格
        while (i < expr.length() && isspace(expr[i])) i++;
        if (i >= expr.length()) break;

        // 检查负数或小数
        bool isNegative = false;
        if (expr[i] == '-') {
            // 负号后必须是数字或小数点
            if (i + 1 < expr.length() && (isdigit(expr[i + 1]) || expr[i + 1] == '.')) {
                isNegative = true;
                i++;
            } else {
                return false;
            }
        }

        if (isdigit(expr[i]) || expr[i] == '.') {
            bool hasDot = false;
            if (expr[i] == '.') hasDot = true;
            i++;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') {
                    if (hasDot) return false; // 多个小数点
                    hasDot = true;
                }
                i++;
            }
            // 科学计数法
            if (i < expr.length() && (expr[i] == 'e' || expr[i] == 'E')) {
                i++;
                if (i < expr.length() && (expr[i] == '+' || expr[i] == '-')) i++;
                if (i >= expr.length() || !isdigit(expr[i])) return false;
                while (i < expr.length() && isdigit(expr[i])) i++;
            }
            numCount++;
        } else if (isOperator(expr[i])) {
            opCount++;
            i++;
        } else {
            return false;
        }
    }
    
    return numCount == opCount + 1 && numCount > 0;
} 