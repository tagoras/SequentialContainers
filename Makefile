main.exe: main.o
	g++ main.o -o main.exe
	rm -f main.o

main.cpp:
	g++ -c main.cpp -o main.o