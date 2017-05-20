
CXXFLAGS = -m32

INCLUDES=-I. -I../include

MYCXXFLAGS=-g $(CXXFLAGS) $(INCLUDES)

MYLDFLAGS=$(LDFLAGS) -lminigui_ths -lpthread -lm -lpciaccess

define all-cpp-files-under
$(shell find $(1) -name "*."$(2) -and -not -name ".*" )
endef

define all-subdir-cpp-files
$(call all-cpp-files-under,.,"cpp")
endef


CPPSRCS	 = $(call all-subdir-cpp-files)

CPPOBJS	:= $(CPPSRCS:.cpp=.o)

INCS	 = -I./tp -I./cpp -I./tp/widget -I./tp/res  -I./  -I../

TARGETS=video

APPLDFLAGS=$(MYLDFLAGS) -L. -ldl

all:$(TARGETS)

video:$(CPPOBJS)
	$(CXX) $(MYCXXFLAGS) $(CPPOBJS) -o video $(APPLDFLAGS)

$(CPPOBJS) : %.o : %.cpp
	$(CXX) $(MYCXXFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f *.o $(TARGETS)



