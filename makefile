CXX = g++
CXXFLAGS = -std=c++17
TARGET = matrix_ops

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET)
