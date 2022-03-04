CFLAGS = -O3 -g -Wall -pthread
TARGET = main_base

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)


