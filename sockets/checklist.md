### Map Server and Client

1.
- [x] Call the client and server "mapclient" and "mapserver" with appropriate C and header files.
- [x] Define a common header file where common protocol messages (see below) are defined as well as the default port (23032) and IP (127.0.0.1)
- [ ] Allow IP override via argv (on both client and server programs).

2. 
- [x]  The client connects to the server and sends a request consisting of the ASCII character ‘M’ followed by either binary 0 (a single int) or two binary values, W IDT H and HEIGHT (two ints).
- [ ]  0 means, the client does not care about the map size and server decides the size.
- [x] The server accepts, spawns a child, and reads from the socket and interprets the client’s request for its validity.  
- [x] The ‘M’ message in the protocol always has ‘M’ followed by W IDT H and HEIGHT, followed by the entire map.
- [ ] The ‘E’ message is followed by a single int, which is the length of the error message string that follows.
- [x] The client reads and interprets the reply. A map is output then to STDOUT, the error to STDERR.
- [ ] Any other message types currently would generate an error on either client or server sides (“unrecognized protocol message”).
- [ ] The map server should read the default map from the /dev/asciimap device and write it to the client socket. 
- [ ] At each valid client’s request the server seeks to the beginning of the map.


3. 
- [ ] Implement client and server logging where message exchanges and non-fatal errors are logged.
- [ ] Write your log outputs to a file, which you can name "mapclient.log" and "mapserver.log" respectively (or any other reasonable name). 
- [ ] You can use fprintf and friends to write to these files. You can use tail -f to watch the log files update in separate terminal windows.

4. 
- [ ] Test the communications by reading and printing the maps to STDOUT on the client side 10 times.
- [ ] You can restrict your tests simply to the local system.

6.
- [x] Adjust your Makefile to compile and run the server, client, and any relevant tests.
- [ ] Implement killing off any server processes as well in your Makefile.
- [ ] After starting the server in its Makefile target, output a netstat run confirming the server is listening as intended (or confirming that it's not listening as intended).