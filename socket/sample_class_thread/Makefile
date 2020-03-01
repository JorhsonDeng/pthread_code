 #多线程编程技术！
EXE = helloworld
SUB_DIR = src
CXX_SOURCES = $(foreach dir,$(SUB_DIR),$(wildcard $(SUB_DIR)/*.cpp))
CXX_SOURCES += main.cpp

CXX_OBJECTS = $(patsubst %.cpp,%.o,$(CXX_SOURCES))


CXX_DEPFILES = $(patsubst %.cpp,%.d,$(CXX_SOURCES))

#只需要include相应的头文件即可
#前面必须用-I标识符
INCLUDE += -I./include

#使用pthread创建线程在链接的时候必须加上如下的编译选项
LDFLAGS += -lpthread  
 
#CXXFLAGS += -c -MMD -fPIC
CXXFLAGS += -c -MMD

Sever_source += ./src/server.cpp ./src/Socket.cpp ./src/Thread.cpp
Sever_object = $(patsubst %.cpp,%.o,$(Sever_source)) 
Sever_DEPFILES = $(patsubst %.cpp,%.d,$(Sever_source)) 

Client_source += ./src/client.cpp ./src/Socket.cpp ./src/Thread.cpp
Client_object = $(patsubst %.cpp,%.o,$(Client_source)) 
Client_DEPFILES = $(patsubst %.cpp,%.d,$(Client_source)) 

server:$(Sever_object)
	g++ $(Sever_object) -o $@ $(LDFLAGS)

client:$(Client_object)
	g++ $(Client_object) -o $@ $(LDFLAGS)

$(EXE):$(CXX_OBJECTS)
	g++ $(CXX_OBJECTS) -o $@ $(LDFLAGS)


%.o:%.cpp
	g++ $(CXXFLAGS) $(INCLUDE) $< -o $@

-include $(CXX_DEPFILES)


clean:
	rm -rf server client $(Sever_object) $(Sever_DEPFILES) $(Client_object) $(Client_DEPFILES)

