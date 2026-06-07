# 🚀 Xz Language Interpreter

It's my own AST-based (at this moment) training interpreter written in C from scratch.

## 🛠️ Features Under the Hood

Instead of using heavy standard libraries, I implement core components manually for performance and deep learning:
* **Custom Arena Allocator:** A dynamic, chunked memory allocator aligned to 8 bytes to prevent memory leaks and speed up allocations.
* **Custom Hash Table:** A chaining hash table powered by the FNV-1a algorithm for the future symbol table (scopes and variables).

## 📈 Current Status

The project is under active development. Currently, it supports lexing, parsing, and evaluating basic arithmetic expressions, taking into account operator precedence (`+`, `-`, `*`, `/` and parentheses). 

Example: `(5 + 129) * 2`

## 🗺️ Development Plan

- [ ] Add variable support (Scope & Symbol Table)
- [ ] Implement `if/else` statements and loops (`while`)
- [ ] Built-in functions (e.g., `print`)
- [ ] Compilation to bytecode and writing a custom VM

## 💻 Build & Run

To build the interpreter, you need `make` and a C compiler (`gcc` or `clang`).

```bash
make
./interpreter <input file>
