#include "calculator.h"  // 包含计算器头文件
#include <iostream>  
#include <string>  
#include <iomanip>     
#include <sstream>
#include <vector>

using namespace std;  
////////////////////////////////搞清楚哪些函数被使用，哪些函数没有被使用，每个文件之间的关系////////////////////////////////////
void printWelcome() {  // 打印欢迎信息的函数
    cout << "欢迎使用！ Welcome to Expression Calculator!" << endl;  
    cout << "所支持运算符： Supported operators: +, -, *, /, %, ^, &, |" << endl;  
    cout << "所支持函数： Supported functions: sin(s), cos(c), tan(t), log(l)" << endl; 
    
    Calculator calc;  // 创建计算器对象
    auto& constants = calc.getConstants();  // 获取数学常量列表
    
    cout << "已内置常量： Supported mathematical constants: ";  // 数学常量提示
    for (const auto& [name, value] : constants) {  
        cout << name << " = " << value << ", ";    //每个常量的名称和值
    }
    cout << endl; 
    
    cout << "请选择表达式类型： Please select expression type:" << endl;  // 选择表达式类型
    cout << "1. 中缀表达式 Infix expression (e.g., 1 3 + 4 * 5)" << endl;    // 中缀
    cout << "2. 前缀表达式 Prefix expression (e.g., 2 + * 2 3 4)" << endl;   // 前缀
    cout << "3. 后缀表达式 Postfix expression (e.g., 3 3 4 * 2 +)" << endl;  // 后缀
}

int main() {  
    Calculator calc;  
    string input;    // 存储输入的字符串
    
    printWelcome();  
    
    while (true) {  
        cout << "\n请输入表达式类型和表达式 (或输入 'q' 退出) / Enter expression type and expression (or 'q' to quit): ";  // 提示用户输入
        getline(cin, input);  
        
        if (input == "q" || input == "Q") {  // 退出
            break;  
        }
        
        try {  // 异常处理开始
            // 解析输入
            size_t pos = input.find(' ');  
            if (pos == string::npos) {  //未搜索到
                cout << "\n错误 (Error): 输入格式无效，请使用'类型 表达式'格式 / Invalid input format. Please use 'type expression' format" << endl;
                continue;
            }
            
            string typeStr = input.substr(0, pos);  // 类型
            string expr = input.substr(pos + 1);    // 表达式
            
            ExpressionType type; 
            try {
                type = static_cast<ExpressionType>(stoi(typeStr));  
            } catch (...) {  // 捕获所有可能的异常
                cout << "\n错误 (Error): 表达式类型必须是数字 (1-中缀, 2-前缀, 3-后缀) / Expression type must be a number (1-Infix, 2-Prefix, 3-Postfix)" << endl;
                continue;
            }
            
            // 预处理表达式
            if (type == ExpressionType::PREFIX) {
                // 检查第一个非空字符是否是运算符
                size_t firstNonSpace = expr.find_first_not_of(" \t\n\r");
                if (firstNonSpace != string::npos && !calc.isOperator(expr[firstNonSpace])) {
                    cout << "\n错误 (Error): 前缀表达式必须以运算符开始 / Prefix expression must start with an operator" << endl;
                    continue;
                }
            }
            
            cout << "\n正在计算 (Evaluating):" << endl;
            switch (type) {
                case ExpressionType::INFIX:   
                    cout << "中缀表达式 (Infix): " << expr << endl;
                    break;
                case ExpressionType::PREFIX:  
                    cout << "前缀表达式 (Prefix): " << expr << endl;
                    break;
                case ExpressionType::POSTFIX: 
                    cout << "后缀表达式 (Postfix): " << expr << endl;
                    break;
            }
        
            double result = calc.evaluate(expr, type);  // 调用计算器求值
            
            if (!calc.hasErrorOccurred()) {  // 如果没有错误发生
                cout << "\n最终结果 (Final result): " << fixed << setprecision(10) << result << endl;  // 输出结果，保留10位小数
            }
            cout << "----------------------------------------" << endl; 
        }
        catch (const exception& e) {  // 捕获异常
            cout << "\n错误 (Error): " << e.what() << endl;  // 输出错误
            cout << "----------------------------------------" << endl; 
        }
    }
    
    cout << "感谢使用！/ Thank you for using!" << endl;  // 打印感谢信息
    return 0; 
}