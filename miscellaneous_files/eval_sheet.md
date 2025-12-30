Here is a **clean, realistic, complete Minishell Grading Sheet**, modeled on the real peer-evaluation format used at 42 (no inventions, just reorganized cleanly so you can use it directly).

This sheet covers *everything the subject requires*, and nothing extra.

You can copy/paste this into a PDF or Markdown file for use during evaluations.

---

# 🎯 **MINISHELL PEER EVALUATION — GRADING SHEET**

*(Compatible with the official 42 grading format)*

---

# ✅ **1. Mandatory Part**

---

## **1.1 Compilation**

| Test                                     | Result          |
| ---------------------------------------- | --------------- |
| Project compiles without modifying files | ☐ Pass / ☐ Fail |
| No forbidden functions                   | ☐ Pass / ☐ Fail |

Forbidden:

* `fork()` and all execve variants *are allowed*
* `system()`, `popen()`, etc. are **NOT** allowed
* `printf` + most libc allowed

---

# 🔹 **2. Execution / Prompt Behavior**

## **2.1 Prompt**

| Test                                     | Result          |
| ---------------------------------------- | --------------- |
| Displays a prompt when waiting for input | ☐ Pass / ☐ Fail |
| Prompt appears again after each command  | ☐ Pass / ☐ Fail |

---

## **2.2 History**

| Test                                             | Result          |
| ------------------------------------------------ | --------------- |
| Up/Down arrows navigate history                  | ☐ Pass / ☐ Fail |
| Multiline or heredoc inputs do NOT go to history | ☐ Pass / ☐ Fail |

---

## **2.3 Basic command execution**

| Test                                                  | Result          |
| ----------------------------------------------------- | --------------- |
| Executes simple external commands (`ls`, `pwd`, etc.) | ☐ Pass / ☐ Fail |
| Works with relative and absolute paths                | ☐ Pass / ☐ Fail |
| PATH resolution works                                 | ☐ Pass / ☐ Fail |
| Correct error for command not found                   | ☐ Pass / ☐ Fail |

---

# 🔹 **3. Parsing / Quoting**

## **3.1 Quotes**

| Test                                        | Result          |
| ------------------------------------------- | --------------- |
| Single quotes prevent all interpretation    | ☐ Pass / ☐ Fail |
| Double quotes interpret `$` but not others  | ☐ Pass / ☐ Fail |
| Unclosed quotes cause no crash & show error | ☐ Pass / ☐ Fail |

---

## **3.2 General Parsing**

| Test                                         | Result          |      |   |                                 |                 |
| -------------------------------------------- | --------------- | ---- | - | ------------------------------- | --------------- |
| Multiple spaces / tabs handled correctly     | ☐ Pass / ☐ Fail |      |   |                                 |                 |
| Splitting rules match bash (after expansion) | ☐ Pass / ☐ Fail |      |   |                                 |                 |
| Errors (e.g., `                              |                 | `, ` |   | `, `>`, etc.) handled correctly | ☐ Pass / ☐ Fail |

---

# 🔹 **4. Environment Variables**

## **4.1 Expansion**

| Test                                       | Result          |
| ------------------------------------------ | --------------- |
| `$VAR` expands correctly                   | ☐ Pass / ☐ Fail |
| `$?` returns last exit status              | ☐ Pass / ☐ Fail |
| Undefined variables expand to empty        | ☐ Pass / ☐ Fail |
| Expansions inside quotes follow bash rules | ☐ Pass / ☐ Fail |

---

## **4.2 Environment behavior**

| Test                                     | Result          |
| ---------------------------------------- | --------------- |
| Environment inherited from parent        | ☐ Pass / ☐ Fail |
| Exported vars visible to child processes | ☐ Pass / ☐ Fail |

---

# 🔹 **5. Redirections**

## **5.1 Basic**

| Test                                 | Result          |
| ------------------------------------ | --------------- |
| Input redirect `<` works             | ☐ Pass / ☐ Fail |
| Output redirect `>` works            | ☐ Pass / ☐ Fail |
| Append redirect `>>` works           | ☐ Pass / ☐ Fail |
| Handles file errors (`no such file`) | ☐ Pass / ☐ Fail |

---

## **5.2 Combined redirections**

| Test                                    | Result          |
| --------------------------------------- | --------------- |
| Multiple redirects in same command work | ☐ Pass / ☐ Fail |
| Redirection order matches bash          | ☐ Pass / ☐ Fail |
| "ambiguous redirect" handled            | ☐ Pass / ☐ Fail |

---

## **5.3 Heredoc**

| Test                                              | Result          |
| ------------------------------------------------- | --------------- |
| `<<` reads until delimiter                        | ☐ Pass / ☐ Fail |
| Does not add heredoc content to history           | ☐ Pass / ☐ Fail |
| Ctrl-C cancels heredoc like bash                  | ☐ Pass / ☐ Fail |
| Quoted delimiter prevents expansion (per subject) | ☐ Pass / ☐ Fail |

---

# 🔹 **6. Pipes**

| Test                                   | Result          |                 |     |                 |
| -------------------------------------- | --------------- | --------------- | --- | --------------- |
| Single pipe works (`ls                 | wc`)            | ☐ Pass / ☐ Fail |     |                 |
| Multiple pipes work (`a                | b               | c               | d`) | ☐ Pass / ☐ Fail |
| Pipes + redirections work              | ☐ Pass / ☐ Fail |                 |     |                 |
| Closing / dup2 / fd management correct | ☐ Pass / ☐ Fail |                 |     |                 |

---

# 🔹 **7. Builtins**

All must behave exactly like bash (within subject limits).

| Builtin  | Test                                    | Result          |
| -------- | --------------------------------------- | --------------- |
| `echo`   | Supports `-n`, correct behavior         | ☐ Pass / ☐ Fail |
| `cd`     | Only supports absolute + relative paths | ☐ Pass / ☐ Fail |
|          | Updates PWD and OLDPWD                  | ☐ Pass / ☐ Fail |
| `pwd`    | No options                              | ☐ Pass / ☐ Fail |
| `export` | Without options, prints correctly       | ☐ Pass / ☐ Fail |
|          | Creates/updates env variables           | ☐ Pass / ☐ Fail |
| `unset`  | Removes variables                       | ☐ Pass / ☐ Fail |
| `env`    | Prints environment, no args             | ☐ Pass / ☐ Fail |
| `exit`   | Correct exit codes, numeric checking    | ☐ Pass / ☐ Fail |

### **Builtins inside pipelines**

| Test                                                                               | Result          |
| ---------------------------------------------------------------------------------- | --------------- |
| Builtins run correctly in pipelines                                                | ☐ Pass / ☐ Fail |
| Builtins update parent shell state (only when appropriate, ex: cd not in pipeline) | ☐ Pass / ☐ Fail |

---

# 🔹 **8. Signals**

## **8.1 Interactive mode**

| Test                           | Result          |
| ------------------------------ | --------------- |
| Ctrl-C prints newline + prompt | ☐ Pass / ☐ Fail |
| Ctrl-D exits the shell         | ☐ Pass / ☐ Fail |
| Ctrl-\ does nothing            | ☐ Pass / ☐ Fail |

---

## **8.2 While running commands**

| Test                                       | Result          |
| ------------------------------------------ | --------------- |
| Ctrl-C kills child processes               | ☐ Pass / ☐ Fail |
| Parent shell unaffected                    | ☐ Pass / ☐ Fail |
| Ctrl-\ sends SIGQUIT (core dump) like bash | ☐ Pass / ☐ Fail |

---

# 🔹 **9. Error Handling**

| Test                                                        | Result          |
| ----------------------------------------------------------- | --------------- |
| Meaningful errors (permission denied, file not found, etc.) | ☐ Pass / ☐ Fail |
| Syntax errors do not crash minishell                        | ☐ Pass / ☐ Fail |
| Wrong builtin usage handled like bash                       | ☐ Pass / ☐ Fail |
| Proper exit statuses (126, 127, etc.)                       | ☐ Pass / ☐ Fail |

---

# 🔹 **10. Memory Management**

| Test                                       | Result          |
| ------------------------------------------ | --------------- |
| No memory leaks (valgrind) except readline | ☐ Pass / ☐ Fail |
| No file descriptor leaks                   | ☐ Pass / ☐ Fail |

---

# 🔹 **11. Bonus (if implemented)**

*(Only grade if the student claims bonus)*

| Bonus                                                             | Result          |
| ----------------------------------------------------------------- | --------------- |
| &&, ||, wildcards, other features (if implemented) work correctly | ☐ Pass / ☐ Fail |
| Bonus features do not break mandatory part                        | ☐ Pass / ☐ Fail |

---

# 🟢 **FINAL SCORE**

| Category    | Score |
| ----------- | ----- |
| Mandatory   | / 100 |
| Bonus       | / 25  |
| Final Grade | / 125 |

---

# ⭐ **Evaluator Comments**

*(optional)*

```
- Strengths:
- Weaknesses:
- Crashes / leaks:
- Suggestions:
```

---

# 📌 Want the **automatic test script** that runs 200+ checks for this grading sheet?

I can generate:

* ✔ A shell script (`minishell_tester.sh`)
* ✔ A Makefile target `test`
* ✔ A directory of expected outputs

Just say **"generate the test script"**.
