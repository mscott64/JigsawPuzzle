JigsawPuzzle
============

Setup: http://surflab.cise.ufl.edu/wiki/Getting_Started_with_OpenGL_in_VisualC%2B%2B_2010
-in case there is trouble with the GLUT or glew

All source files are in the src folder and images are in the images folder.

11/9/2013 - Crude puzzle implementation - There are pieces that can be moved by clicking on them and sliding the mouse around. There are zooming capabilities using the n (zoom-in) and m(zoom-out) keys. Quit with (escape, q or Q).

11/12/2013 - Laying the foundation for texture mapping.

11/13/2013 - Texture layered on pieces. There is an issue on the edges, but I can't do anything about that. There are three puzzles (falcons, castle, and tech). There are three types of puzzle (EASY, MEDIUM, and HARD).

11/15/2013 - Puzzle functionality: Press Ctrl and click on a piece to pick it up. (You don't have to hold Ctrl down while moving the piece). When the selected piece is in the right spot, it connects to its neighbor and they move as one.

11/16/2013 - Keyboard piece manipulation - Select a piece by clicking it. (No need to hold Ctrl anymore). To deselect a piece, click outside the pieces. Flip a piece by selecting it. Then pressing f/F to flip in the x direction and d/D to flip in the y direction. Rotate a piece by selecting it, then press r/R to rotate to the right and e/E to rotate to the left. (This works for single and joined pieces.)

11/17/2013 - Fixed bugs introduced to join by rotation and flipping. Joining works as expected.