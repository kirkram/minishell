# Minishell
This a recreation of BASH using C.

![Screenshot from 2024-11-06 19-13-35 50%](https://github.com/user-attachments/assets/524fc003-b8f8-4834-a958-8a94bc5abaa1)

## How to use it
Using make will create the minishell executable. 
It requires [readline](https://tiswww.case.edu/php/chet/readline/rltop.html) library to compile.

On mac install it with `brew install readline` on Mac. It will assume `-L${HOME}/.brew/opt/readline/lib` as path to the lib. 

On linux `sudo apt install libreadline-dev`.

`Make` and run it with :
```
./minishell
```
Press `Ctrl + D` to exit.


## Scope
This minishell handles basic functionality of BASH: pipes, working directories, environment variables, POSIX signals, redirections and here_doc (<, <<, >, >>). Readline library is included for portability. The original project uses a installed version on MAC/LINUX.

## Credits
This was a two person project done in collaboration with Casimir Lundberg ([Welhox](https://github.com/Welhox)).
I was mainly in charge of executing part of the program, while Casimir was responsible for the parsing.
