CC=gcc
CXX=g++
RM=rm -f
#CPPFLAGS=-O3 -g -mrtm -fno-omit-frame-pointer 
CPPFLAGS=-O3 -g -fno-omit-frame-pointer -mrtm
LDFLAGS=-O3 -g  -lboost_timer -lboost_system -lboost_thread 
LDLIBS=

SRCS=$(wildcard *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

.PHONY: all depend clean dist-clean record report

all: fft_pipeline

fft_pipeline: $(OBJS)
	g++ $(LDFLAGS) -o $@ $(OBJS) 

profile: rb_tree
	perf stat -e r1c9,r2c9,r4c9,r8c9,r10c9,r20c9,r40c9,r80c9 `pwd`/$^ 2>&1 | sed -e 's/r1c9/txstarts/' -e 's/r4c9/tx aborts/' -e 's/r2c9/tx commits/' -e 's/r8c9/tx aborts memory/' -e 's/r10c9/tx aborts uncommon condition/' -e 's/r20c9/tx aborts unfriendly instruction/' -e 's/r40c9/tx aborts incompatible memory type/' -e 's/r80c9/tx aborts none of the prev. 4 (e.g. interrupt)/' 

record: rb_tree
	perf record -g -e r1c9,r2c9,r4c9:pp,r8c9,r10c9,r20c9,r40c9,r80c9 `pwd`/$^ 

report: record 
	perf report

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
