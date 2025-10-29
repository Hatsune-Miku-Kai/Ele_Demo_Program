TARGET=nrc2.out
all :
	aarch64-linux-gnu-g++ -o nrc2.out src/*.cpp -I./include -L./lib -lNexRob -lpthread -lm -ldl -lrt -std=c++11

clean :
	rm $(TARGET) $(objects)
