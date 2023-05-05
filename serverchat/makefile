cc = clang++
object = ./bin/main.o ./bin/thread_threadpool.o ./bin/action.o ./bin/fun.o
parameter = -L /usr/lib/mysql -lmysqlclient -I/usr/include/mysql -lglog

chatserver : $(object)
	$(cc) $(object) $(parameter) -o chatserver

./bin/main.o : main.cc server.hh
	$(cc) -c main.cc -g -o  ./bin/main.o

./bin/thread_threadpool.o : thread_threadpool.cc server.hh
	$(cc) -c thread_threadpool.cc -g -o ./bin/thread_threadpool.o

./bin/action.o : action.cc server.hh
	$(cc) -c action.cc -g -o ./bin/action.o

./bin/fun.o : fun.cc server.hh
	$(cc) -c fun.cc -g -o ./bin/fun.o

clean :
	rm $$( find . -name "*.o") ;

#debug（用于编译调试版本）
debug: parameter += -g
debug: chatserver

#release（用于编译发布版本）
release: parameter += -O2
release: chatserver

#clean-all（用于清除所有已编译文件及执行文件）
clean-all :
	rm $$( find . -name "*.o") ;
	rm chatserver
