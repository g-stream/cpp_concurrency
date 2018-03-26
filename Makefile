cxx = g++-7
cflag =  -std=c++14 -g -Wall -pthread
target = chapter1 chapter3 chapter3_thread_safe_stack chapter2 chapter3_lock_mult_mutexes chapter3_unique_lock chapter4_cvqueue\
chapter4_future_promise chapter4_wrap_future chapter4_time chapter5_atomic chapter5_sequential_consistency\
chapter5_relaxed_ordering chapter5_relaxed_ordering_2 chapter5_acquire_release chapter5_acquire_release_2 \
chapter5_comsumer_semantic chapter5_atomic_queue chapter5_fence
all:${target}
#chapter1:chapter1.cpp
#	${cxx} -o $@ ${cflag} $<
#chapter3:chapter3.cpp
#	${cxx} -o $@ ${cflag} $<
#chapter3_thread_safe_stack: chapter3_thread_safe_stack.cpp
#	${cxx} -o $@ ${cflag} $<
#chapter2:chapter2.cpp
#	${cxx} -o $@ ${cflag} $<
${target}: %:%.cpp
	${cxx} -o $@ ${cflag} $<
${target:=_run}:%_run:%
	./$<
test:${target:=_run}
	
.PHONY: all clean test ${target:=_run}
clean:
	rm ${target}
