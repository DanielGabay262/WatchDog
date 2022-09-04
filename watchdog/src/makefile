CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall
INCLUDES = -I /home/dgabay/git/daniel-gabay/projects/watchdog/include
DEBUGFLAG = -Wextra -g
RELCFLAG = -DNDEBUG -O3
LDFLAGS = -shared
SHARED_TEST = -L. -Wl,-rpath=/.
SRCS = $(wildcard /home/dgabay/git/daniel-gabay/projects/watchdog/src/*.c)
OBJS = $(wildcard /home/dgabay/git/daniel-gabay/projects/watchdog/src/*.o)
TEST_FILES = $(wildcard /home/dgabay/git/daniel-gabay/projects/watchdog/test/*.c)
VLGFLAG = valgrind --leak-check=yes --track-origins=yes
DEBUGGER = cgdb


all : 
	make get_o_d && make libds_d.so &&make get_o_r && make libds_r.so	&& make clean_o

test:
	make get_o_d && make libds_d.so && make clean_o && make tests_d

debug : 
		make get_o_d && make libds_d.so && make clean_o
		
release : 
		make get_o_r && make libds_r.so && make clean_o
		
libds_d.so : $(OBJS)
		$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDES) $(DEBUGFLAG) $(OBJS) -o $@
	
libds_r.so : $(OBJS)
		$(CC) $(LDFLAGS) $(CFLAGS) $(INCLUDES) $(RELCFLAG) $(OBJS) -o $@	
	
get_o_d : $(SRCS)
		$(CC) $(INCLUDES) $(CFLAGS) $(DEBUGFLAG) -c -fPIC $(SRCS) 
		
get_o_r : $(SRCS)
		$(CC) $(INCLUDES) $(CFLAGS) $(RELCFLAG) -c -fPIC $(SRCS)
	
tests_d : libds_d.so $(TESTS)
		for CURRENT_FILE in $(TEST_FILES);  do \
        	EXE_NAME=`basename -s _test.c $$CURRENT_FILE`; \
        	$(CC) $(CFLAGS) $(DEBUGFLAG) $(INCLUDES) $(SHARED_TEST) -o $$EXE_NAME.out $$CURRENT_FILE -lds_d;\
        done
        
tests_r : libdata_structure_d.so $(TEST_FILES)
		for CURRENT_FILE in $(TEST_FILES);  do \
        	EXE_NAME=`basename -s _test.c $$CURRENT_FILE`; \
        	$(CC) $(CFLAGS) $(RELCFLAG) $(INCLUDES) $(SHARED_TEST) -o $$EXE_NAME.out $$CURRENT_FILE -lds_r;\
        done
				 	
clean_lib: 
		rm *.so
		
clean: 
		rm *.o *.out
		
clean_o: 
		rm *.o		

	
	
	
	
	
	
	
	
		
		
