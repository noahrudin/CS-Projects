all: mydash-project

mydash-project:
	cd mydash-src; make

dox:
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config > doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"

valgrind: mydash
	valgrind --leak-check=yes --suppressions=valgrind.supp --track-origins=yes mydash

clean:
	$(MAKE) -C mydash-src clean


