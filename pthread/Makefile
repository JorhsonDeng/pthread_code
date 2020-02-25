 #多线程编程技术！
EXE = helloworld
SUB_DIR = src
CXX_SOURCES = $(foreach dir,$(SUB_DIR),$(wildcard $(SUB_DIR)/*.cpp))

CXX_OBJECTS = $(patsubst %.cpp,%.o,$(CXX_SOURCES))
CXX_DEPFILES = $(patsubst %.cpp,%.d,$(CXX_SOURCES))

#这个是头文件的添加选项
INCLUDE +=

#使用pthread创建线程在链接的时候必须加上如下的编译选项
LDFLAGS += -lpthread  
 
CXXFLAGS += -c -MMD -fPIC



$(EXE):$(CXX_OBJECTS)
	g++ $(CXX_OBJECTS) -o $@ $(LDFLAGS)


%.o:%.cpp
	g++ $(CXXFLAGS) $(INCLUDE) $< -o $@

-include $(CXX_DEPFILES)


clean:
	rm -rf $(EXE) $(CXX_OBJECTS) $(CXX_DEPFILES)

