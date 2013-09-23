CPPFLAGS = -I../../src
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS  = -L../../src/posix++/.libs -lposix++

PROGRAMS = shmcat

all: $(PROGRAMS)

clean:
	$(RM) $(PROGRAMS) *.o *~

.PHONY: all clean
