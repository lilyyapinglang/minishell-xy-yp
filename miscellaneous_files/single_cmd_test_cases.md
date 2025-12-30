✅ 1. Basic command execution tests

1.1 simple command

./execution ls
./execution pwd
./execution echo hello
./execution env

1.2 Multiple arguments

./execution echo hello world this is minishell
./execution ls -l -a
./execution grep foo testfile.txt

1.3 Commands not found

./execution nonexistent
./execution ls123
Expected: return 127, and message like bash:
nonexistent: command not found

✅ 2. PATH handling tests

2.1 Command found through PATH
./execution cat /etc/hosts
./execution whoami

2.2 PATH empty ?
PATH=""
./execution ls
Expected: cannot find ls.

2.3 Relative path to executable
echo '#!/bin/sh\necho hi' > ./mycmd
chmod +x mycmd
./execution ./mycmd
2.4 Absolute path
./execution /bin/echo hello
./execution /usr/bin/env

✅ 3. Quoting tests
3.1 Single quotes: nothing is expanded
./execution echo 'hello world'
./execution echo '$HOME'

3.2 Double quotes: expand $ but not others
./execution echo "$HOME"

✅ 4. Environment variable expansion
4.1 Basic
./execution echo $HOME
./execution echo $USER

4.2 Undefined variable
./execution echo $NOTHING
4.3 Mid-string expansion
./execution echo $HOME $USER $PATH
4.4 Multiple expansions
./execution echo $HOME $USER $PATH

✅ 5. $? exit status tests
5.1 Successful command
./execution echo hello
./execution echo $? → should be 0
5.2 Failed command
./execution nonexistent
./execution echo $? → 127
5.3 Command with nonzero exit
./execution sh -c "exit 42"
./execution echo $? → 42

✅ 6. Builtin: echo -n
6.1 Basic -n
./execution echo -n hello

Expected: no trailing newline.

6.2 Multiple -n

Bash accepts this:

./execution echo -nnnn hello

6.3 Mixed options
./execution echo -n -n hello

6.4 Invalid option

Bash stops parsing after invalid option:

./execution echo -nX hello❌

Expected output:

-nX hello

✅ 7. Argument parsing edge cases
7.1 Tabs, multiple spaces
./execution echo hello world

7.2 Empty argument
./execution echo ""
./execution echo ''

7.3 Leading/trailing spaces inside quotes
./execution echo " hello "

7.4 Combination
./execution echo ""$HOME""

✅ 8. Errors unrelated to PATH
8.1 Permission denied
touch file
chmod -x file
./execution ./file
Expected: exit code 126 (bash behavior) ❌

8.2 Directory instead of executable ❌
./execution /bin
Expected: also 126 ("is a directory")
