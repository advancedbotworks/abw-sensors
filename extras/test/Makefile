# Host unit tests for the AdvancedBotWorks library (no Arduino needed).
#   make -C extras/test        build and run everything
#   make -C extras/test clean
CXX      ?= g++
CXXFLAGS ?= -std=c++11 -O2 -Wall -Wextra -Werror
SRC      := ../../src
BUILD    := build
INCLUDES := -I$(SRC) -I$(SRC)/core -I$(SRC)/sensors

all: run

$(BUILD)/test_irring_math: test_irring_math.cpp $(SRC)/sensors/IRRingMath.cpp $(SRC)/sensors/IRRingMath.h
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ test_irring_math.cpp $(SRC)/sensors/IRRingMath.cpp -lm

$(BUILD)/test_lego_version: test_lego_version.cpp $(SRC)/core/LegoVersion.h $(SRC)/AdvancedBotWorksVersion.h
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ test_lego_version.cpp

# library.properties version must match ABW_VERSION in AdvancedBotWorksVersion.h
check-version:
	@props=$$(sed -n 's/^version=//p' ../../library.properties); \
	header=$$(sed -n 's/^#define ABW_VERSION[[:space:]]*"\(.*\)".*/\1/p' $(SRC)/AdvancedBotWorksVersion.h); \
	if [ "$$props" = "$$header" ]; then echo "PASS version_matches ($$props)"; \
	else echo "FAIL version mismatch: library.properties=$$props, AdvancedBotWorksVersion.h=$$header"; exit 1; fi

run: $(BUILD)/test_irring_math $(BUILD)/test_lego_version check-version
	./$(BUILD)/test_irring_math
	./$(BUILD)/test_lego_version

# Code formatting (clang-format 18, style in /.clang-format).
# lego_uart.h is third-party (Pybricks) and kept as upstream.
CLANG_FORMAT ?= clang-format
FORMAT_FILES = $(shell cd ../.. && git ls-files 'src/*.h' 'src/*.cpp' 'examples/*.ino' 'extras/test/*.cpp' | grep -v 'src/core/lego_uart.h')

format:
	cd ../.. && $(CLANG_FORMAT) -i $(FORMAT_FILES)

format-check:
	cd ../.. && $(CLANG_FORMAT) --dry-run --Werror $(FORMAT_FILES) && echo "PASS formatting"

clean:
	rm -rf $(BUILD)

.PHONY: all run clean check-version format format-check
