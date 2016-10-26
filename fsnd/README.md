CS 425 - Lab1: SocketProgrammig:fsend

Authors:
meinenec - Emmanuel Meinen
kronhoew - Erik Kronholm

Description:
fsend takes parameters to open a socket to listen, or send data over the internet. It takes both the role of the client and the server depending on the given parameters.

Tasks Accomplished:
We can send a file from a client to server.
Gained a better understanding of the C language.

How to Compile:
Run make or use "gcc fsnd.c -o fsnd"

Execute:
For the server:
./fsnd -l -p port

For the client:
./fsnd -p port -n bytes -o offset serverAddress inputFile

Reading output:
By default the only output is the file to stdout on the server side. Running -v on the client gives IP, port, and file information.

Further Questions:
We had an issue getting the transfer to work from one thingy to another but we were able to send on localhost fine.
