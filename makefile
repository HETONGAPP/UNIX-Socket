# compiler
CXX ?= g++
CXXFLAGS = -Wall -O2

# output directory (default: x86_64)
OUTPUT_DIR ?= output/x86_64

# target files
SERVER = $(OUTPUT_DIR)/server
CLIENT = $(OUTPUT_DIR)/client

# source files
SERVER_SRC = src/server.cpp
CLIENT_SRC = src/client.cpp

# automatically get git commit hash
GIT_HASH := $(shell git rev-parse HEAD)

# default target
all: $(SERVER) $(CLIENT)

# ensure output directory exists
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# rule: server
$(SERVER): $(SERVER_SRC) | $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) -DVERSION_HASH="\"$(GIT_HASH)\"" -o $@ $<

# rule: client
$(CLIENT): $(CLIENT_SRC) | $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# clean all output
clean:
	rm -rf output
