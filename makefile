# compiler
CXX = g++
CXXFLAGS = -Wall -O2

# output directory
OUTPUT = output

# target files
SERVER = $(OUTPUT)/server
CLIENT = $(OUTPUT)/client

# source files
SERVER_SRC = src/server.cpp
CLIENT_SRC = src/client.cpp

# automatically get git commit hash
GIT_HASH := $(shell git rev-parse HEAD)

# default target
all: $(SERVER) $(CLIENT)

# ensure output directory exists
$(OUTPUT):
	mkdir -p $(OUTPUT)

# rule: server
$(SERVER): $(SERVER_SRC) | $(OUTPUT)
	$(CXX) $(CXXFLAGS) -DVERSION_HASH="\"$(GIT_HASH)\"" -o $@ $<

# rule: client
$(CLIENT): $(CLIENT_SRC) | $(OUTPUT)
	$(CXX) $(CXXFLAGS) -o $@ $<

# clean
clean:
	rm -rf $(OUTPUT)
