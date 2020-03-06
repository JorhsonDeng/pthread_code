 #多线程编程技术！
EXE = main
#SUB_DIR = src
SUB_DIR = .
CXX_SOURCES = $(foreach dir,$(SUB_DIR),$(wildcard $(SUB_DIR)/*.cpp))

CXX_OBJECTS = $(patsubst %.cpp,%.o,$(CXX_SOURCES))


CXX_DEPFILES = $(patsubst %.cpp,%.d,$(CXX_SOURCES))

#只需要include相应的头文件即可
#前面必须用-I标识符
INCLUDE += -I./include

#使用pthread创建线程在链接的时候必须加上如下的编译选项
LDFLAGS += -lpthread  
 
#CXXFLAGS += -c -MMD -fPIC
CXXFLAGS += -c -MMD

$(EXE):$(CXX_OBJECTS)
	g++ $(CXX_OBJECTS) -o $@ $(LDFLAGS)


%.o:%.cpp
	g++ $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LDFLAGS)

-include $(CXX_DEPFILES)


clean:
	rm -rf $(CXX_OBJECTS) $(CXX_DEPFILES) $(EXE)

