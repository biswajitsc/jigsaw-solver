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

1. Compile the jigsaw generating code using the following code.
```bash
chmod +x compile_GeneratePieces.sh
./compile_GeneratePieces.sh
```
This generates an executable named ```generate_pieces```.

2. Run the following command after replacing ```myimage.jpg``` with the address of a JPEG image.
```bash
./generate_pieces myimage.jpg
```
3. After running the command, enter the side the side length of the square pieces into which the image will be dissected into. Square images of the entered size would be generated, shuffled and will be saved in the folder ```generated_pieces``` as *1.jpg, 2.jpg* and so on.

The solver assumes that the scrambled images can be arranged into a square grid to generate the solved image. Hence the jigsaw generator crops out a maximum possible square from the entered image so as to make it possible to generate jigsaw pieces of the proper format.

Jigsaw Solver
-------------

Once we have the scrambled jigsaw pieces, we can use the jigsaw solving code to retrieve the original image. 

1. Compile the jigsaw solving code using the following code.
```bash
chmod +x compile_solver.sh
./compile_solver.sh
```
This generates an executable named ```solver```.

2. Run the following command to retrieve the original image from the images generated in the folder ```generated_pieces```.
```bash
./solver
```
3. After running the command, enter the number of rows of the square grid into which the jigsaw pieces would be fit into.
	For example, 64 generated images would fit into a 8x8 square grid. Hence we enter 8.

4. Both the scrambled and the solved images would be generated and saved in files ```scrambled_image.jpg``` and ```solved_image.jpg``` respectively.

Algorithms used
---------------

We have used two methods to solve the problem.

1 . The first method is based on a **Minimum Spanning Tree** construction of the images. We constructed a modified form of the minimum spanning tree of the pieces by considering the mean squared pixel value difference of the edges of the jigsaw pieces as the edge weights of the graph.
2 . The second method is based on a **Genetic Algorithm**. This method selects the best images from a pool of *evoluting* images based on a fitness function and *crosses* them together. The crossing is performed in a way so as to improve the resulting image.

Since, the Genetic Algorithm takes much time to converge to the solution ( but provides much better results ), we run the **MST Solver** if the number of jigsaw pieces are over 400. Otherwise we run the **Genetic Algorithm Solver** for smaller datasets. 

Authors
-------

The whole project was contributed to by the following people.

1. Anmol Gulati , IIT Kharagpur
2. Biswajit Paria , IIT Kharagpur
3. Kumar Krishna Aggarwal, IIT Kharagpur

This **Jigsaw-Solver** was developed for the following contest: http://www.robotix.in/events/event/sudocode. We were place 1st in this national level contest.

Future Plans
------------

We would love to have people contribute to this project.
Some of the ideas we have in our minds are
* Solve the jigsaw for a general rectangular grid.
* Have non-square dimensions for the jigsaw pieces.
* Include more methods to solve the jigsaw.
* Run all methods parallely and output the best result.

Contact
-------

For any queries/issues regarding the project, you can contact any of us at anmol01gulati@gmail.com or biswajitsc@gmail.com.