#ifndef CHESS_H
#define CHESS_H

/*
 * This is the function you need to implement to display a picture on the 
 * screen. You should call the interpreter function within it only once, 
 * passing the picture as a string array. The string array must end with 
 * a '0' in the last position.
 */
void display();

/*
 * Return a new picture, which is the original picture with reversed 
 * colors. The user is responsible for freeing the memory allocated for 
 * the string array (the picture).
 */
char** reverse(char**);

/*
 * Return a new picture, placing the second picture to the right of the 
 * first. The new picture will have a width equal to the sum of both 
 * pictures' widths.
 * The user must liberate the memory space reserved for the string array 
 * (the picture) 
 */
char** join(char**, char**);

/*
 * return a new picture, the original picture repeated a number of times 
 * on the side of each other
 * The user must liberate the memory space reserved for the string array 
 * (the picture) 
 */
char** repeatH(char**, int);

/*
 * return a new picture, the first one up to the second, the new picture will have 
 * the sum of hides from the both pictures.
 * The user must liberate the memory space reserved for the string array (the picture) 
 */
char** up(char**, char**);

/*
 * return a new picture, the original picture repeated a number of times 
 * down of each other
 * The user must liberate the memory space reserved for the string array (the picture) 
 */
char** repeatV(char**, int);

/*
 * return a new picture, the first picture over the second picture
 * The user must liberate the memory space reserved for the string array (the picture) 
 */
char** superImpose(char**, char**);

/*
 * return a new picture, the vertical mirror from the original picture
 * The user must liberate the memory space reserved for the string array 
 * (the picture) 
 */
char** flipV(char**);

/*
 * return a new picture, the horizontal mirror from the original the picture
 * The user must liberate the memory space reserved for the string array 
 * (the picture) 
 */
char** flipH(char**);

/*
 * return a new picture, the original picture rotated anti-clockwise
 * The user must liberate the memory space reserved for the string array 
 * (the picture) 
 */
char** rotateL(char**);

/*
 * return a new picture, the original picture rotated clockwise
 * The user must liberate the memory space reserved for the string array 
 * (the picture) 
 */
char** rotateR(char**);

/*
 * Do not try to implement this at home, unless the teacher authorizes it. 
 * It could be dangerous for your heatlh 
 */
void interpreter(char**);
#endif
