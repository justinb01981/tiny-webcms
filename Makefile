TARGET=httpfsd
all:
	g++ -D_UNIX_=1 -pthread -o ${TARGET} auth.cpp cpthread.cpp httpfileserver.cpp httpfsutils.cpp simplesocket.cpp

clean:
	rm ${TARGET};
