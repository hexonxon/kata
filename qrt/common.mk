CXX := clang++
CXXFLAGS := -Wall -fsanitize=address -O0 -ggdb3 -std=c++03
LDFLAGS := -fsanitize=address

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
