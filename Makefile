BUILD_DIR=build

CFLAGS=-Wall -Wextra -Wpedantic -std=c++17

TARGET = $(BUILD_DIR)/quantum-hackenbush-suite

SRCS := $(shell find . -name '*.cpp')
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

$(TARGET) : $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o : %.cpp
	mkdir -p $(@D)
	g++ $(CFLAGS) -I. -c -o $@ $<

clean :
	rm -f -r $(BUILD_DIR)
