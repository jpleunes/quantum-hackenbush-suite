BUILD_DIR=build

# DEFINES=-DDEBUG
CFLAGS=-Wall -Wextra -Wpedantic -std=c++20 -O3

TARGET = $(BUILD_DIR)/quantum-hackenbush-suite

SRCS := $(shell find . -name '*.cpp')
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

$(TARGET) : $(OBJS)
	g++ $(DEFINES) $(CFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o : %.cpp
	mkdir -p $(@D)
	g++ $(DEFINES) $(CFLAGS) -I. -c -o $@ $<

clean :
	rm -f -r $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	touch $(BUILD_DIR)/.gitkeep
