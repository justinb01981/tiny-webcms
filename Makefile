TARGET=httpfsd
all:
	g++ -g -Wno-write-strings -D_UNIX_=1 -pthread -o ${TARGET} auth.cpp cpthread.cpp httpfileserver.cpp httpfsutils.cpp simplesocket.cpp

clean:
	rm ${TARGET};
