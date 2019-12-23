# defines the needed .c files in alphabetic order
sourceFies = graph.c heap.c main.c queue.c

# gather all .c files and sort (alphabetically)
files = $(sort $(wildcard *.c))

#checks if you have every file you need
ifneq ($(files),$(sourceFies))
$(error You're missing some (if not all) the necessary source files for this project, try getting them at 'github.com/EdPirro/USP-SCC0216' then try again)
endif

# simple compile all .c files with gcc
all exe exe.exe:
	gcc $(files) -o exe

# defines run and clean if windows is being used
ifeq ($(OS), Windows_NT)

# runs the exe file created
run: exe.exe
	./exe

# simply remove the exe and every .bin file created
clean: exe.exe
	powershell "rm exe.exe"

# defines run and clean for non-windows system (Tested on Ubuntu18.04LTS only) 
else 

# runs the exe file created
run: exe
	./exe

# simply remove the exe and every .bin file created
clean: exe
	rm exe
endif