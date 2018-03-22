cxx = g++-7
cflag =  -std=c++14 -g -Wall -pthread
target = chapter1 chapter3 chapter3_thread_safe_stack
all:${target}
chapter1:chapter1.cpp
	${cxx} -o $@ ${cflag} $<
chapter3:chapter3.cpp
	${cxx} -o $@ ${cflag} $<
chapter3_thread_safe_stack: chapter3_thread_safe_stack.cpp
	${cxx} -o $@ ${cflag} $<
.PHONY: all clean
clean:
	rm ${target}
