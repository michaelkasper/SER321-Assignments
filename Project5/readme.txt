Author: Tim Lindquist (Tim.Lindquist@asu.edu), ASU Polytechnic, CIDSE, SE
Version: February 2018

See http://pooh.poly.asu.edu/Mobile
See http://pooh.poly.asu.edu/Ser321

Purpose: demonstrate Json-RPC with server in CPP, and command-line clients
in both languages. The stubs for CPP are created using libjson-rpc-cpp, and the
Java stub is manually created using java.net.HttpURLConnection class.
The server and terminal clients are executable on both MacOS and Debian Linux.

Communication between the service and its clients is done using JSON-RPC.
The purpose of the example is to demonstrate JSON and JSON-RPC. Reference
the following sources for background on these technologies:

JSON (JavaScript Object Notation):
 http://en.wikipedia.org/wiki/JSON
 The JSON web site: http://json.org/

JSON-RPC (JSON Remote Procedure Call):
 http://www.jsonrpc.org
 http://en.wikipedia.org/wiki/JSON-RPC

Building and running the server and terminal clients is done with Ant.
This example depends on the following frameworks:
1. Ant
   see: http://ant.apache.org/
2. Json for Java as implemented (reference) by Doug Crockford.
   See: https://github.com/stleary/JSON-java
3. Using the package manager apt-get, install the C++ JsonRPC and jsoncpp frameworks
   See the course page for instructions, and follow the build instructions for C++:
   http://pooh.poly.asu.edu/Ser321/Resources/buildVB.html

To build and run the examples, you will need to have Ant installed on
your system, with the target extensions for building C++
(see the CppFraction example from unit1).

ant build.server   // build the cpp server
./bin/studentRPCServer 8080 // to execute the server

The C++ JsonRPC framework generates server and client-side proxies based on the methods that are
described using Json in the file: StudentLibraryMethods.json
See the Class Notes on Object-Based Client-Server Communications and the C++ framework usage
described at:
https://github.com/cinemast/libjson-rpc-cpp

To execute the client, start a new terminal in the same folder:
ant build.java.client
execute the java client with the command:
java -cp classes:lib/json.jar sample.student.client.StudentCollectionClient localhost 8080

To execute the client, start a new terminal in the same folder:
ant build.client   // build the cpp client
./bin/studentRPCClient http://localhost:8080   // to run the client.

You should run the server on your Raspberry Pi and the clients on your Virtual Box Debian.
When doing so, the host name or ip for the Raspberry Pi should be used when invoking clients.

To clean the project directory:
ant clean

The project directory also includes scripts to demonstrate one approach that
can be used to run a processes in the background on either
linux or MacOS. This allows you to start up a server on your Raspberry Pi,
then to log off of the RPi and have the server continue to run until the system
goes down, or you log in again to stop the service. The approach shown in this
project directory is preferred unless you want the service to start-
up automatically everytime the system is re-booted. If so, then use the system-specific
mechanism for creating a service. Linux uses the files in /etc/init.d as start-ups for
services.

Running a program in the background, and allowing it to continue after you log off.

To do this, you should provide execute permission to the shell scripts accompanying
this example (the jar tool strips them off).
chmod u+x backgroundStart.sh background.sh
You will find these bourne shell (/bin/sh) scripts in the project base directory.
These are shell programs (command files). When you execute backgroundStart.sh it
will start a server, and record its proces id. The script assigns the standard
output, error, and input streams (stdin, stderr, and stdout) to files. Run:
./backgroundStart.sh ./bin/calculateRPCServer 8080
  
After executing this command script, the server will run in the background until you kill it.
To see its status, and get the information needed to kill it (PID), use the ps command:
ps
which will show you a listing of your active programs. The process id (PID) of each
running program will be shown on the left, and the command that started is shown as
the last string displayed. To terminate a running program, you can use:
kill -9 pid
the -9 is a signal to the process indicating it should not ignore the kill.

The other included script command file (background.sh) does something similar,
but provides the ability to start, stop or restart a server. If you execute
./background.sh start ./bin/calculateRPCServer 8080
The CPP server will be started in the background as with backgroundStart.sh, but
it places the starting command and the standard input, error, and PID files in the
log directory. You can use this same script to stop or restart the program with:
./background.sh stop
./background.sh restart
When using either of these scripts, you will find the output of the server logged to files:
log.txt (for backgroundStart.sh) and log/log.txt (for background.sh). There are also error logs.

If you don't use one of these scripts, you may experience servers being stopped, but not
terminating. Thus, when you try to start them again, the new process won't be able to
attach to the requested port because its already being used by a prior invocation of the
server. When you run a server manually, as shown above, you must terminate it using the
shell ps (process status) and kill commands, as explained above.
When you want to shut down the Java server started as shown
above you can stop it with <Ctrl-C> (control key and c key depressed simultaneously) or <Ctrl-Z>. Doing
so does not terminate the program, it only stops it. One way to terminate a stopped program is to find
its process id (pid) with ps from the terminal as:
ps
This command shows you all of the active processes running under your login-id.
The pid is the leftmost number printed for the running server process (java). Then terminate it
by executing the following from the terminal:
kill -9 pid

end
