# GeneticWalk
My first project that deals with genetic algorithm. In this project I tried to implement
some simple pathfinding with genetic algorithm. 

I used OpenGL for fun because I wanted to refresh some 3D knowledge. Based on this simple version I will in the future change the map, put obstacles,
and many more things !

## Basic samples

Bellow a simple sample of the first generation tracing their own way. Note that the red point is the destination point.

<p align="center">                                                                                                                                                      
<img src =samples/first_gen.gif/>                                                    
</p>

40 Generations later, we see that many people overlap though there is sometimes mutation. There is still not the global optimum
path, but it is way better than the first generation. I should work on the fitness function again, and punishing much more the repeated position in order to tend to remove all the 'LEFT' => 'RIGHT' => LEFT' kind of move.

<p align="center">                                                                                                                                                      
<img src =samples/last_gen.gif/>                                                    
</p>
