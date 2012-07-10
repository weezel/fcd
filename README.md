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


## TODO
	[½] Separate databases for `/usr/`, `/home/`, and `/var/` and custom
	directory

	[½] Favourites / Frequently visited / Tags

	[½] Spellcheck

	[ ] Use file descriptors to pass data around?

	[ ] Support for configuration file?

	[ ] Write SIGWINCH handler

## DONE:
	[X] Implement directory walk

	[X] Implement getopt functionality

	[X] Implement basic functionality

	[X] Show current item number / total items / (MAX_ITEMS)

## FUTURE IMPROVEMENTS
Things I might consider at some point.

	[ ] UTF8 support

