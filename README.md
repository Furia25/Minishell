<p align="center">
  <img src="./assets/logo.png" alt="MiniShell Logo" width="200" />
</p>

# MiniShell

A lightweight, POSIX-compliant command-line shell written in C, featuring advanced parsing, redirections, wildcard expansion, and more.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen) ![License](https://img.shields.io/badge/license-MIT-blue)

---

## Table of Contents

* [Introduction](#introduction)
* [Advanced Features](#advanced-features)
* [Installation](#installation)
* [Usage](#usage)
* [Configuration](#configuration)
* [Contributing](#contributing)
* [License](#license)

---

## Introduction

MiniShell is a custom command-line interpreter designed to mimic the behavior of popular shells (bash, zsh) while introducing robust internal mechanisms such as garbage-collected memory management, detailed debug output, and a modular architecture for easy extension.

## Advanced Features

### 1. Comprehensive Parsing Engine

* **Tokenization & AST Construction**: Supports quotes (`'`, `"`), command substitution `` `cmd` ``, and logical operators (`&&`, `||`, `|`).
* **Wildcard Expansion**: Implements `*` expansion within arguments, searching directories recursively.

### 2. Redirections & Here-Documents

* **Input/Output Redirections**: `<`, `>`, `>>`, and combined descriptors (`&>`).
* **Here-Document (`<<`)**: Inline multi-line input with proper variable expansion support.

### 3. Built-in Commands

* `cd`, `export`, `unset`, `env`, `pwd`, `echo`, `exit` with POSIX-compatible flags and error handling.

### 4. Environment Management

* **Dynamic Hashmap**: Efficient name-value storage for environment variables, automatically resizing.
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

## Configuration

* **Debugging**: Set `DEBUG` in `debug.h` to desired level (0–13).

  ```bash
  make CFLAGS="-DDEBUG=<level>"
  ```

* **Buffer Size**: Adjust `GNL_BUFFER_SIZE` in `get_next_line.h` as needed.

  ```bash
  make CFLAGS="-DGNL_BUFFER_SIZE=<size>"
  ```

* **Shell Name**: Customize the shell’s displayed name by editing the `MINISHELL_NAME` macro in `header.h`.
  Or override at compile time:

  ```bash
  make CFLAGS="-DMINISHELL_NAME=\"YourShell\""
  ```

* **Builtin Command Names**: Rename built-in commands by modifying the `BUILTIN_<CMD>_NAME` macros in `builtins.h`.

  ```bash
  make CFLAGS="-DBUILTIN_CD_NAME=\"chdir\" -DBUILTIN_EXIT_NAME=\"quit\""
  ```

* **Prompt Behavior**: Define a custom prompt format using the `PROMPT_FORMAT` macro in `header.h`.

  ```bash
  make CFLAGS="-DPROMPT_FORMAT=\"[MiniShell] $ \""
  ```

* **Signal Display**: Enable or disable signal reporting by setting `MINISHELL_SHOW_SIGNALS` in `header.h` (0 disables, 1 enables).

  ```bash
  make CFLAGS="-DMINISHELL_SHOW_SIGNALS=1"
  ```

* **Core Dump on Signal**: Control core dump behavior via the `SIGNAL_CORE_DUMP` macro in `header.h` (0 disables, 1 enables).

  ```bash
  make CFLAGS="-DSIGNAL_CORE_DUMP=1"
  ```

> **Note:** After adjusting any configuration macros via `CFLAGS`, run `make clean && make` to recompile and apply your changes.

## License

Distributed under the MIT License. See [LICENSE](./LICENSE) for details.