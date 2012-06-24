# Fast change directory

# Motivation
If you are like me and STILL (it's 2012 doh!) want to spend most of the time in
console, this tool might come handy. A big amount of time is spend on
navigating in a directory structure jungle. You might like it but I came to a
conclusion that it would be a neat feature to just type `fcd dirname` and
choose from the list. Some of my files are located under different file system
that is mounted under `/mnt`. Although I've done a symbolic link to it under
my home directory it's still not convenient.

Fcd comes to save. By using different heuristics it tries to determine what
directory was intended to enter.


### TL;DR
This slick tool saves you from typing the directory names and can do
spellchecking against the parameter.

Purpose of the program is to deliver a fast way to navigate trough Unix
directory structure. The weight is in frequently visited directories so that
those can be easily accessed.

## TODO
Things I might consider at some point.

	[½] Separate databases for `/usr/`, `/home/`, and `/var/` and custom
	directory

	[½] Favourites / Frequently visited / Tags

	[½] Spellcheck

	[ ] UTF8 support

	[ ] Show current item number / total items / (MAX_ITEMS)

	[ ] Use file descriptors to pass data around (major speed up, but also needs
	major modifications)

	[ ] Support for configuration file

	[ ] Write SIGWINCH handler

## DONE:
	[X] Implement directory walk
	[X] Implement getopt functionality
	[X] Implement basic functionality

