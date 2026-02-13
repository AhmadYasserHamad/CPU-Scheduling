all: lab7
	@chmod +x lab7

lab7: lab7.cpp lab7.h
	@g++ -std=c++17 lab7.cpp -o lab7

clean:
	@rm lab7