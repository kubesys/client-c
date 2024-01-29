CC := g++
CFLAGS := -Wall -std=c++17
LDFLAGS := -lcurl -pthread -lstdc++fs
SRC_DIR := .
INCLUDE_DIR := include
KUBESYS_DIR := kubesys

INCLUDE_FILES := $(wildcard $(INCLUDE_DIR)/*.h)
KUBESYS_SRCS := $(wildcard $(KUBESYS_DIR)/*.cpp)
KUBESYS_HEADERS := $(wildcard $(KUBESYS_DIR)/*.h)
OBJS := $(SRC_DIR)/test.o $(KUBESYS_SRCS:.cpp=.o)
EXECUTABLE := test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

$(SRC_DIR)/test.o: $(SRC_DIR)/test.cpp $(INCLUDE_FILES)
	$(CC) $(CFLAGS) -c $< -o $@

$(KUBESYS_DIR)/%.o: $(KUBESYS_DIR)/%.cpp $(KUBESYS_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) 
