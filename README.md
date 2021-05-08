# README

MiniBasic is a simplified Basic interpreter built by QT

Please install the font before use.

#### Example:

![](https://raw.githubusercontent.com/Naplesoul/MiniBasic/main/example.png)



#### Debug Example:

![](https://raw.githubusercontent.com/Naplesoul/MiniBasic/main/debugExample.png)



#### Error Highlights:

![](https://raw.githubusercontent.com/Naplesoul/MiniBasic/main/errorExample.png)



#### Usage:

RUN --Execute the program

SAVE --Save code to a file

LOAD --Load code from a file

LIST --List code in the Code box

CLEAR --Clear code and context

DEL --Delete a line

HELP --Get help text

QUIT --Quit the interpreter



#### Supported Statements:

REM: Comments, e.g. 1 REM HELLO!

LET: Assignment, e.g. 1 LET N = 3  or  1 LET N = "HELLO"

PRINT: Print, e.g. 1 PRINT 5  or  1 PRINT N

PRINTF: Formatted print, e.g. 1 PRINTF "HELLO, {}! {} {}", "WORLD", 666, N

INPUT: Input an INT, e.g. 1 INPUT N

INPUTS: Input a STRING, e.g. 1 INPUT N

GOTO: Jump to a line, e.g. 1 GOTO 9

IF: if, <= and >= NOT supported, e.g. 1 IF N > 9 THEN 9

END: End of the program, can be omitted, e.g. 10 END



#### Shortcuts:

Ctrl + L --LOAD

Ctrl + S --SAVE

Ctrl + R --RUN

Ctrl + Del --Clear input box



To insert a line, type line number with a space before the command:

1 PRINT 666

Line number should be between 1 & 1000000

Only typing a line number will delete this line



INPUT, INPUTS, PRINT, PRINTF and LET command can be executed immediately on the current context