Jigsaw-Solver
=============

A jigsaw puzzle solver for randomly shuffled rectangular shaped images.

**generate_pieces.cpp** takes the image file name as parameter and creates the scrambled images in the **generated_pieces** folder.
**compile_all.sh** contains scripts to compile all the cpp files.

System-Requirements
===================

1. gcc 4.0+ compiler.
2. libopencv


GENERATING PIECES ( JIGSAW PIECES )
===================================

If you have an image ( jpg format ) , you would need to cut it out into jigsaw pieces ( to solve it later ! ).
To do this, follow the steps :

1. Run compile_GeneratePieces.sh ( ensure that its been given root access ) . This will compile the code , and generate an exectable named **generate_pieces**.
2. Run the Command : ./generate_pieces <name of jpg image>     . ( Note : It requires the full address of the image as a Command Line Argument ).
 
Note : After running the command, it would ask for the "length of each jigsaw block" to be cut into. Then squares of that length would be cut out from the image, shuffled and will be saved into folder generated_pieces/  named as {1.jpg .. N.jpg} ,And the remaining part of the Initial image left out will be cropped.

JIGSAW SOLVER
==============

Now, as we already have the jigsaw square pieces, we would like to solve the jigsaw and retrieve the Solved Image.
To do this, follow the steps : 

1. Run compile_solver.sh ( ensure that its been given root access ) . This will compile the code , and generate an executable named **solver**.
2. Run the Command ./solver

Note : Then It would ask for the Number of Jigsaw Pieces are there  (in the folder generated_pieces/ folder ). 
Note : There is a TIME LIMIT of 15 seconds , ie , the code will run for 15 seconds and then generate a Solved Image.

3. There will appear a new image named **final.jpg** , which would be the Solved Image for the Jigsaw.

AUTHORS
=======

The Whole Project was developed by :
1. Anmol Gulati , IIT Kharagpur
2. Biswajit Paria , IIT Kharagpur

We developed this **Jigsaw-Solver** for the following contest : http://www.robotix.in/events/event/sudocode . This was a National Level Online Contest , and we were placed 1st.

ALGORITHM-USED
===============

We have used two methods to solve the Jigsaw.
1 . First One is an MST solver, which basically makes a weighted graph(based on gradient difference) between the pieces, and picks out a **Planar Minimum Spanning Tree**.
2 . Second One uses **Genetic Algorithm** to solve the **Jigsaw Puzzle** . 

As, the **Genetic Algorithm** takes much time to converge to the Solution ( but provides better results ). We Run the **MST Solver** if the Number of Jigsaw Pieces are Over 400 , else we run the **Genetic Algorithm Solver** . 

FUTURE PLANS
============

We would love to have people contribute to this project.
One of the things on our mind is to have more methods to Solve the Jigsaw, and also Run all these Parallely and pick out the best solution.

NOTE
====

For Any Queries/Issues regarding the project, you can contact any of us at , anmol01gulati@gmail.com OR biswajitsc@gmail.com  .