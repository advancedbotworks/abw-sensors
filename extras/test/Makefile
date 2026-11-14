# Host unit tests for the AdvancedBotWorks library (no Arduino needed).
#   make -C extras/test        build and run
#   make -C extras/test clean
CXX      ?= g++
CXXFLAGS ?= -std=c++11 -O2 -Wall -Wextra -Werror
SRC_DIR  := ../../src/sensors
BUILD    := build

all: run

$(BUILD)/test_irring_math: test_irring_math.cpp $(SRC_DIR)/IRRingMath.cpp $(SRC_DIR)/IRRingMath.h
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -o $@ test_irring_math.cpp $(SRC_DIR)/IRRingMath.cpp -lm

run: $(BUILD)/test_irring_math
	./$(BUILD)/test_irring_math

clean:
	rm -rf $(BUILD)

.PHONY: all run clean
