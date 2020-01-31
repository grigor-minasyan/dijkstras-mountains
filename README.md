# dijkstras-mountains                                  
For this assignment, you will use the BRIDGES library to visualize pathfinding
your way through the mountains of Colorado. BRIDGES is a neat library, written
by UNCC, that helps new programmers out by allowing them to visually see
the results of their work.

Your goal will be: given a starting row and column, and an ending row and
column, to use Dijkstra's Algorithm to find the shortest path between the two.
Each tile represents a 1.84 square kilometer so a horizontal move is a
cost of 1840, and a diagonal move is a cost of 2601. However, each point on
the grid has a vertical elevation. Any elevation change costs 20x the change
in vertical elevation (either up or down) in meters. So a move to the right on level
land only is a total distance of 1840, but a move to the right with 10m of
elevation gain is a cost of 2040.

The cost of moving from one tile to the next is given by the following
formula:
horizontal cost (1840m for lateral movement / 2601m for diagonals) +
20x(vertical change in meters)

The reason for this is that mountains are much easier to walk around than to
go over, but sometimes it's worth a little elevation gain to avoid long
detours. 

Any negative elevation in the input file should be treated as blocking terrain.
This will prevent you from routing through the ocean on the Korea map.

To make your life easier, I have provided the source code from the BRIDGES
people that implements a greedy algorithm which always starts on the left edge
of the map, and walks to the right, making a choice between the three squares
to the right at each step, minimizing total elevation change between each
step. This is called a "greedy" algorithm as it makes locally optimal
decisions, but will not result in globally optimal results. For that, you need
to do Dijkstra's. 

The code is provided, though, so you have starter code that reads the map
data, writes images to the BRIDGES server, and shows you how to treat a 1D
array as if it was a 2D array. (If you want to rewrite it using a vector of
vectors, that might be a good idea, too. That's what I did.)

To get an A on this assignment you must:
1) (Three grades) Write a Dijkstra's or A* algorithm that pathfinds from the starting
location to the end, minimizing the total distance travelled (given the cost
function above which takes into account vertical distance at 20x the cost).

2) (One grade) Write a red line (yes, you'll need to change the color from the staring
code on the map showing the route taken, and visualize it on
BRIDGES (this bit is done for you, so just don't break it). You must also
change the title of the BRIDGES visualizaton from Greedy to Dijkstra's.

3) (One grade) You must use the NOAA grid extract tool to download elevation data for some
region on earth and run your algorithm on it. Go here:
http://maps.ngdc.noaa.gov/viewers/wcs-client/, select a region, choose to save
it as an ArcGIS ASCII Grid and then download it. Use WinSCP (or another SCP
tool if you use a Mac or GNU/Linux box) to transfer the file into your
directory, and then run it by typing "a.out filename" where filename is the
name of the file you downloaded. BRIDGES has given with two input files,
colorado1.dat and colorado2.dat, that you can use to test your code. Look at
how colorado1.dat is formated and make the file you download match that
format. It's pretty easy, but it won't work as you have downloaded it. I have
included maps for Tahoe, the whole Central Valley, and Korea.

To get a grade, you must allow me to run your code on all six maps (the five
ones I've given you and the one you created.) Don't crash on the map of the
Central Valley or Korea! Negative elevations aren't handled by default, so it
throws an exception when trying to set the color to a negative value.

To get BRIDGES working, you must register on their website to get a username
and API key. Enter these into the source code on the first couple lines of
main where you see variables for your name and API key. You can pick any
assignment number you want - I'm using 1001 for this one. The code is set to
not compile until you get a BRIDGES account, using a static assert.

Hopefully you will learn lots of things by seeing how other people write code.
Every instution has its own style and way of doing things.

Credit:
This assignment is a modded version of the Mountain Paths Nifty Assignment here:
http://nifty.stanford.edu/2016/franke-mountain-paths/

The starting source code is from the BRIDGES team at UNCC:
http://bridgesuncc.github.io/
