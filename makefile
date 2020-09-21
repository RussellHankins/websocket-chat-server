# https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html
# the compiler: gcc for C programs, defined as g++ for c++
CC = g++

#compiler flags:
# -g adds debugging information to the executable file
# -Wall turns off most, but not all compiler warnings
CFLAGS = -std=c++11

# define any libraries
LIBS = -lwebsockets -lpthread

# define the source files
SRCS = websocket.cpp chatcommand.cpp chatcommand_addchatroom.cpp chatcommand_adduser.cpp chatcommand_getchatroomlist.cpp chatcommand_getusers.cpp chatcommand_getusersinchatroom.cpp chatcommand_joinchatroom.cpp chatcommand_leavechatroom.cpp chatcommand_login.cpp chatcommand_logout.cpp chatcommand_nop.cpp chatcommand_removechatroom.cpp chatcommand_removeuser.cpp chatcommand_send.cpp chatcommand_sendto.cpp chatcommand_sendtoall.cpp chatclient.cpp chatroom.cpp datablock.cpp datastring.cpp Debug.cpp error_signals.cpp message.cpp parameters.cpp sha256.cpp StackTrace.cpp stringbuilder.cpp task.cpp tasks.cpp test.cpp user.cpp

# define the object files
OBJS = $(SRCS:.cpp=.o)

# define the main executable file
MAIN = websocket

# The following part of the makefile is generic.
# It can be used to make any executable just by changing the definitions above.

.PHONY:		depend clean

all:	$(MAIN)
		@echo websocket has been compiled

$(MAIN):	$(OBJS)
			$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)
			
# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule (a .cpp file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual about automatic variables)
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	$(RM) *.o *~ $(MAIN)

depend:	$(SRCS)
		makedepend $^

# DO NOT DELETE THIS LINE -- make depend needs it
