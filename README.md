# 表达式计算器 (Expression Calculator)

这是一个功能强大的表达式计算器，支持中缀、前缀和后缀三种表达式格式，并能够实时显示计算过程。项目采用面向对象设计，具有高度的可扩展性和错误处理能力。
This is a powerful expression calculator that supports infix, prefix, and postfix expressions, with real-time calculation process display. The project uses object-oriented design with high extensibility and error handling capabilities.

## 功能特点 (Features)

### 1. 支持三种表达式格式 (Supported Expression Formats)
- **中缀表达式**（如：2 + 3 * 4, sin(pi/2), -5.2 + 3.14）/ Infix expression (e.g., 2 + 3 * 4, sin(pi/2), -5.2 + 3.14)
- **前缀表达式**（如：+ * 3 4 2, s / pi 2, + -5.2 3.14）/ Prefix expression (e.g., + * 3 4 2, s / pi 2, + -5.2 3.14)
- **后缀表达式**（如：3 4 * 2 +, pi 2 / s, -5.2 3.14 +）/ Postfix expression (e.g., 3 4 * 2 +, pi 2 / s, -5.2 3.14 +)

### 2. 支持的运算符 (Supported Operators)
- **基本运算**：+, -, *, /, %, ^（幂运算）/ Basic operations: +, -, *, /, %, ^ (power)
- **位运算**：&（与）, |（或）/ Bitwise operations: & (AND), | (OR)
- **三角函数**：s(sin), c(cos), t(tan) / Trigonometric functions: s(sin), c(cos), t(tan)
- **对数函数**：l(log) / Logarithmic function: l(log)

### 3. 支持的数字格式 (Supported Number Formats)
- **整数** / Integers
- **小数**（如：3.14）/ Decimals (e.g., 3.14)
- **负数**（如：-5.2, -3）/ Negative numbers (e.g., -5.2, -3)
- **科学计数法**（如：2e3, 1.5e-2）/ Scientific notation (e.g., 2e3, 1.5e-2)

### 4. 内置数学常量 (Built-in Mathematical Constants)
- **pi (π)**: 3.14159265358979323846
- **e**: 2.71828182845904523536

### 5. 特殊功能 (Special Features)
- **实时显示计算过程** / Real-time calculation process display
- **显示运算符栈和运算数栈的状态** / Display operator and operand stack states
- **智能错误检测和提示** / Intelligent error detection and prompts
- **支持三种类型的括号**：(), [], {} / Support for three types of brackets: (), [], {}
- **自动表达式类型检测** / Automatic expression type detection
- **表达式格式转换** / Expression format conversion

## 项目结构 (Project Structure)

```
q/
├── main.cpp                    # 主程序入口，用户交互界面
├── calculator.h                # 计算器主类声明，统一接口
├── calculator.cpp              # 计算器主类实现，协调各求值器
├── ExpressionEvaluator.h       # 表达式求值器基类和接口定义
├── infix_evaluator.h           # 中缀表达式求值器声明
├── infix_evaluator.cpp         # 中缀表达式求值器实现
├── prefix_evaluator.h          # 前缀表达式求值器声明
├── prefix_evaluator.cpp        # 前缀表达式求值器实现
├── postfix_evaluator.h         # 后缀表达式求值器声明
├── postfix_evaluator.cpp       # 后缀表达式求值器实现
├── utils.h                     # 工具函数声明，共享功能
├── utils.cpp                   # 工具函数实现，表达式转换等
├── test.txt                    # 测试用例文件
├── README.md                   # 项目说明文档
└── .vscode/                    # VS Code 配置文件
```

### 文件详细说明 (Detailed File Descriptions)

#### 核心文件 (Core Files)
- **`main.cpp`**: 程序入口点，处理用户输入，提供交互式界面，支持三种表达式类型的输入和错误处理
- **`calculator.h/cpp`**: 计算器主类，作为统一接口协调不同的表达式求值器，管理错误状态和常量
- **`ExpressionEvaluator.h`**: 定义表达式求值器的基类和接口，包含错误类型枚举和表达式类型检测器（可实现面向对象设计）

#### 求值器文件 (Evaluator Files)
- **`infix_evaluator.h/cpp`**: 中缀表达式求值器，支持函数调用、常量、负数、小数、科学计数法，使用双栈算法
- **`prefix_evaluator.h/cpp`**: 前缀表达式求值器，从右向左扫描，支持所有数字格式和运算符
- **`postfix_evaluator.h/cpp`**: 后缀表达式求值器，从左向右扫描，支持负数、小数和科学计数法

#### 工具文件 (Utility Files)
- **`utils.h/cpp`**: 工具函数库，提供表达式验证、格式转换、括号匹配检查等共享功能

## 使用方法说明 (Usage)

### 1. 运行程序 (Run the program)

### 2. 输入格式 (Input format)
```
<类型> <表达式> / <type> <expression>
```

**类型 (Type)**：
- `1`：中缀表达式 / Infix expression
- `2`：前缀表达式 / Prefix expression  
- `3`：后缀表达式 / Postfix expression

### 3. 输入示例 (Input examples)

#### 中缀表达式 (Infix expressions)
```
1 2 + 3 * 4
1 sin(pi/2)
1 2^3 + 4.5
1 -5.2 + 3.14
1 2e3 * 1.5e-2
1 10 % 3
1 5 & 3
1 cos(0) + log(e)
```

#### 前缀表达式 (Prefix expressions)
```
2 + * 3 4 2
2 s / pi 2
2 + ^ 2 3 4.5
2 + -5.2 3.14
2 * 2e3 1.5e-2
2 % 10 3
2 & 5 3
2 + c 0 l e
```

#### 后缀表达式 (Postfix expressions)
```
3 3 4 * 2 +
3 pi 2 / s
3 2 3 ^ 4.5 +
3 -5.2 3.14 +
3 2e3 1.5e-2 *
3 10 3 %
3 5 3 &
3 0 c e l +
```

## 计算过程显示 (Calculation Process Display)

程序会实时显示以下信息 (The program displays in real-time)：

1. **剩余表达式** / Remaining expression
2. **数字栈状态** / Number stack state
3. **运算符栈状态**（对于中缀表达式）/ Operator stack state (for infix expressions)
4. **每步操作的详细说明** / Detailed step-by-step operation description

### 示例输出 (Sample output)
```
请输入表达式 (Enter expression)：1 2 + 3 * 4

执行操作 (Operation): 压入数字 (Push number): 2
剩余表达式 (Remaining expression): + 3 * 4
数字栈 (Number stack): |2.00|
运算符栈 (Operator stack): |
----------------------------------------

执行操作 (Operation): 压入运算符 (Push operator): +
剩余表达式 (Remaining expression): 3 * 4
数字栈 (Number stack): |2.00|
运算符栈 (Operator stack): |+|
----------------------------------------

...（更多步骤）(more steps)...

最终结果 (Final result): 14
----------------------------------------
```

## 错误处理 (Error Handling)

程序会智能检测并提示以下错误 (The program intelligently detects and prompts for the following errors)：

1. **括号不匹配** / Mismatched parentheses
2. **非法字符** / Invalid characters
3. **连续运算符** / Consecutive operators
4. **除以零** / Division by zero
5. **表达式格式错误** / Invalid expression format
6. **负数格式错误** / Invalid negative number format
7. **函数参数错误** / Function argument errors
8. **操作数不足** / Insufficient operands

## 创新设计点 (Innovation Design Points)

### 1. 可转换成面向对象架构 (Object-Oriented Architecture)
- **基类设计**：`ExpressionEvaluator` 作为基类，定义统一接口
- **多态性**：三种求值器继承自基类，实现不同的求值策略
- **封装性**：每个类负责特定的功能，降低耦合度

### 2. 智能错误处理 (Intelligent Error Handling)
- **分层错误检测**：语法级、语义级、运行时错误分别处理
- **详细错误信息**：提供中英文双语错误提示
- **错误恢复**：程序不会因单个错误而崩溃

### 3. 实时可视化 (Real-time Visualization)
- **栈状态显示**：实时显示数字栈和运算符栈的内容
- **计算过程追踪**：每步操作都有详细说明
- **表达式剩余部分**：显示待处理的部分

### 4. 数字格式支持 (Number Format Support)
- **负数处理**：智能区分一元减号（负数）和二元减号（减法）
- **科学计数法**：支持 e/E 格式的科学计数法
- **小数精度**：支持高精度小数计算

### 5. 函数和常量系统 (Function and Constant System)
- **内置函数**：三角函数和对数函数
- **数学常量**：pi 和 e 常量
- **函数调用语法**：支持嵌套函数调用

### 6. 表达式转换功能 (Expression Conversion)
- **中缀转后缀**：使用栈算法实现转换
- **中缀转前缀**：通过反转和转换实现
- **后缀转中缀**：重建带括号的中缀表达式

## 技术实现细节 (Technical Implementation Details)

### 1. 中缀表达式求值算法 (Infix Evaluation Algorithm)
- **双栈算法**：使用数字栈和运算符栈
- **优先级处理**：根据运算符优先级决定计算顺序
- **括号处理**：支持多种括号类型
- **函数处理**：支持一元函数调用

### 2. 前缀表达式求值算法 (Prefix Evaluation Algorithm)
- **从右向左扫描**：逆序处理表达式
- **栈操作**：遇到运算符时弹出操作数进行计算
- **Token分割**：按空格分割表达式

### 3. 后缀表达式求值算法 (Postfix Evaluation Algorithm)
- **从左向右扫描**：顺序处理表达式
- **栈操作**：遇到运算符时弹出操作数进行计算
- **数字解析**：支持各种数字格式

### 4. 表达式验证算法 (Expression Validation)
- **语法检查**：检查括号匹配、运算符连续性
- **语义检查**：检查操作数数量、函数参数
- **格式检查**：检查数字格式、表达式结构

## 扩展性 (Extensibility)

程序设计时考虑了高度的扩展性，可以方便地添加新功能 (The program is designed for high extensibility)：

### 1. 新运算符 (New Operators)
- 在 `evaluateOperation` 方法中添加新的 case
- 在 `precedence` 映射中设置优先级
- 在 `isOperator` 方法中添加判断

### 2. 新数学函数 (New Mathematical Functions)
- 在 `evaluateOperation` 方法中添加函数实现
- 在 `isFunction` 方法中添加函数标识
- 支持多参数函数

### 3. 新数学常量 (New Mathematical Constants)
- 在 `constants` 映射中添加新常量
- 在解析逻辑中添加常量识别

### 4. 新表达式格式 (New Expression Formats)
- 继承 `ExpressionEvaluator` 基类
- 实现 `evaluate` 和 `validateExpression` 方法
- 在 `Calculator` 类中添加新类型支持

### 5. 自定义输出格式 (Custom Output Format)
- 修改 `displayStack` 和 `displayStep` 方法
- 支持不同的显示风格和语言

### 5. 可通过输出信息的“-------”来划分界面，通过qt实现模拟单步调试动态计算过程
- 完善qt文件夹里的函数，实现相关功能
  
## 注意事项 (Notes)

1. **输入格式**：输入表达式时不需要在末尾添加等号 / No need to add equals sign at the end
2. **三角函数**：三角函数使用弧度制 / Trigonometric functions use radians
3. **空格处理**：程序对空格不敏感，但建议使用空格分隔 / The program is space-insensitive but spaces are recommended
4. **退出程序**：输入 'q' 或 'Q' 可以退出程序 / Enter 'q' or 'Q' to quit
5. **括号使用**：括号可以混合使用：(), [], {} / Brackets can be mixed: (), [], {}
6. **负数输入**：负数前必须有空格或括号 / Negative numbers must be preceded by space or bracket
7. **函数调用**：函数使用单字母标识：s(sin), c(cos), t(tan), l(log) / Functions use single letter: s(sin), c(cos), t(tan), l(log)

## 性能特点 (Performance Characteristics)

- **时间复杂度**：O(n)，其中 n 是表达式长度 / O(n) where n is expression length
- **空间复杂度**：O(n)，主要用于栈存储 / O(n) mainly for stack storage
- **内存使用**：高效的内存管理，避免内存泄漏 / Efficient memory management, no memory leaks
- **计算精度**：使用 double 类型，支持高精度计算 / Uses double type for high precision
