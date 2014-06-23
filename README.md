Jigsaw-Solver
=============

A jigsaw puzzle solver for randomly shuffled rectangular shaped images.

System-Requirements
-------------------

The following need to be installed in your linux system to compile the code.

1. gcc 4.0+ compiler.
2. libopencv


Generating Jigsaw Pieces
------------------------

A JPEG image can be used to generate randomly shuffled jigsaw pieces (to solve it later!) in the following manner:

1. Compile the jigsaw generating code
'''
chmod +x compile_GeneratePieces.sh
./compile_GeneratePieces.sh
'''
This generates an executable named **generate_pieces**.

2. Run the Command after replacing "myimage.jpg" with the address of a JPEG image.
'''
./generate_pieces myimage.jpg
'''
3. After running the command, enter the side the side length of the square pieces into which the image will be dissected into. Square images of the entered size would be generated, shuffled and will be saved in the folder 'generated_pieces' as '1.jpg', '2.jpg' and so on.

The solver assumes that the scrambled images can be arranged into a square grid to generate the solved image. Hence the jigsaw generator crops out a maximum possible square from the entered image so as to make it possible to generate jigsaw pieces of the proper format.

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