Both servers and client is coded similarly, each with its own class 
server and client respectively. 

After creating the objects in main, both programs are executed by the 
execute method. server.execute() client.execute()

The execute method creates everything necessary for the client to connect
to the server and the server creates all objects necessary for accepting 
a connection. 

After execute finishes, the currentState function is called. The server 
wait a command, while the client inputs the command. After the client 
inputs a command, both follow their respective instructions and 
invoke the sendReceiveStuff method. This method contains a switch 
function that handles all the available instructions, upload(1), 
download(2), and quit(3). 

Server handles upload by opening a file outputFile for output. It reads
the socket for a infinite amount of time until it the client sends a "end".
This is where the server knows that file sending is finished. It then closes 
the outputFile and setState back to sleep. For download, the server reads a 
file name from the client to be downloaded. It opens a file for reading and 
writes the file in chunks of 1024 and if less than 1024bytes are left, it writes
the remaining to the client. Before finishing, it sends a "end" to the client 
to indicate that it has finished the "download". Case 3 is just acknowledgement that 
client has quit and does nothing. The client side of this code is basically in the same
format.