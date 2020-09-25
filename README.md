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

websocket Port ServerPassword  
	starts the websocket server listening to Port.  
	ServerPassword is the password to edit users/chatrooms.  

websocket test classname  
	Runs the test for classname.  

websocket test testnumber  
	testnumber can be from 1 to 5  

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
	getchatroomdetails(messageid,chatroomid)
		Returns the chatroom details such as name,question,info in a chatroomdetails message.  
		Returns an error if the chatroomid wasn't found.  
	getchatroomlist(messageid)  
		Returns a list of all chatrooms. The list is returned with the chatroomlist command.  
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
	logout(messageid,disconnect)  
		Logs the connection out. If disconnect is not 0, then also disconnects.  
		Sends a success message if disconnect == 0.  
		Sends an error message if you're already logged out and disconnect == 0.  
	nop()  
		No operation. This is used by the client to keep the connection alive. No response.  
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
	sendtoall(server_password,messageid,message)  
		Returns success(messageid) or error(message,messageid).  
		sends privatemessage(chatclientid,userid,username,message) to all users.  
		If user isn't logged in, userid = 0 and username is blank.  
	
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
	error(message,messageid)  
		The message had an error.  
	error(message)  
		The message had such a bad error, the messageid wasn't parsed out.  
	msg(messageid,chatclientid,userid,message)  
		A private message from the sendto command.  
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
