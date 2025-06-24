#include "utils.h"       // 包含工具类头文件
#include <stack>          
#include <algorithm>      
#include <stdexcept>     // 标准异常

using namespace std;     
//采用静态成员，不需要实例化对象，直接可以使用
const map<string, double> Utils::CONSTANTS = {    // 定义数学常量
    {"pi", 3.14159265358979323846},    // 圆周率
    {"e", 2.71828182845904523536}      // 自然对数的底
};

const map<char, int> Utils::PRECEDENCE = {    //优先级
    {'+', 1}, {'-', 1},               
    {'*', 2}, {'/', 2}, {'%', 2},      
    {'^', 3},                          
    {'&', 1}, {'|', 1},               
    {'s', 4}, {'c', 4}, {'t', 4}, {'l', 4},  // 函数（sin, cos, tan, log）
    {'(', 0}, {')', 0}               
};

bool Utils::isOperator(char c) {    
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
           c == '^' || c == '&' || c == '|' || c == 's' || c == 'c' || 
           c == 't' || c == 'l';
}

bool Utils::isNumber(char c) {   
    return (c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E';
}

bool Utils::isFunction(char c) {  
    return c == 's' || c == 'c' || c == 't' || c == 'l';
}

bool Utils::isSpace(char c) {  
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Utils::isBracket(char c) {  
    return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}';
}

string Utils::standardizeBrackets(const string& expr) {   
    string result;
    for (char c : expr) {
        if (c == '[' || c == '{') result += '(';      // 中大括号替换成小括号格式处理
        else if (c == ']' || c == '}') result += ')';
        else result += c;
    }
    return result;
}

bool Utils::checkBracketMatch(const string& expr) {    // 检查括号匹配
    stack<char> brackets;    // 括号栈
    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {    // 入栈
            brackets.push(c);
        } else if (c == ')' || c == ']' || c == '}') {    
            if (brackets.empty()) return false;    // 右括号多余
            
            char left = brackets.top();
            brackets.pop();
            
            if ((c == ')' && left != '(') ||      // 检查类型是否匹配
                (c == ']' && left != '[') ||
                (c == '}' && left != '{')) {
                return false;
            }
        }
    }
    return brackets.empty();    // 都匹配
}

bool Utils::validateExpression(const string& expr) {    // 验证表达式合法性
    if (!checkBracketMatch(expr)) return false;    // 检查括号匹配
    
    bool lastWasOperator = true;  // 处理开头负数
    bool lastWasNumber = false;   
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        if (isSpace(c)) continue;    // 跳过空白字符
        
        if (isOperator(c)) {    
            if (lastWasOperator && c != '-') return false;  // 不允许连续运算符，但允许负号
            lastWasOperator = true;
            lastWasNumber = false;
        } else if (isNumber(c)) { 
            lastWasOperator = false;
            lastWasNumber = true;
        } else if (isBracket(c)) {    
            if (c == '(' || c == '[' || c == '{') {
                if (lastWasNumber) return false;  // 数字后面不能直接跟左括号
            }
            lastWasOperator = false;
            lastWasNumber = false;
        }
    }
    
    return !lastWasOperator;  // 表达式不能以运算符结尾
}
/*用栈保存运算符，遇到数字直接输出。
遇到运算符，弹出栈中优先级高或相等的运算符，最后将当前运算符入栈。
遇到括号，左括号入栈，遇到右括号将两括号间符号弹出栈。
最后把栈中剩余运算符输出。*/
string Utils::infixToPostfix(const string& expr) {    // 中缀转后缀表达式
    string result;
    stack<char> operators;    // 运算符栈
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        if (isSpace(c)) continue;    // 跳过空白字符
        
        if (isNumber(c)) {    // 处理数字
            while (i < expr.length() && (isNumber(expr[i]) || expr[i] == '.')) {
                result += expr[i++];    // 收集完整的数字
            }
            result += ' ';    // 数字之间用空格分隔
            i--;
        }
        else if (isOperator(c)) {    // 处理运算符
            while (!operators.empty() && operators.top() != '(' &&
                   PRECEDENCE.at(operators.top()) >= PRECEDENCE.at(c)) {
                result += operators.top();    // 将优先级高的运算符加入结果
                result += ' ';
                operators.pop();
            }
            operators.push(c);    // 当前运算符入栈
        }
        else if (c == '(') {    // 左括号直接入栈
            operators.push(c);
        }
        else if (c == ')') {    // 处理右括号
            while (!operators.empty() && operators.top() != '(') {
                result += operators.top();    // 将括号内的运算符加入结果
                result += ' ';
                operators.pop();
            }
            if (!operators.empty()) operators.pop();  // 弹出左括号
        }
    }
    
    while (!operators.empty()) {    // 处理剩余的运算符
        result += operators.top();
        result += ' ';
        operators.pop();
    }
    
    return result;
}
/*先反转表达式，左右括号互换。
按后缀转换方法处理（即调用 infixToPostfix）。
得到的后缀表达式再反转，就是前缀表达式。逆向后缀转换处理*/
string Utils::infixToPrefix(const string& expr) {    // 中缀转前缀表达式
    // 1. 反转表达式
    string reversed = expr;
    reverse(reversed.begin(), reversed.end());
    
    // 2. 交换括号
    for (char& c : reversed) {
        if (c == '(') c = ')';
        else if (c == ')') c = '(';
    }
    
    // 3. 转换为后缀表达式
    string postfix = infixToPostfix(reversed);
    
    // 4. 再次反转得到前缀表达式
    reverse(postfix.begin(), postfix.end());
    return postfix;
}
/*后缀转中缀（postfixToInfix）：遇到数字入栈，遇到运算符弹出两个操作数，拼成(a op b)再入栈。*/
string Utils::postfixToInfix(const string& expr) {    // 后缀转中缀表达式
    stack<string> operands;    // 操作数栈
    string token;             // 当前数字字符串
    
    for (char c : expr) {
        if (isSpace(c)) {    // 处理空白字符
            if (!token.empty()) {
                operands.push(token);    // 将数字压入栈
                token.clear();
            }
            continue;
        }
        
        if (isNumber(c)) {    // 处理数字
            token += c;
        }
        else if (isOperator(c)) {    // 处理运算符
            if (!token.empty()) {
                operands.push(token);
                token.clear();
            }
            
            string b = operands.top(); operands.pop();    // 取出两个操作数
            string a = operands.top(); operands.pop();
            
            operands.push("(" + a + " " + c + " " + b + ")");    // 构造中缀表达式并压入栈
        }
    }
    
    return operands.top();    // 返回最终的中缀表达式
}
/*前缀转中缀（prefixToInfix）：从右往左，遇到数字入栈，遇到运算符弹出两个操作数，拼成(a op b)再入栈。逆向后缀转中缀*/
string Utils::prefixToInfix(const string& expr) {    // 前缀转中缀表达式
    stack<string> operands;    // 操作数栈
    string token;             // 当前数字字符串
    
    for (int i = expr.length() - 1; i >= 0; i--) {    // 从右向左处理
        char c = expr[i];
        
        if (isSpace(c)) {    // 处理空白字符
            if (!token.empty()) {
                reverse(token.begin(), token.end());    // 反转数字字符串
                operands.push(token);
                token.clear();
            }
            continue;
        }
        
        if (isNumber(c)) {    // 处理数字
            token += c;
        }
        else if (isOperator(c)) {    // 处理运算符
            if (!token.empty()) {
                reverse(token.begin(), token.end());
                operands.push(token);
                token.clear();
            }
            
            string a = operands.top(); operands.pop();    // 取出两个操作数
            string b = operands.top(); operands.pop();
            
            operands.push("(" + a + " " + c + " " + b + ")");    // 构造中缀表达式并压入栈
        }
    }
    
    if (!token.empty()) {    // 处理最后一个数字
        reverse(token.begin(), token.end());
        operands.push(token);
    }
    
    return operands.top();    // 返回最终的中缀表达式
} 