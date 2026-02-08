#!/bin/bash
# test_underscore.sh
# 测试 minishell 中 "_" 的行为

echo "==== Test 1: Simple command ===="
/bin/echo hello
echo "_ = $_"    # 预期: /bin/echo

echo
echo "==== Test 2: Pipeline left ===="
/bin/env | grep HOME
echo "_ = $_"    # 预期: /bin/env

echo
echo "==== Test 3: Pipeline right ===="
/bin/echo hello | /usr/bin/grep hello
echo "_ = $_"    # 预期: /bin/echo

echo
echo "==== Test 4: Subshell ===="
(echo in_subshell)
echo "_ = $_"    # 预期: 上一条命令的 argv[0]

echo
echo "==== Test 5: Stateful built-in ===="
export FOO=42
echo "_ = $_"    # 预期: export

echo
echo "==== Test 6: Consecutive commands ===="
/bin/echo one
/bin/echo two
echo "_ = $_"    # 预期: /bin/echo (最后一条命令)

echo
echo "==== Test 7: Logic AND/OR ===="
/bin/true && /bin/echo ok
echo "_ = $_"    # 预期: /bin/echo

/bin/false || /bin/echo fail
echo "_ = $_"    # 预期: /bin/echo
