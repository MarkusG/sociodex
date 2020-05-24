# sociodex
A contact list on steroids - an ncurses frontend to a PostgreSQL database of people and much more

# screenshots
![](https://i.imgur.com/2nxHvNL.png)
![](https://i.imgur.com/HZ8fbcr.png)

# Compiling
```
$ git clone https://github.com/MarkusG/sociodex.git
$ cd sociodex
$ meson build && cd build
$ ninja
```

# Troubleshooting
In case of a fatal error, the program will write to standard error. As ncurses takes control of the terminal, standard error must be redirected in order to review the error:
```
$ ./sociodex 2> error.txt
```
