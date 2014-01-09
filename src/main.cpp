/*
 *          File: main.cpp
 * Last Modified: February 19, 2000
 *         Topic: Writing Make Files
 * ----------------------------------------------------------------
 */

#include <iostream>
#include "Point.h"
#include "Rectangle.h"

using namespace std;

int main()
{

  Point p1(1, 1);
  cout 
	<< "Point p1 at: " 
	<< p1.get_x() << ", " 
	<< p1.get_y() << endl;

  p1.move(3, 5);
  cout 
	<< "Moved p1 now at: "
    << p1.get_x() << ", " 
    << p1.get_y() << endl;

  Point p2(9,8);
  Rectangle r1(p1, p2);

  cout 
	<< "Rectangle r1 area is: " 
	<< r1.area() << endl;

  return 0;
}
