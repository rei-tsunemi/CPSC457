# CPSC 457 Tutorial 2
    - Tues/Thurs, 11am-12:50pm

# Introduction
- My name is Haiyang
- My email is `haiyang.he@ucalgary.ca`
# Agenda
- expectations for tut and assignments
- plagiarism
- ssh
- basic linux commands
- edit and compile C++ 
- review of C++ (next tutorial)

# Expectation on tutorial
- will  be doing live coding (wooohoo)
    - you should take notes as I talk ... 
- if you missed the tutorial, too bad ...
    - you can make some friends in the class to ask them about tutorial notes
    - I will be posting the stuff we did in tutorial on D2L
- if you want to ask questions about assignments in tutorial, please come prepared
    - can answer at the beginning and the end of tutorial
    - maybe somewhere in between
- Please upload assignment as PDF ... Clearly I'm not windows user and I have difficulties opening `.doc` files

# Assignments
- written parts: clear and concise 
    - don't bloat your response. Answer what is required so its easier to mark
- code:
    - ***MUST RUN ON U OF C LINUX COMPUTERS***
    - provide comments and documentations on where you got the code from
        - if its from Dr. Federl, its fine, just cite him
        - comment your code on what you are doing ...
    - if the question says you cannot use external resources, then DON'T ... Citing external resources when you shouldn't be using it does not count

- You should work on the assignment ***IMMEDIATELY*** (at least read it) once its out
    - the first one will be easier than the others, but the remaining ones aren't

# Plagiarism
- all assignments must be individual work, no plagiarism!
- don't go and look for code online and BLINDLY COPY PASTE ... 
    - Dr. Federl will run your code against plagiarism checker with previous years assignments ...
- don't discuss implementation, only high level idea ...
- cannot record or write anything when discussing, and wait for 20min after discussion to start typing code

# SSH
- how do you connect to school's servers using SSH?
Use CPSC account!
`https://ucalgary.service-now.com/it?id=kb_article&sys_id=788f8c1adbbe5740fb10302f9d96195c`

- Windows: can use PuTTY, or WSL (can use CMD as well ...)
    - Host: `linuxlab.cpsc.ucalgary.ca`
    - port number: 22
    - can also get git bash/cygwin ...

- MAC: its based on BSD so no problem
- Linux: `ssh`
    - `ssh <your_CPSC_username>@linuxlab.cpsc.ucalgary.ca`
    - for me, it's `ssh haiyang.he@linuxlab.cpsc.ucalgary.ca`
    - can make it as an alias in bashrc

- start ssh agent on login / password less ssh
```
ssh-keygen
ssh-copy-id -i ~/.ssh/id_rsa username@host
```
- `https://www.ssh.com/academy/ssh/copy-id`

- if you are wondering how to start ssh agent at login (so that you don't need to type password everytime you push to github):
    - `https://stackoverflow.com/questions/18880024/start-ssh-agent-on-login`

- depends on what editors you guys use, VS code have SSH extension ... I don't use it, but I know it will bloat your file system quota in school's computers
    - probably not bad text editor, but its still kinda bloat

## Transfer files from remote computers
- can use sftp, pretty cool!

- used to transfer files from or to a server
- transfer from your own computer to a server:
```
scp hello.txt haiyang.he@linuxlab.cpsc.ucalgary.ca:~/testSSH
scp hello.txt haiyang.he@linuxlab.cpsc.ucalgary.ca:~/testSSH/anotherName.txt
```
- transfer from server to your own computer:
```
scp haiyang.he@linuxlab.cpsc.ucalgary.ca:~/testSSH/imNew.txt 
scp haiyang.he@linuxlab.cpsc.ucalgary.ca:~/testSSH/imNew.txt imNew2.txt
```
    - use `-r` option to transmit directories
    - can use wildcards ...
    - remote path format:
```
<username>@<hostname>:<path>`
```

- Suggestion in doing the assignments: create a github/gitlab repo! version control is great :)

# Linux commands
## Basics
- `pwd`: print current working directory (shows where in the filesystem you are at)
- `cd`: change directory (directory is a folder) to some some other ones
- `ls`: list directory contents
    - `-a` shows all files
    - `-l` lists one entry per line
- `touch <FILE>`: usually used to create a new file
- `mkdir <dirname>`: make directory
- `cat <List of Files>`: concatenate files in order
    - usually used to display a file to stdout
- `cp <src> <destination>`: copy things from a source to destination
    - `-r` option to copy recursively (copying a directory)
- `rm <file/dir>`: remove a thing
    - `-r` to recursively remove (delete a directory)
    - ***WARNING: everything deleted CANNOT be recovered***
- `mv <src> <destination>`: moves things
    - can be used for renaming
## Slightly advanced (by some arbitrary small epsilon), but SHOULD KNOW
- `man <command>`: opens up the man page for the command you want
    - there are different man pages! Usually wants the Linux programmer/POSIX programmer man page
    - eg: `man read` gives you the man page number 1, which is a general commands mannual for bash
    - `man read.2` gives the Linux man page
    - `man read.3` gives the POSIX man page
    - can open man pages in vim with colors by `:Man <command>` :) (I don't use it as frequently as I should be)
    - try `.2`, `.3` and `.7` for Linux and POSIX man page ... the number isn't always fixed

    - whenever you want to do something (either on command line or when programming), and you KNOW the command/function you need to use, use `man`! He's your man :)
    - sometimes (but very very rarely) man page can be wrong ...

- `make <commands>`: run the commands in a makefile
    - for each assignment, a makefile will be provided by the prof
    - can add your own commands ... probably talk/discuss later
    - many resources online if you want to be cool :)

- `grep [options] [files]`: can be used to search things
    - usually I use `-i`  to ignore case
    - can do on a directory with `-r`
    - to grep multiple things, use `-E` with an extended regex, or multiple `-e`'s with different patterns

- pipes: the `|` symbol with usage `command_1 | command_2 | ... | command_n`, that essentially takes stdout of `command_i` and feed it as stdin into `command_{i+1}`
    - https://en.wikipedia.org/wiki/Pipeline_(Unix)
    - eg: `ls | grep -i yourDesiredName`

- if you want to write own (bash/sh) scripts, you can write it as
```
#!/bin/bash
your bash script here
```
then you need to give it execution permission. You can simply do
```
chmod +x yourScript.sh
```
and it will give you the correct permissions (you can have lots of different permissions, `+x` is easiest to remember and to type).
Will talk more about it when assignment comes

## Useful "tips"
- default bash shell is emacs bindings (which I don't really mind actually)
- you can be a noob and use arrow up/down to bring up the previous commands you typed
- or you can be a giga chad and use ctrl-n and ctrl-p 
- ctrl/alt-b/f can bring you forward and backwards, ctrl-a and ctrl-e can move in front/back of the line, ctrl-/ will undo
- ctrl-u and ctrl-k to clear to left/right

