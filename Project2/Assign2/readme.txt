Author: Tim Lindquist (Tim.Lindquist@asu.edu)
        Software Engineering, CIDSE, IAFSE, Arizona State University Polytechnic
Version: January 2018

See http://pooh.poly.asu.edu/Ser321/Assigns/Assign2/assign2.html

Purpose: Sample Java program demonstrating Ant builds, a simple
javax.swing GUI for waypoint manipulation, and sample java program using
a Waypoint class.

This program is executable on MacOS, or Linux.

Classes in Java to demonstrate defining a simple GUI with javax.swing
components, and controlling that GUI. This program represents the
View (WaypointGUI class) and Control (SampleAssign2 class) components.

The Assign2 project includes an Ant build file, with the following
targets: prepare, clean, build.gui, execute.gui, build.waypoint, and targets.
To execute ant using the build.xml in this directory, you will need to
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

To run the GUI example, from a bash shell in the project directory, execute the
command:
ant execute.gui

To run the Waypoint program:
ant build.waypoint
java -cp classes Waypoint

this will prompt for sample waypoint input. See the file samples.txt which
contains several example waypoints. Enter the waypoints and then the last
line of input should be calc. See the web page:
  http://www.movable-type.co.uk/scripts/latlong.html
for the algorithms for providing full implementations of the great circle
distance and heading.

To clean the project (remove the .class files) execute:
ant clean


