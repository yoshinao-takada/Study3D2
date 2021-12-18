# Geometry problems
## Problem 1: Draw a normal line to a reference line (P0,D) from a point P1
The reference line is defined by a reference point __P__<sub>0</sub> and a direction vector __D__ .  
&nbsp; &nbsp; __P__ = __P__<sub>0</sub> + _a_ __D__ , &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; (1)  
Vector __P__ - __P__<sub>1</sub> is orthogonal to __D__ .  
&nbsp; &nbsp; &lt;__P__ - __P__<sub>1</sub> , __D__ &gt; = 0 , &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; (2)  
Replace __P__ with RHS of (1),  
&nbsp; &nbsp; &lt;__P__<sub>0</sub> + _a_ __D__ - __P__<sub>1</sub> , __D__ &gt; = 0 ,  
&nbsp; &nbsp; &lt;__P__<sub>0</sub> , __D__ &gt; + _a_ || __D__ ||<sup>2</sup> - &lt; __P__<sub>1</sub> , __D__ &gt; = 0,  
&nbsp; &nbsp; _a_ = ( 1 / || __D__ ||<sup>2</sup>) &lt; __P__<sub>1</sub> - __P__<sub>0</sub> , __D__ &gt; .  
Once _a_ is derived, __P__ satisfying the two line orthgonal condition is also derived by (1).  
The orthgonal line is represented by (__P__<sub>0</sub> + _a_ __D__ , __P__<sub>1</sub> - ( __P__<sub>0</sub> + _a_ __D__ ) ) ,
where  
__P__<sub>0</sub> + _a_ __D__ : line reference point  
__P__<sub>1</sub> - ( __P__<sub>0</sub> + _a_ __D__ ) : direction vector  

## Problem 2: Draw a normal line to a reference plane (P0, N) from a point P1
The reference plane is defined by a reference point __P__<sub>0</sub> and a normal vector __N__ .  
&nbsp; &nbsp; &lt; __P__ - __P__<sub>0</sub> , __N__ &gt; = 0 ,
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  (1)  
where __P__ is a point in the plane.  
&nbsp; &nbsp; __P__ + _&alpha;_ __N__ = __P__<sub>1</sub> ,  
&nbsp; &nbsp; __P__ = __P__<sub>1</sub> - _&alpha;_ __N__,
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  (2)  

and _&alpha;_ exists. (1) is modified as  
&nbsp; &nbsp; &lt; __P__<sub>1</sub> - _&alpha;_ __N__ - __P__<sub>0</sub> , __N__ &gt; = 0 ,  
&nbsp; &nbsp; &lt; __P__<sub>1</sub> , __N__ &gt; - _&alpha;_ - &lt; __P__<sub>0</sub> , __N__ &gt; = 0 ,  
&nbsp; &nbsp; _&alpha;_ = &lt; __P__<sub>1</sub> , __N__ &gt; - &lt; __P__<sub>0</sub> , __N__ &gt;
= &lt; __P__<sub>1</sub> - __P__<sub>0</sub> , __N__ &gt; .
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  (3)  
Put (3) into (2),  
&nbsp; &nbsp; __P__ = __P__<sub>1</sub> - &lt; __P__<sub>1</sub> - __P__<sub>0</sub> , __N__ &gt; __N__,
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  (4) is an intersection.  

## Problem 3: Intersection of a line and a plane
The line is defined by a reference point __P__<sub>0</sub> and a direction vector __D__ .  
&nbsp; &nbsp; __P__ = __P__<sub>0</sub> + _a_ __D__ , &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; (1)  
The plane is defiend by a reference point __P__<sub>1</sub> and a normal vector __N__ .  
&nbsp; &nbsp; &lt; __P__ - __P__<sub>1</sub> , __N__ &gt; = 0 , &nbsp; &nbsp; (2)  

&nbsp; &nbsp; &lt; __P__<sub>0</sub> + _a_ __D__ - __P__<sub>1</sub> , __N__ &gt; = 0 ,  
&nbsp; &nbsp; &lt; __P__<sub>0</sub> - __P__<sub>1</sub> , __N__ &gt; = - _&alpha;_ &lt; __D__ , __N__ &gt; ,  
&nbsp; &nbsp; _&alpha;_ = &lt; __P__<sub>1</sub> - __P__<sub>0</sub> , __N__ &gt; /
&lt; __D__, __N__ &gt; , &nbsp; &nbsp; (3)  
Put _&alpha;_ from (3) into (1) ,  
__P__ = __P__<sub>0</sub> + 
( &lt; __P__<sub>1</sub> - __P__<sub>0</sub> , __N__ &gt; /
&lt; __D__, __N__ &gt; ) __D__; &nbsp; &nbsp; &nbsp; &nbsp; (4) ... result  

## Problem 4: Intersection of a line and a triangle
The line is defined by a reference point __P__<sub>0</sub> and a direction vector __D__ .  
&nbsp; &nbsp; __P__ = __P__<sub>0</sub> + _a_ __D__ , &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; (1)  
The triangle is defined by three vertices, __P__<sub>T 0</sub> , __P__<sub>T 1</sub> , __P__<sub>T 2</sub> .  
The plane which the triangle belongs to is  
&nbsp; &nbsp; &lt; __P__ - __P__<sub>T 0</sub> , __N__ &gt; = 0 , where
__N__ = (__P__<sub>T 1</sub> - __P__<sub>T 0</sub>) x (__P__<sub>T 2</sub> - __P__<sub>T 0</sub>) .  
Using the result of Problem 3, __P__ is obtained.  
And then test whether __P__ is in the triangle.
Get the three vectors,  
&nbsp; &nbsp; __V__<sub>0</sub> = __P__<sub>T 0</sub> - __P__ ,  
&nbsp; &nbsp; __V__<sub>1</sub> = __P__<sub>T 1</sub> - __P__ ,  
&nbsp; &nbsp; __V__<sub>2</sub> = __P__<sub>T 2</sub> - __P__ ,  
and then get the three cross products,  
&nbsp; &nbsp; __V__<sub>X01</sub> = __V__<sub>0</sub> x __V__<sub>1</sub> ,  
&nbsp; &nbsp; __V__<sub>X12</sub> = __V__<sub>1</sub> x __V__<sub>2</sub> ,  
&nbsp; &nbsp; __V__<sub>X20</sub> = __V__<sub>2</sub> x __V__<sub>0</sub> ,  
The directions of the three cross products are the same if __P__ is in the triangle. Otherwise, one of the three
has an opposing direction.  
&nbsp; &nbsp; Sign(
    &lt; __V__<sub>X01</sub> , __V__<sub>X12</sub> &gt;
    &lt; __V__<sub>X12</sub> , __V__<sub>X20</sub> &gt;
    &lt; __V__<sub>X20</sub> , __V__<sub>X01</sub> &gt;
 ) > 0 ==> __P__ is in the triangle.  
 