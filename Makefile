# define constants
CXXFLAGS = -Wall -O
CXX=g++

gnss_l86_example: gnss_l86_example.o gnss_l86_lib.o
	$(CXX) $(CXXFLAGS) -o gnss_l86_example gnss_l86_example.o gnss_l86_lib.o -lwiringPi

gnss_l86_lib.o: gnss_l86_lib.h gnss_l86_lib.cpp
	$(CXX) $(CXXFLAGS) -c gnss_l86_lib.cpp

gnss_l86_example.o: gnss_l86_example.cpp
	$(CXX) $(CXXFLAGS) -c gnss_l86_example.cpp

clean: 
	rm -f *.o
