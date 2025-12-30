example 1 :
input

```
cat a | grep hi | wc -l
```

Command Table

```

nb_cmds = 3

cmd[0]:
  argv = ["cat", "a", NULL]
  redirs = []

cmd[1]:
  argv = ["grep", "hi", NULL]
  redirs = []


cmd[2]:
  argv = ["wc", "-l", NULL]
  redirs = []

```

example 2

input :

```
cat file.txt | grep foo > result


```

command table :

```
nb_cmds = 2

cmd[0]:
  argv = ["cat", "file.txt", NULL]
  redirs = []
  pipe_in = 0
  pipe_out = 1

cmd[1]:
  argv = ["grep", "foo", NULL]
  redirs = [
      { type: REDIR_OUT, filename: "result" }
  ]
  pipe_in = 1
  pipe_out = 0


```
