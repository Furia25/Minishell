# MiniShell

A lightweight, POSIX-compliant command-line shell written in C, featuring advanced parsing, redirections, wildcard expansion, and more.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen) ![License](https://img.shields.io/badge/license-MIT-blue)

---

<p align="center">
  <img src="https://github.com/user-attachments/assets/e2170b17-095c-49af-b00c-b29d4a7a4b2f" alt="MiniShell Logo" />
</p>

## Table of Contents

* [Introduction](#introduction)
* [Advanced Features](#advanced-features)
* [Installation](#installation)
* [Configuration](#configuration)
* [Usage](#usage)
* [License](#license)

---

## Introduction

MiniShell is a custom command-line interpreter designed to mimic the behavior of popular shells (bash, zsh) while introducing robust internal mechanisms such as garbage-collected memory management, detailed debug output, and a modular architecture for easy extension.

## Advanced Features

### 1. Comprehensive Parsing Engine

* **Tokenization & AST Construction**: Supports quotes (`'`, `"`), command substitution `` `cmd` ``, and logical operators (`&&`, `||`, `|`).
* **Wildcard Expansion**: Implements `*` and `?` expansion within arguments, searching directories recursively.

### 2. Redirections & Here-Documents

* **Input/Output Redirections**: `<`, `>`, `>>`, `<>`.
* **Here-Document (`<<`)**: Inline multi-line input with proper variable expansion support.

### 3. Built-in Commands

* `cd`, `export`, `unset`, `env`, `pwd`, `echo`, `exit` with POSIX-compatible flags and error handling.

### 4. Environment Management

* **Dynamic Hashmap**: Efficient and fast name-value storage for environment variables, automatically resizing.
* **Persistent State**: Updates `SHLVL`, `PWD`, and `OLDPWD` seamlessly.

### 5. Signal Handling & Job Control

* Context-aware signal setup for prompt, heredoc, and child processes to ensure a smooth user experience.

### 6. Memory Safety & Garbage Collector

* **Custom GC**: Tracks allocations and automatically frees unused memory at end-of-execution or on errors.
* Prevents leaks and dangling pointers, even in complex pipelines.

### 7. Debugging Utilities

* Toggleable debug levels to inspect token lists, AST nodes, and command execution states for rapid development and troubleshooting.

## Installation

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
make
```

## Configuration

* **Debugging**: Set `DEBUG` in `debug.h` to desired level (0–13).

* **Shell Name**: Customize the shell’s displayed name by editing the `MINISHELL_NAME` macro in `header.h`.

* **Prompt**: Define a custom prompt format using the `PROMPT` macro in `minishell.h`.

* **Welcome Prompt**: Customize the welcome message displayed at shell startup by modifying the `MINISHELL_WELCOME_START` and `MINISHELL_WELCOME_END` macros in `minishell.h`.

* **Builtin Command Names**: Rename built-in commands by modifying the `BUILTIN_<CMD>_NAME` macros in `builtins.h`.

* **Signal Display**: Enable or disable signal reporting by setting `MINISHELL_SHOW_SIGNALS` in `minishell.h` (0 disables, 1 enables).

* **Core Dump on Signal**: Control core dump message via the `SIGNAL_CORE_DUMP_MESSAGE` macro in `minishell_signal.h`.

> **Note:** After adjusting any configuration macros in headers run `make clean && make` to recompile and apply your changes or directly run `make CONFIGURATION=-D<DEFINE>=<VALUE>`.

## Usage

Launch the shell interactively:

```bash
./minishell
```

Run a script:

```bash
./minishell script.sh
```

Execute single command:

```bash
./minishell -c "echo Hello World"
```

## License

Distributed under the MIT License. See [LICENSE](./LICENSE) for details.
