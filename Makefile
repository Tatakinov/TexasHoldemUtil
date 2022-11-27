LIBRARY			= TexasHoldemUtil.dll
CXX				= clang++
CXXFLAGS		= -I . -Wall -Ofast -std=c++17
LD				= clang++
LDFLAGS			= -shared -lmsvcrt -Xlinker /NODEFAULTLIB:LIBCMT
OBJS			= saori.o saori_windows.o header.o request.o response.o estimate.o
ALL				= all

.SUFFIXES: .cc .o

.PHONY: all
all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(LD) $(LDFLAGS) -o $(LIBRARY) $(OBJS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o *.exp *.lib *.exe *.dll
