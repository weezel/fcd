# Fast change directory

### TL;DR
This slick tool saves you from typing absolute directory paths and can do
spellchecking against the given name.


# Motivation
If you are like me and STILL (yes it's 2012, I know) operate mostly through
terminal, this tool might come handy. A big amount of time is spend on
navigating in a directory structure jungle. Since most of the time I wander
around same directories, it would be neat to have a way to change directory
without typing the absolute path. Moreover, I was lacking a small hack project
so `Fast CD` was born.

Purpose of the program is to deliver a fast way to navigate trough Unix
directory structure. The weight is in frequently visited directories so that
those can be easily accessed.


# Logic behind the program
--Unfinished--

# Usage
Find directory that has `test` in its name. Since there can be many search
results we will like to choose which one was intended:

`fcd -a test`


### TODO
* Write SIGWINCH handler

* Replace %q -> %Q

* Open as readonly when reading

* ~~Implement basic functionality~~

* ~~Separate databases for `/usr/`, `/home/`, and `/var/` and custom directory~~

* ~~Functions to handle add/remove of a dir~~

* ~~Favourites / Frequently visited / Tags~~

* ~~Spellcheck~~ (needs improvements though)

* ~~Implement directory walk~~

* ~~Implement getopt functionality~~

* ~~Show current item number / total items / (MAX_ITEMS)~~

### FUTURE IMPROVEMENTS
Things I might consider at some point.

* UTF8 support

* Support for configuration file?

* Use file descriptors to pass data around?

