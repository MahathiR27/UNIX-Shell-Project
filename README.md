# UNIX Shell using C
**Details:** The shell must be able to run basic linux commands, I/O redirections and handle errors. Following core features must be implemented:
1. Display a command prompt (e.g., `sh>` ) and read user input.
2. Parse and execute system commands.  
_For example, running `pwd` will output the absolute path of the directory that your shell is working on.  
Hint: Use `fork` and `exec` system call._
3. Support input (`<`) and output (`>` and `>>`) redirection.  
_Hint: Use `dup` and `dup2`._
4. Support command piping (`|`). Your shell should support any number of piping.  
_For example, `command1 | command2 | command3 | command4` should work._
5. Support multiple commands in-line separated by semicolon (`;`).
6.  Support multiple command in sequence using (`&&`).
7.  Support history of executed commands.
8.   Support signal handling. Pressing `CTRL+C` should terminate the currently running command inside your shell, not your shell.  
_Hint: Use `signal` and `sigaction`._

**Marks Distribution:** There will be a project demonstration and viva after submission, which actually carries the marks. If you complete the project but fail in viva you will not get the marks. 
|**Features**|  **Marks**|
|--|--|
|Basic shell functionality & built-in command support|40|
|Redirection (`<`, `>`, `>>`)|15|
|Piping|15|
|Signal handling|15|
|Logical opt. in terminal (`;`, `&`)|10|
|History|5|
|**Total**|**100 (Converts to 5)**|
