# motion_planning_nearest_neighbours
MPNN, kd-tree nearest neighbor implementation for mixed topologies based on Yershova and LaValle's paper.

So to compile: g++ -std=c++14 -o main main.cpp -larmadillo -lGLEW -lGL -lglfw

The kd-tree works in layers, and you pass the topology for each coordinate (0 = real line, 1 = circle (S_1) ), and bounds...
bounds for the S_1 layers are [0,1]. Sorry!

The example is a kd-tree on a torus; which is a cartesian product S_1 x S_1. Added an extra parameter for the sake of experimentation.

![image](res_100.png)

The red point is the nearest neighbour, on a torus, to the position of the mouse.

The yellow points are the K nearest neighbours of the mouse.

May later be used for a RRT implementation.
