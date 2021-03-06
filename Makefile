GCC_WARNINGS1=-Wall -Wpointer-arith -Wstrict-prototypes
GCC_WARNINGS2=-Wmissing-prototypes -Wmissing-declarations
GCC_WARNINGS3=-Wnested-externs -fno-strict-aliasing
GCC_WARNINGS=$(GCC_WARNINGS1) $(GCC_WARNINGS2) $(GCC_WARNINGS3)
CFLAGS=-O2 -g $(GCC_WARNINGS) -I../git.git -DSHA1_HEADER='<openssl/sha.h>'
#CFLAGS+=-DDEBUG=1
GITPATH=../git.git
LIBS=-L$(GITPATH) -lgit $(GITPATH)/xdiff/lib.a -lssl -lcrypto -lz
YFLAGS=-d -l
LFLAGS=-l
CC=cc

OBJS=gram.o lex.o parsecvs.o cvsutil.o revdir.o \
	revlist.o atom.o revcvs.o git.o gitutil.o rcs2git.o \
	nodehash.o tags.o tree.o

parsecvs: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS): cvs.h
lex.o: y.tab.h

lex.o: lex.c

y.tab.h: gram.c

clean:
	rm -f $(OBJS) y.tab.h gram.c lex.c parsecvs parsecvs.exe
install:
	cp parsecvs edit-change-log ${HOME}/bin
