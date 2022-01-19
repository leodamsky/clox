# Lox (Bytecode Interpreter)

Implementation of the [Lox](https://craftinginterpreters.com/the-lox-language.html) programming language in C. This
interpreter reads the script, compiles it to a sequence of bytecode instructions, and hands off instructions to the
virtual machine, which executes them one by one.

`main` branch doesn't include additional features from book's challenges. Checkout a specific branch to test some extra
features.

## How to Build

```shell
mkdir build
cd build
# set up CMake project
cmake ..
cd -
# compile executable to the build directory
cmake --build build --target clox
```

## How To Run

Interpreter can interpret file contents as well as run as a REPL.

### File Interpretation

To run interpret against a file with a source code, run: `./build/clox <file-path>`. There are some examples in
the `examples/` folder. So you can try:

```shell
# tests 'super' keyword
./build/clox examples/inheritance.lox
```

All examples from https://github.com/leodamsky/lox-rs should also work.

### REPL

To run it as a REPL, run: `./build/clox`. Then try typing in some commands e.g.:

```
> // some comment
> var variableName = "hello";
> print variableName;
```

## Grammar

### Statements

```
program -> declaration* EOF ;

declaration -> classDecl
             | funDecl
             | varDecl
             | statement ;

classDecl -> "class" IDENTIFIER ( "<" IDENTIFIER )? "{" function* "}" ;

funDecl -> "fun" function ;

varDecl -> "var" IDENTIFIER ( "=" expression )? ";" ;

statement -> exprStmt
           | forStmt
           | ifStmt
           | printStmt
           | returnStmt
           | whileStmt
           | block ;

exprStmt -> expression ";" ;

forStmt -> "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" statement ;

ifStmt -> "if" "(" expression ")" statement
          ( "else" statement )? ;

printStmt -> "print" expression ";" ;

returnStmt -> "return" expression? ";" ;

whileStmt -> "while" "(" expression ")" statement ;

block -> "{" declaration* "}" ;

function -> INDENTIFIER "(" parameters? ")" block ;

parameters -> IDENTIFIER ( "," IDENTIFIER )* ;
```

### Expressions

```
expression -> assignement ;

assignment -> ( call "." )? IDENTIFIER "=" assignment
            | logic_or ;

logic_or -> logic_and ( "or" logic_and )* ;

logic_and -> equality ( "and" equality )* ;

equality -> comparison ( ( "!=" | "==" ) comparison )* ;

comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;

term -> factor ( ( "-" | "+" ) factor )* ;

factor -> unary ( ( "/" | "*" ) unary )* ;

unary -> ( "!" | "-" ) unary
       | call ;

call -> primary ( "(" arguments? ")" | "." IDENTIFIER )* ;

arguments -> expression ( "," expression )* ;

primary -> NUMBER
         | STRING
         | "true"
         | "false"
         | "nil"
         | "this"
         | "(" expression ")"
         | IDENTIFIER
         | "super" "." IDENTIFIER;
```
