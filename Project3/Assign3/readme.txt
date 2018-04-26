Author: Tim Lindquist (Tim.Lindquist@asu.edu)
        Software Engineering, CIDSE, IAFSE, Arizona State University Polytechnic
Version: January 2018

See http://pooh.poly.asu.edu/Ser321/Assigns/Assign3/assign3.html

Purpose: Sample C++ programs demonstrating Ant builds and an FLTK
 GUI for Waypoints. Another program demonstrates Waypoint class.

This program is has been tested on Linux and MacOS (build.xml only works on
Linux).

To execute Ant using the build.xml in this directory, you will need to
copy the file: antlibs.jar from the lib directory to your home directory:
cp lib/antlibs.jar ~
or
cp lib/antlibs.jar $HOME/
Note that ~ (tilde) is a shortcut for $HOME
then extract the antlibs.jar file:
pushd ~
jar xvf antlibs.jar
pushd -0
The pushd commands manipulate a stack of directories for switching your
bash's current directory. The first pushd pushes home onto the stack and
switches the current directory to home. The second pushd takes you
back to whatever directory you were in before the first.

To run the example GUI, from a bash shell in the project directory, execute the
command:
ant execute.gui

To run the example Waypoint program:
ant build.waypoint
./bin/waypoint
use the file samples.txt (copy and paste lines) to provide waypoint input to
the program. Then enter calc to end.

To clean the project (remove the .class files) execute:
ant clean


