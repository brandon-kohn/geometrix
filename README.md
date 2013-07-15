Geometrix
=========

Geometrix is a computational geometry library developed with the primary goal of allowing developers to express geometric and algebraic constructs in the same language as they are expressed in textbooks. For example, a common geometry problem is to find the intersection point between two intersecting line segments. This can be algebraically expressed as: 

Given points A, B, C, D which define properly intersecting line segments AB, CD the intersection point I is defined as:

I = A + s *(B-A), where s is a scalar parameter whose value represents a scale factor to the vector B-A which designates where the intersection occurs.

Many geometry libraries require simple constructs like this to be made explicitly by manipulating the elements of the points and vectors involved in the computation.

e.g.: 

I.x = A.x + s * (B.x-A.x)
I.y = A.y + s * (B.y-A.y)

With geometrix the calculation can be expressed as:

point<double,2> I = A + s * (A-B);

This simpler form is true to the original intention and is optimised through the use of expression templates.

