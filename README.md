# websocket-chat-server
Multithreaded websocket chat server in C++ in Linux using the libwebsockets library with example html/javascript code.

websocket is a chatroom websocket server.
Written in C++ using libwebsockets: 
https://libwebsockets.org/lws-api-doc-master/html/md_READMEs_README_8coding.html

To test out the program, run it with the port number and the server password like ./websocket 8000 test  
Then, open up a few browser windows like http://localhost:8000/  
Those browser windows will be able to talk to each other. example.html is the test web page.

Q. Why do a chatroom in C++ instead of C#?  
A. Because C# has a "stop the world" garbage collector.  

Q. Is this application multithreaded?  
A. Yes. There are two threads. One sending and receiving messages  
   from a queue when clients are available and one thread processing messages.  
   Because only one thread is processing messages, no locking is needed.
   
Q. If there's no database connection, how does authentication work?
A. After the client logs into a web page on the server that shows the list
   of chat servers, the server connects to the websocket engine and adds
   the client that's about to log in using the server password. When the
   client logs in a few seconds later using javascript, the login command
   will find his credentials.

Q. Is the file io finished?
A. No. read_directory, read_file, write_file,erase_file are not finished.

websocket Port ServerPassword  
	starts the websocket server listening to Port.  
	ServerPassword is the password to edit users/chatrooms.  

websocket test classname  
	Runs the test for classname.  

websocket test testnumber  
	testnumber can be from 1 to 8  

websocket test all  
	Runs all the tests. The last test is the Debug test.   
  Running the Debug test crashes the program.  
  But if debugging is working, you can tell where the program crashed at with a stack trace.  
	
Clients connect and send commands to the server. The server sends commands back.  
Commands consist of the command name then a ( then parameters then )  
Numbers are sent comma delimited.   
Examples: 3,12,16  
Strings are sent with the length in base 10 then a comma then the text.  
There's no comma after the text because the length is already known.  
Examples: 4,test6,client  
messageid is a unique number the client sends with each message.  
The messageid if the server response will match the messageid of the client.  
If a message comes from another client, that messageid won't match  
because each client assigns its own message id.  

##Commands the client send to the server:  
	addchatroom(messageid,room_name,question,answer_hashed,server_password)  
		Adds a chatroom.  
		Returns chatroomcreated(messageid,chatroomid,room_name) or error(message,messageid).  
		If server_password is not sent then everyone_can_join and delete_if_unused are true.  
		question and answer and answer_hashed can be blank.  
	adduser(messageid,userid,username,password,server_password)  
		The server_password is required. The idea is that   
		some other service connects and feeds this from a database.  
	erase_file(messageid,filename) - Not finished.
	    erases a file. Returns a success message or an error message.
	    filenames for existing files are from whatever file_location they're found on.
	    All file names have the userid as a directory prefix.
	    If a subfolder is empty, the subfolder is deleted.
	getchatroomdetails(messageid,chatroomid)
		Returns the chatroom details such as name,question,info in a chatroomdetails message.  
		Returns an error if the chatroomid wasn't found.  
	getchatroomlist(messageid)  
		Returns a list of all chatrooms. The list is returned with the chatroomlist command.	
	getsnapcount(messageid) - Not finished.
		Gets the number of snaps you have. Returns a snapcount message 
		or an error message if you're not logged in.
	getsnaps(messageid)		
		After calling this, all of your snaps will start being sent to you.
		They will be snap messages (see message types below).
		Returns a success message or an error message if you're not logged on.
	getusersinchatroom(messageid,chatroomid)  
		Returns the list of users/clients in the chatroom. usersinchatroom message.  
	getusers(messageid,status,server_password)  
		Gets the list of users in ram. Server_password is required.  
		If status == 1, only return users that are logged in.  
		If status == 2, only return users that are logged off.  
		If status == 3, return both users logged in and users logged off.  
		Returns a users message or an error.  
	joinchatroom(messageid,chatroomid,leave_other_chatrooms,answerhashed)  
		Joins a chatroom. Answerhashed is required for chatrooms with question/answers.  
		If leave_other_chatrooms is not zero then any other chatrooms are left.  
	leavechatroom(messageid,chatroomid)  
		Leaves a chatroom. Returns a userleftchatroom message or an error.  
	login(messageid,username,password)  
		Logs in. Returns success or error messages.
		If the user isn't found then it will call a hard coded api url
		to get the userid or a user not found message.
	logout(messageid,disconnect)  
		Logs the connection out. If disconnect is not 0, then also disconnects.  
		Sends a success message if disconnect == 0.  
		Sends an error message if you're already logged out and disconnect == 0.		
	nop()  
		No operation. This is used by the client to keep the connection alive. No response.  
	read_directory(messageid,subdirectory) - Not finished.
	    Reads a directory. Returns the file_name,file_type,last_modified_date,length.
	    Tab delimited. Returns a data message or an error message.
	    All directories are under a file_location and the user's userid folder.	    
	read_file(messageid,filename,start_byte,length) - Not finished.
	    Reads some of filename. Returns a data message or error message.
	    filenames for existing files are from whatever file_location they're found on.
	    All file names have the userid as a directory prefix.
	readsnap(messageid,snapid)
		Call this after you read a snap.
		resends a snap message to both users of the snap or an error message.
	removechatroom(messageid,chatroomid,server_password)  
		Removes a chatroom. Requires server password.  
		Sends a chatroomwasdeleted message to you and everybody in the chatroom.  
	removeuser(messageid,userid,server_password)  
		Removes a user from ram. Server_password is required.  
	send(messageid,chatroomid,message)  
		Send a chat message to everybody in the chatroom.  
	sendto(messageid,chatclientid,userid,message)  
		Sends a message to a specific user or chatclientid.  
		Chatclientid and userid are optional, but you must send at least one.  
		Sends message to the user or error back to the caller. See msg below.  
		Sends a success message or an error message back the the client.
	sendsnap(messageid,userid,expirelength,readtime,snap)
		Sends a snap. Snaps delete after expirelength seconds.
		The user doesn't have to be online to receive it.
		A snap_sent message or error message is returned.
		expirelength is the number of seconds before the message deletes. 
		expirelength of 0 means delete the message when it's read.
		readtime is the number of seconds the target user can read the message. If 0 then infinity.
	sendtoall(server_password,messageid,message)  
		Returns success(messageid) or error(message,messageid).  
		sends privatemessage(chatclientid,userid,username,message) to all users.  
		If user isn't logged in, userid = 0 and username is blank.
	write_file(messageid,filename,start_byte,length,data) - Not finished.
	    Writes to a filename. Writing starts at start_byte. Data can be text or binary.
	    Returns a success message or an error message.
	    filenames can have subfolders. New subfolders will be created if necessary.
	    filenames for new files are from the active (default:first) file_location. 
	    filenames for existing files are from whatever file_location they're found on.
	    All file names have the userid as a directory prefix.
	
	
##Commands the server sends to the client:  
	chat(messageid,chatroomid,chatclientid,userid,message)  
		A message from somebody in the chatroom.  
	chatroomcreated(messageid,chatroomid,room_name,info,number_of_clients)
		Some user created a chatroom.  
	chatroomdetails(messageid,chatroomid,room_name,info,number_of_clients,question)
		Returns details about the chatroom.
	chatroomlist(messageid,[chatroomid,room_name,info,people_count])  
		The list of chatrooms. [ ] is a array. No [ or ] is in the data.  
		info is a number:  
			bit 1 = Are you in this chatroom.  
			bit 3 = Does this chatroom require a password. This would be an answer to a question.  
			bit 4 = Will this chatroom be deleted when everybody leaves.  
		people_count is the number of people currently in the chatroom.
	chatroomwasdeleted(messageid,chatroomid)  
		A chatroom was just deleted. 
	data(messageid,data_length,data)
	    A data message is how data is returned from a file. data can be text or binary.
	error(message,messageid)  
		The message had an error.  
	error(message)  
		The message had such a bad error, the messageid wasn't parsed out.  
	msg(messageid,chatclientid,userid,message)  
		A private message from the sendto command.  
	snap_sent(messageid,snapid)
		Acknowledgement that you sent a snap.
	snap(snapid,to_userid,from_userid,datesent,datereceived,readtime,snap)
		A snap message. These messages show up randomly after a call to getsnaps.
		datesent and datereceived are in seconds since the birth of disco 1/1/1970.
		A datereceived of 0 means they haven't received the message yet.
		readtime is the number of seconds the target user can read the message. If 0 then infinity.
	success(messageid)  
		The message was processed successfully.  
	userjoinedchatroom(messageid,chatroomid,chatclientid,userid,username,number_of_clients)  
		The user joined a chatroom. number_of_clients is the number of clients currently in this chatroom.
	userleftchatroom(messageid,chatroomid,chatclientid,userid,username,number_of_clients)  
		The user left the chat room. number_of_clients is the number of clients currently in this chatroom.
	users(messageid,[userid,username,status])  
		List of users. status is 1 for logged in, 2 for logged out.  
	usersinchatroom(messageid,chatroomid,[chatclientid,userid,username])  
		List of users in a chatroom.  
	yourchatclientid(chatclientid)  
		This is your chatclientid. Each time you connect, you get a different  
		chat client id.  
	
If the program crashes, there's a good chance you'll see a stack trace of where it crashed at.   
Not all functions will give a stack trace.  
General Protection Faults can't be caught with a try/catch block in Linux. Instead, a signal is thrown just before your program aborts.  
There's nothing you can do. Your program is going to abort. But with some clever programming, you'll see a stack trace.  

Multithreading:
  There are fast tasks and slow tasks. There's a thread that processes fast tasks. This thread
  processes one task at a time (from queue the_websocket->chatroom_tasks). Responses are added
  to the chatclient's messages_to_send queue. If a fast task decides it'll take a while, it 
  will create a new task object and add it to the slow task queue (the_websocket->slow_tasks). 
  A separate thread will processes these commands one at a time. Their output goes to 
  chatclient->add_message_from_slow_command. Slow tasks are tasks that require data from
  another server. The chatclient->get_next_message() function will get the next message from
  the fast messages_to_send queue. If that queue is empty, it will look in the 
  chatclient->messages_to_send_from_slow_commands queue. The purpose of this is so messages like
  keystrokes or mouse clicks for games are sent faster.
  
