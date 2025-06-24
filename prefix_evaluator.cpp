#include "prefix_evaluator.h"    // 包含前缀表达式求值器头文件
#include <iostream>           
#include <iomanip>           
#include <sstream>           
#include <cmath>             
#include <algorithm>         
#include <stack>             
#include <vector>            
#include <cctype>

using namespace std;         

double PrefixEvaluator::evaluateOperation(double a, double b, char op) {    // 执行运算操作
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

void PrefixEvaluator::displayStack(const string& remainingExpr) const {    // 显示栈的状态
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

void PrefixEvaluator::displayStep(const string& remainingExpr, const string& operation) {    // 显示计算步骤
    cout << "执行操作 (Operation)： " << operation << endl;
    displayStack(remainingExpr);
}
/*从右向左扫描表达式。（这是与前缀计算的关键区别）
如果遇到操作数，则将其压入栈中。
如果遇到运算符：
从栈中弹出所需数量的操作数（对于二元运算符是 2 个，一元运算符是 1 个）。
使用该运算符对弹出的操作数进行计算
将计算结果压回栈中。
重复以上步骤，直到扫描完整个表达式。
栈中最后剩下的唯一元素就是整个表达式的最终结果。*/
double PrefixEvaluator::evaluate(const string& expression) {    // 求值前缀表达式
    // 清空栈
    while (!numberStack.empty()) numberStack.pop();
    // 按空格分割token
    vector<string> tokens;
    istringstream iss(expression);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    string remainingExpr = expression;
    
    // 从右向左处理token
    for (int i = tokens.size() - 1; i >= 0; --i) {
        const string& tk = tokens[i];
        // 判断是否为数字（支持负数、小数、科学计数法）
        bool isNum = false;
        if (!tk.empty()) {
            size_t idx = 0;
            if (tk[0] == '-') idx = 1;
            bool hasDot = false, hasE = false;
            for (; idx < tk.size(); ++idx) {
                if (isdigit(tk[idx])) continue;
                if (tk[idx] == '.' && !hasDot) { hasDot = true; continue; }
                if ((tk[idx] == 'e' || tk[idx] == 'E') && !hasE) { hasE = true; continue; }
                if ((tk[idx] == '+' || tk[idx] == '-') && idx > 0 && (tk[idx-1] == 'e' || tk[idx-1] == 'E')) continue;
                break;
            }
            if (idx == tk.size()) isNum = true;
        }
        if (isNum) {
            double num = stod(tk);
            numberStack.push(num);
            displayStep(joinTokens(tokens, 0, i), "压入数字 (Push number): " + to_string(num));
        } else if (tk.size() == 1 && isOperator(tk[0])) {
            if (numberStack.size() < 2) {
                throw runtime_error("表达式有误，操作数不足 (Insufficient operands)");
            }
            double a = numberStack.top(); numberStack.pop();
            double b = numberStack.top(); numberStack.pop();
            numberStack.push(evaluateOperation(a, b, tk[0]));
            displayStep(joinTokens(tokens, 0, i), string("执行运算 (Calculate): ") + to_string(a) + string(1, tk[0]) + to_string(b));
        } else {
            throw runtime_error("无效的token: " + tk);
        }
    }
    if (numberStack.size() != 1) {
        throw runtime_error("表达式有误，栈中剩余多余元素 (Invalid expression, extra elements in stack)");
    }
    double result = numberStack.top();
    displayStep("", "计算完成 (Calculation completed), 结果 (Result): " + to_string(result));
    return result;
}

bool PrefixEvaluator::isOperator(char c) const {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
           c == '^' || c == '&' || c == '|' || c == 's' || c == 'c' || 
           c == 't' || c == 'l';
}

bool PrefixEvaluator::isNumber(char c) const {
    return (c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E';
}

bool PrefixEvaluator::isFunction(char c) const {
    return c == 's' || c == 'c' || c == 't' || c == 'l';
}

bool PrefixEvaluator::validateExpression(const string& expr) const {
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

// 辅助函数：将tokens[begin, end)拼接为字符串
string PrefixEvaluator::joinTokens(const vector<string>& tokens, int begin, int end) const {
    string res;
    for (int i = begin; i < end; ++i) {
        if (i > begin) res += " ";
        res += tokens[i];
    }
    return res;
} 