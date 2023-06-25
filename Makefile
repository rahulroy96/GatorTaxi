CC = g++ 

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
# CFLAGS  = -g -Wall -O2 -Wno-deprecated -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment
CFLAGS  = -g -Wall

gatorTaxi: 
		$(CC) $(CFLAGS) rideTaxi.cpp -o gatorTaxi


clean:
	rm -f *.o
	rm -f *.out
	rm -f gatorTaxi