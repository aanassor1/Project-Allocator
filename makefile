# First, as you can see, any line that begins with # is a comment.
#
# Second, make sure your makefile is actually called "makefile" or
# "Makefile". Don't call it by any other name, or with any suffix
# like Makefile.txt. It won't work.

# In the below, the line with a colon specifies a target to compile,
# and its dependencies are listed after the colon. It means if anything in
# the dependency list is changed, that target will be recompiled.
# The next line is the command to compile that target (or indeed to do
# anything).
# Note that the second line MUST BEGIN WITH A TAB (NOT SPACES).


# This is a simple makefile when all your code are in a file GenAlloc.cpp.

All: all
all: GenAlloc

GenAlloc: GenAlloc.cpp System.o
	g++ GenAlloc.cpp System.o -o GenAlloc

System.o: System.cpp System.h
	g++ -c System.cpp -o System.o