#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <string>

class Point
{	
	public: 
		float X, Y;

	public:
		Point(float X = 0, float Y = 0) : X(X), Y(Y) {};

		//ADD+
		inline Point operator+ (const Point& p2) const {
			return Point(X + p2.X, Y + p2.Y);
		}

		//ADD+=
		inline friend Point operator +=(Point& p1, const Point& p2) {
			p1.X += p2.X;
			p1.Y += p2.Y;
			return p1;
		}

		//SUBTRACT -
		inline Point operator- (const Point& p2) const {
			return Point(X - p2.X, Y - p2.Y);
		}

		//SUBTRACT-=
		inline friend Point operator -=(Point& p1, const Point& p2) {
			p1.X -= p2.X;
			p1.Y -= p2.Y;
			return p1;
		}

		//MULTIPLI*
		inline Point operator* (const float scalar) const {
			return Point(X * scalar, Y * scalar);
		}

		void Log(std::string msg = "") {
			std::cout << msg << " (X Y) = (" << X << " " << Y << ")" << std::endl;
		}

};

#endif // !POINT_H


