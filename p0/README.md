# Project 0

This is a little *Hello World* project intended to refresh your memory on coding in C, and to teach or remind you how to use the lab machines in the UW-Madison CS department.  In particular, you should be able to do the following:

* Remotely connect to one of the department Linux lab machines
* Use basic commands in the terminal
* Understand the file-directory structure on AFS (specific for CS537)
* Use `micro` to edit, `gcc` to compile, and `gdb` to debug
* Write code in C
* Run tests (specific for CS537)
* Turn in your solution (specific for CS537)

## Project Structure

Your projects for the semester will be provided to you via DoIT's GitLab repository.  The URL for course projects is
[https://git.doit.wisc.edu/cdis/cs/courses/cs537/spring24/public](https://git.doit.wisc.edu/cdis/cs/courses/cs537/spring24/public).  The instructions and
any starter code will be found there.

You will want to clone the repository for the project, follow the instructions to complete your solution, run any provided tests to make sure your solution is correct on the CS Lab machines,
and turn in your solution to the handin directory.  This project 0 is intended to make sure you can do all of those things.

## Remote Connections to CS Lab Machines

Your solutions to the projects will be tested on the CS department Linux lab machines.  Because of this, you may want to clone the repository and do all of your work there.  You can physically go to the
[labs](https://csl.cs.wisc.edu/docs/csl/2012-08-16-instructional-facilities/) or [remote connect](https://csl.cs.wisc.edu/docs/csl/2012-08-16-remote-access-csl-computers/) to the machines.  This
document will go over how to remotely connect to the machines.

I recommend using SSH to remotely connect.  [PuTTY](https://www.ssh.com/academy/ssh/putty/download) is a popular SSH client for Windows.  For Macs you can open a terminal window and type an ssh command
to connect.

To connect to a Linux lab machine, you can type in the following:

```
ssh <username>@best-linux.cs.wisc.edu
```

Where `<username>` is your username.  Using the computer name `best-linux.cs.wisc.edu` will connect you to the least busy Linux lab computer.  You will probably be prompted for your password and
will get a dual-factor authentication push to Duo.  After you have finished authenticating you will be placed at a command prompt which should show you your username, the name of the computer to which
you were connected, and the directory you are currently in.  for example:

```
oliphant@royal-26:~$
```

This shows that the username is `oliphant` on the computer `royal-26` and currently in the directory `~` (your home directory).


## Basic Terminal Commands and the CS Department's File Directory Structure

Each user has their own home directory where you can create files and directories to do your work.  Your home directory alias is `~`.  To see the actual location of your home directory
you can type the command `pwd` to see the present working directory.  In my case it shows:

```
oliphant@royal-26:~$ pwd
/home/oliphant
```

You should be familiar with several terminal commands including:

* pwd - present working directory
* ls - list
* cd - change directory
* mkdir - make directory
* mv - move
* cp - copy
* cat - concatenate files and print on the standard output
* grep - print lines of a file that match patterns

If you need some brushing up on the Linux command line, I recommend reading the online short book [The Linux Command Line](https://linuxcommand.org/tlcl.php).

Besides your own home directory, the course also has a home directory.  The course's home directory alias is `~cs537-1` (note that this is the case even if you're registered for section 2).  You will want to become familiar with this directory
as you will be turning in your solutions to projects in this directory, receiving feedback through this directory, and finding and running the tests for projects from this directory.  You
can change to the directory by using its alias and listing the contents:

```
oliphant@royal-26:~$ cd ~cs537-1
oliphant@royal-26:/home/cs537-1$ ls
handin	private  project-archive  project-snapshots  public  tests  xv6
```

You have a sub-directory under `handin` where you will be submitting your solutions to projects and where we will provide feedback to you.  The `tests` directory contains tests that you can run
on your project solution to see if it is correct.  You will also be using the starting code from the `xv6` directory for some of your projects.  The other directories you do not have access to.

## Cloning and Starting Your Projects

Make sure you can sign into UW-Madison DoIT's GitLab on-line repository at [https://git.doit.wisc.edu/](https://git.doit.wisc.edu/).  Use the button **UW-Madison NetID** to sign in with your
UW-Madison credentials.  You will need to sign in once on the web before you can clone repositories from the command line.

In the terminal window, change back to your home directory, make a sub-directory for project 0, and clone the repository into this sub-directory:

```
oliphant@royal-26:/home/cs537-1$ cd ~
oliphant@royal-26:~$ mkdir project0
oliphant@royal-26:~$ cd project0
oliphant@royal-26:~$ git clone https://git.doit.wisc.edu/cdis/cs/courses/cs537/spring24/public/p0
```

You will be prompted for your GitLab username and password and then the on-line directory will be copied to your local directory.  This is a public repository and you will not be able to push your changes
back to the repository.  However, you can add and commit your changes locally.  If you are not familiar with git and repositories, I recommend reading chapter 1 and 2 of the on-line
[Pro Git book](https://git-scm.com/book/en/v2).

## Creating and Editing Code with `micro`

If you plan on working remotely, you will need a simple editor for working in a terminal window.  The department has installed [micro](https://micro-editor.github.io/), a intuitive terminal-based text editor.
It allows you to have multiple windows open at the same time, use standard command shortcuts (e.g. *ctrl-c*, *ctrl-v*, *ctrl-f*, etc.) as well as your mouse to interact with the editor.  You can read more
about the basic shortcut bindings by typing alt-g when you are running micro or get a more complete list [here](https://github.com/zyedidia/micro/blob/master/runtime/help/defaultkeys.md).

Change to the p0 repository directory and create and edit the file "hw.c":

```
oliphant@royal-26:~/project0$ cd p0/
oliphant@royal-26:~/project0/p0$ micro hw.c
```

This will open micro, editing the file hw.c.  Type in the c code for a simple "Hello World" program:

![Hello World Program](images/hello_world.png)

Save the code (ctrl-s) and open a terminal window within micro to compile and run your program.  Use ctrl-e to open the command bar within micro and type in the command "hsplit" to horizontally
split the window.  Then type ctrl-e and type the command "term" to start up a terminal within that pane.  Now you can alternate between editing your code and compiling and executing your program
by clicking between the two panes or pressing ctrl-w.

Within the terminal pane, type the command to compile and execute your program:

```
oliphant@royal-26:~/project0/p0$ gcc hw.c
oliphant@royal-26:~/project0/p0$ ./a.out
hello, world
```

## Coding in C

You should be familiar with C basics including compiling with `gcc`, debugging using `gdb`, creating and using `Makefile`s, and using the `man` program to explore the C libraries and system call documentation
within Linux.  If you need a refresher on any of these things, the textbook has a [Lab Tutorial](https://pages.cs.wisc.edu/~remzi/OSTEP/lab-tutorial.pdf) to help you get up to speed.

## Running Tests

Along with each project a series of tests will be released (often a little later than when the initial project is released) that you can run to verify that your solution is working correctly.  These
tests are not meant to be exhaustive.  Additional tests may be run beyond these tests when you submit your final solution.  Feel free to open and explore these test files to understand what they
are testing and how they are testing it.  They are written in a combination of shell scripts and python code.

Each project's testing scripts are a bit different.  There should be a `README` file in the project's testing directory to explain how to run the tests.  In general, you will want to be in a terminal
window in your project's directory on a lab machine and execute the test script from there.

For example, to test your hello world p0 project, make sure you are in the directory where your `hw.c` file is located and type the following command:

```
oliphant@royal-26:~/project0/p0$ ~cs537-1/tests/P0/test-p0.csh
A.OUT...
TEST 0 - clean build (program should compile without errors or warnings)
RESULT passed

TEST 1 - See if the output is "hello world\n"
RESULT passed

TEST SCORE: 1
```

## Handing In Your Project Solution

There is a directory `~cs537-1/handin/<username>` where <username> is your username.  Under this directory there are sub-directories for each of the projects this semester where you must turn in your
solution.  It is critical that the file organization within these project directories be exactly correct for our autograder scripts to run properly.  Make sure you read and follow the instructions
for each project on the structure of your project's handin directory.

For this p0 project you should have a single file, `hw.c`, in your p0 handin directory.

The project handin directory will be copied by a script on the due date/time.  If you do not have your files in this directory at that time then you will fail the project.  Recall that you do have "slip
days" for turning in projects late.  Every 24 hours past the due date/time the directory will be copied again until the late day period has ended.  If the directory has changed after the due date then the
latest version will be the one tested and points will be taken off based upon the late policy.

## Project Results

After the late period has completed, your code will be tested and results will be placed back in your handin directory letting you know how you did on the project.  The grade for your project will be
uploaded to Canvas.  If you have any issues with the grading, please follow the instructions for requesting a regrade of your project.
