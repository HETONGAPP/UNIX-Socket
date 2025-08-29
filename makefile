# 编译器
CXX = g++
CXXFLAGS = -Wall -O2

# 目标文件
SERVER = server
CLIENT = client

# 源文件
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

# 自动获取 git commit hash
GIT_HASH := $(shell git rev-parse HEAD)

# 默认目标
all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -DVERSION_HASH="\"$(GIT_HASH)\"" -o $@ $<

$(CLIENT): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(SERVER) $(CLIENT)
