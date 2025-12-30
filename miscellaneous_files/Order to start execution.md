Implement single command execution with fork + execve

Test builtins that can run in child (echo, pwd)

Add redirections for single commands (< > >> and << heredoc)

Add pipelines (multiple commands)

Add parent-handled builtins (cd, export, unset)

Add signal handling and proper exit status $?

single commmand test:

ls
ls -l -a
ls > out.txt
cat < file
echo hey >> file
env
export x=1
