# 编译器和选项设置
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

# 查找kubesys目录下的所有cpp文件
SRC_FILES = $(wildcard kubesys/*.cpp) test.cpp

# 生成目标文件列表
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# 最终的可执行文件名
EXECUTABLE = my_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 编译每个源文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)

