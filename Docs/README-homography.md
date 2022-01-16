# Homography matrix calculation
## Point-to-Point homology
Considering two 2-D spaces J and K. The homological point in the two spaces is represented as  
__P__<sub>J</sub> = H __P__<sub>K</sub> ,  
where H is a 3 x 3 homography matrix, __P__<sub>J</sub> is a homogeneous coordinate in space-J and
__P__<sub>K</sub> is a homogeneous coordinate in space-K.  
_X_<sub>J</sub> =
_H_<sub>00</sub> _X_<sub>K</sub> + _H_<sub>01</sub> _Y_<sub>K</sub> + _H_<sub>02</sub> _Z_<sub>K</sub> ,  
_Y_<sub>J</sub> =
_H_<sub>10</sub> _X_<sub>K</sub> + _H_<sub>11</sub> _Y_<sub>K</sub> + _H_<sub>12</sub> _Z_<sub>K</sub> ,  
_Z_<sub>J</sub> =
_H_<sub>20</sub> _X_<sub>K</sub> + _H_<sub>21</sub> _Y_<sub>K</sub> + _H_<sub>22</sub> _Z_<sub>K</sub> .  
## Building simultaneous equation deriving homography matrix
Considering non-projection space coordinates in space-J,  
_x_<sub>J</sub> = _X_<sub>J</sub> / _Z_<sub>J</sub> =
( _H_<sub>00</sub> _X_<sub>K</sub> + _H_<sub>01</sub> _Y_<sub>K</sub> + _H_<sub>02</sub> _Z_<sub>K</sub> ) /
( _H_<sub>20</sub> _X_<sub>K</sub> + _H_<sub>21</sub> _Y_<sub>K</sub> + _H_<sub>22</sub> _Z_<sub>K</sub> ),  
_y_<sub>J</sub> = _Y_<sub>J</sub> / _Z_<sub>J</sub> =
( _H_<sub>10</sub> _X_<sub>K</sub> + _H_<sub>11</sub> _Y_<sub>K</sub> + _H_<sub>12</sub> _Z_<sub>K</sub> ) /
( _H_<sub>20</sub> _X_<sub>K</sub> + _H_<sub>21</sub> _Y_<sub>K</sub> + _H_<sub>22</sub> _Z_<sub>K</sub> ) .  
Linearlizing for _H_<sub>_mn_</sub> ,  
_x_<sub>J</sub> ( _H_<sub>20</sub> _X_<sub>K</sub> + _H_<sub>21</sub> _Y_<sub>K</sub> + _H_<sub>22</sub> _Z_<sub>K</sub> ) = _H_<sub>00</sub> _X_<sub>K</sub> + _H_<sub>01</sub> _Y_<sub>K</sub> + _H_<sub>02</sub> _Z_<sub>K</sub>
----- (1),  
_y_<sub>J</sub> ( _H_<sub>20</sub> _X_<sub>K</sub> + _H_<sub>21</sub> _Y_<sub>K</sub> + _H_<sub>22</sub> _Z_<sub>K</sub> ) = _H_<sub>10</sub> _X_<sub>K</sub> + _H_<sub>11</sub> _Y_<sub>K</sub> + _H_<sub>12</sub> _Z_<sub>K</sub>
----- (2) .  
Considering non-projection space coordinates in space-K ,  
_x_<sub>J</sub> ( _H_<sub>20</sub> _x_<sub>K</sub> + _H_<sub>21</sub> _y_<sub>K</sub> + _H_<sub>22</sub> ) = _H_<sub>00</sub> _x_<sub>K</sub> + _H_<sub>01</sub> _y_<sub>K</sub> + _H_<sub>02</sub> ,  
_y_<sub>J</sub> ( _H_<sub>20</sub> _x_<sub>K</sub> + _H_<sub>21</sub> _y_<sub>K</sub> + _H_<sub>22</sub> ) = _H_<sub>10</sub> _x_<sub>K</sub> + _H_<sub>11</sub> _y_<sub>K</sub> + _H_<sub>12</sub> ,  
where _x_<sub>K</sub> = _X_<sub>K</sub> / _Z_<sub>K</sub> , _y_<sub>K</sub> = _Y_<sub>K</sub> / _Z_<sub>K</sub> .  
Setting _H_<sub>22</sub> = 1 , and arranging the equationos for linear sum of _H_<sub>_mn_</sub> ,  
_H_<sub>00</sub> _x_<sub>K</sub> + _H_<sub>01</sub> _y_<sub>K</sub> + _H_<sub>02</sub> -
_H_<sub>20</sub> _x_<sub>J</sub> _x_<sub>K</sub> - _H_<sub>21</sub> _x_<sub>J</sub> _y_<sub>K</sub> =
_x_<sub>J</sub> ,  
_H_<sub>10</sub> _x_<sub>K</sub> + _H_<sub>11</sub> _y_<sub>K</sub> + _H_<sub>12</sub> -
_H_<sub>20</sub> _y_<sub>J</sub> _x_<sub>K</sub> - _H_<sub>21</sub> _y_<sub>J</sub> _y_<sub>K</sub> =
_y_<sub>J</sub> ,  
Writing in matrix formula,  
[  
    &nbsp; &nbsp;  _x_<sub>K</sub> , _y_<sub>K</sub> , 1 , 0 , 0 , 0 , -_x_<sub>J</sub> _x_<sub>K</sub> , -_x_<sub>J</sub> _y_<sub>K</sub> ;  
    &nbsp; &nbsp; 0 , 0 , 0 , _x_<sub>K</sub> , _y_<sub>K</sub> , 1 , -_y_<sub>J</sub> _x_<sub>K</sub> , -_y_<sub>J</sub> _y_<sub>K</sub> ;  
] [ _H_<sub>00</sub> , _H_<sub>01</sub> , _H_<sub>02</sub> , _H_<sub>10</sub> , _H_<sub>11</sub> , _H_<sub>12</sub> ,
_H_<sub>20</sub> , _H_<sub>21</sub> ]<sup>T</sup> = [ _x_<sub>J</sub> , _y_<sub>J</sub> ]<sup>T</sup>.  
Gathering four sets of homological pairs of points, the simultaneous equation is  
[  
    &nbsp; &nbsp;  _x_<sub>K</sub><sup>(0)</sup> , _y_<sub>K</sub><sup>(0)</sup> ,
    1 , 0 , 0 , 0 ,
    -_x_<sub>J</sub><sup>(0)</sup> _x_<sub>K</sub><sup>(0)</sup> ,
    -_x_<sub>J</sub><sup>(0)</sup> _y_<sub>K</sub><sup>(0)</sup> ;  
    &nbsp; &nbsp; 0 , 0 , 0 ,
    _x_<sub>K</sub><sup>(0)</sup> , _y_<sub>K</sub><sup>(0)</sup> , 1 ,
    -_y_<sub>J</sub><sup>(0)</sup> _x_<sub>K</sub><sup>(0)</sup> ,
    -_y_<sub>J</sub><sup>(0)</sup> _y_<sub>K</sub><sup>(0)</sup> ;  
    &nbsp; &nbsp;  _x_<sub>K</sub><sup>(1)</sup> , _y_<sub>K</sub><sup>(1)</sup> ,
    1 , 0 , 0 , 0 ,
    -_x_<sub>J</sub><sup>(1)</sup> _x_<sub>K</sub><sup>(1)</sup> ,
    -_x_<sub>J</sub><sup>(1)</sup> _y_<sub>K</sub><sup>(1)</sup> ;  
    &nbsp; &nbsp; 0 , 0 , 0 ,
    _x_<sub>K</sub><sup>(1)</sup> , _y_<sub>K</sub><sup>(1)</sup> , 1 ,
    -_y_<sub>J</sub><sup>(1)</sup> _x_<sub>K</sub><sup>(1)</sup> ,
    -_y_<sub>J</sub><sup>(1)</sup> _y_<sub>K</sub><sup>(1)</sup> ;  
    &nbsp; &nbsp;  _x_<sub>K</sub><sup>(2)</sup> , _y_<sub>K</sub><sup>(2)</sup> ,
    1 , 0 , 0 , 0 ,
    -_x_<sub>J</sub><sup>(2)</sup> _x_<sub>K</sub><sup>(2)</sup> ,
    -_x_<sub>J</sub><sup>(2)</sup> _y_<sub>K</sub><sup>(2)</sup> ;  
    &nbsp; &nbsp; 0 , 0 , 0 ,
    _x_<sub>K</sub><sup>(2)</sup> , _y_<sub>K</sub><sup>(2)</sup> , 1 ,
    -_y_<sub>J</sub><sup>(2)</sup> _x_<sub>K</sub><sup>(2)</sup> ,
    -_y_<sub>J</sub><sup>(2)</sup> _y_<sub>K</sub><sup>(2)</sup> ;  
    &nbsp; &nbsp;  _x_<sub>K</sub><sup>(3)</sup> , _y_<sub>K</sub><sup>(3)</sup> ,
    1 , 0 , 0 , 0 ,
    -_x_<sub>J</sub><sup>(3)</sup> _x_<sub>K</sub><sup>(3)</sup> ,
    -_x_<sub>J</sub><sup>(3)</sup> _y_<sub>K</sub><sup>(3)</sup> ;  
    &nbsp; &nbsp; 0 , 0 , 0 ,
    _x_<sub>K</sub><sup>(3)</sup> , _y_<sub>K</sub><sup>(3)</sup> , 1 ,
    -_y_<sub>J</sub><sup>(3)</sup> _x_<sub>K</sub><sup>(3)</sup> ,
    -_y_<sub>J</sub><sup>(3)</sup> _y_<sub>K</sub><sup>(3)</sup> ;  
] [ _H_<sub>00</sub> , _H_<sub>01</sub> , _H_<sub>02</sub> , _H_<sub>10</sub> , _H_<sub>11</sub> , _H_<sub>12</sub> ,
_H_<sub>20</sub> , _H_<sub>21</sub> ]<sup>T</sup> =
[ _x_<sub>J</sub><sup>(0)</sup> , _y_<sub>J</sub><sup>(0)</sup> ,
_x_<sub>J</sub><sup>(1)</sup> , _y_<sub>J</sub><sup>(1)</sup> ,
_x_<sub>J</sub><sup>(2)</sup> , _y_<sub>J</sub><sup>(2)</sup> ,
_x_<sub>J</sub><sup>(3)</sup> , _y_<sub>J</sub><sup>(3)</sup> ]<sup>T</sup>.  

## Application: Triangle - Triangle mapping
Deriving a homography matrix requires two sets of four 3D points in two planes. On the other hand, defining a plane
requires onlyy thtree points. Therefore in many cases using plane - plane mapping, only two sets of three points are
given. The condition is insufficient to derive a homography matrix. Solving the problem, the two triangles are
extended to two parallelograms. The function is defined in [`Pspc/homography.h, .c`](Pspc/homography.h).
A homography matrix defines a relationship between two 2-D projection spaces and _`z`_ cannot be zero.
It means that two 3-D regions of interest cannot contain the origin of the coordinate system.

## Making Homography Matrix Calculation Robuster by Generic Non-linear Solver
In (1), (2), preserving _Z_<sub>K</sub> and _H_<sub>22</sub> ,  
_H_<sub>00</sub> _X_<sub>K</sub> + _H_<sub>01</sub> _Y_<sub>K</sub> + _H_<sub>02</sub> _Z_<sub>K</sub> -
_H_<sub>20</sub> _x_<sub>J</sub> _X_<sub>K</sub> - _H_<sub>21</sub> _x_<sub>J</sub> _Y_<sub>K</sub> -
_H_<sub>22</sub> _x_<sub>J</sub> _Z_<sub>K</sub> = 0 ,  
_H_<sub>10</sub> _X_<sub>K</sub> + _H_<sub>11</sub> _Y_<sub>K</sub> + _H_<sub>12</sub> _Z_<sub>K</sub> -
_H_<sub>20</sub> _y_<sub>J</sub> _X_<sub>K</sub> - _H_<sub>21</sub> _y_<sub>J</sub> _Y_<sub>K</sub> -
_H_<sub>22</sub> _y_<sub>J</sub> _Z_<sub>K</sub> = 0 ,  
Writing a matrix formula,  
[  
&nbsp; &nbsp; _X_<sub>K</sub> , _Y_<sub>K</sub> , _Z_<sub>K</sub> , 0 , 0 , 0
, -_x_<sub>J</sub> _X_<sub>K</sub> , -_x_<sub>J</sub> _Y_<sub>K</sub> , -_x_<sub>J</sub> _Z_<sub>K</sub> ;  
&nbsp; &nbsp;  0 , 0 , 0 , _X_<sub>K</sub> , _Y_<sub>K</sub> , _Z_<sub>K</sub>
, -_y_<sub>J</sub> _X_<sub>K</sub> , -_y_<sub>J</sub> _Y_<sub>K</sub> , -_y_<sub>J</sub> _Z_<sub>K</sub> ;  
] [ _H_<sub>00</sub> , _H_<sub>01</sub> , _H_<sub>02</sub> ,
 _H_<sub>10</sub> , _H_<sub>11</sub> , _H_<sub>12</sub> ,
 _H_<sub>20</sub> , _H_<sub>21</sub> , _H_<sub>22</sub> ,
 ]<sup>T</sup> = [ 0, 0, 0, 0, 0, 0, 0, 0, 0 ]<sup>T</sup>  
Gatherinig four sets of homological pair of points, the simultaneous equation is  
[  
&nbsp; &nbsp; _X_<sub>K</sub><sup>(0)</sup> , _Y_<sub>K</sub><sup>(0)</sup> , _Z_<sub>K</sub><sup>(0)</sup>
, 0 , 0 , 0
, -_x_<sub>J</sub><sup>(0)</sup> _X_<sub>K</sub><sup>(0)</sup>
, -_x_<sub>J</sub><sup>(0)</sup> _Y_<sub>K</sub><sup>(0)</sup>
, -_x_<sub>J</sub><sup>(0)</sup> _Z_<sub>K</sub><sup>(0)</sup> ;  
&nbsp; &nbsp;  0 , 0 , 0
, _X_<sub>K</sub><sup>(0)</sup> , _Y_<sub>K</sub><sup>(0)</sup> , _Z_<sub>K</sub><sup>(0)</sup>
, -_y_<sub>J</sub><sup>(0)</sup> _X_<sub>K</sub><sup>(0)</sup>
, -_y_<sub>J</sub><sup>(0)</sup> _Y_<sub>K</sub><sup>(0)</sup>
, -_y_<sub>J</sub><sup>(0)</sup> _Z_<sub>K</sub><sup>(0)</sup> ;  
&nbsp; &nbsp; _X_<sub>K</sub><sup>(1)</sup> , _Y_<sub>K</sub><sup>(1)</sup> , _Z_<sub>K</sub><sup>(1)</sup>
, 0 , 0 , 0
, -_x_<sub>J</sub><sup>(1)</sup> _X_<sub>K</sub><sup>(1)</sup>
, -_x_<sub>J</sub><sup>(1)</sup> _Y_<sub>K</sub><sup>(1)</sup>
, -_x_<sub>J</sub><sup>(1)</sup> _Z_<sub>K</sub><sup>(1)</sup> ;  
&nbsp; &nbsp;  0 , 0 , 0
, _X_<sub>K</sub><sup>(1)</sup> , _Y_<sub>K</sub><sup>(1)</sup> , _Z_<sub>K</sub><sup>(1)</sup>
, -_y_<sub>J</sub><sup>(1)</sup> _X_<sub>K</sub><sup>(1)</sup>
, -_y_<sub>J</sub><sup>(1)</sup> _Y_<sub>K</sub><sup>(1)</sup>
, -_y_<sub>J</sub><sup>(1)</sup> _Z_<sub>K</sub><sup>(1)</sup> ;  
&nbsp; &nbsp; _X_<sub>K</sub><sup>(2)</sup> , _Y_<sub>K</sub><sup>(2)</sup> , _Z_<sub>K</sub><sup>(2)</sup>
, 0 , 0 , 0
, -_x_<sub>J</sub><sup>(2)</sup> _X_<sub>K</sub><sup>(2)</sup>
, -_x_<sub>J</sub><sup>(2)</sup> _Y_<sub>K</sub><sup>(2)</sup>
, -_x_<sub>J</sub><sup>(2)</sup> _Z_<sub>K</sub><sup>(2)</sup> ;  
&nbsp; &nbsp;  0 , 0 , 0
, _X_<sub>K</sub><sup>(2)</sup> , _Y_<sub>K</sub><sup>(2)</sup> , _Z_<sub>K</sub><sup>(2)</sup>
, -_y_<sub>J</sub><sup>(2)</sup> _X_<sub>K</sub><sup>(2)</sup>
, -_y_<sub>J</sub><sup>(2)</sup> _Y_<sub>K</sub><sup>(2)</sup>
, -_y_<sub>J</sub><sup>(2)</sup> _Z_<sub>K</sub><sup>(2)</sup> ;  
&nbsp; &nbsp; _X_<sub>K</sub><sup>(3)</sup> , _Y_<sub>K</sub><sup>(3)</sup> , _Z_<sub>K</sub><sup>(3)</sup>
, 0 , 0 , 0
, -_x_<sub>J</sub><sup>(3)</sup> _X_<sub>K</sub><sup>(3)</sup>
, -_x_<sub>J</sub><sup>(3)</sup> _Y_<sub>K</sub><sup>(3)</sup>
, -_x_<sub>J</sub><sup>(3)</sup> _Z_<sub>K</sub><sup>(3)</sup> ;  
&nbsp; &nbsp;  0 , 0 , 0
, _X_<sub>K</sub><sup>(3)</sup> , _Y_<sub>K</sub><sup>(3)</sup> , _Z_<sub>K</sub><sup>(3)</sup>
, -_y_<sub>J</sub><sup>(3)</sup> _X_<sub>K</sub><sup>(3)</sup>
, -_y_<sub>J</sub><sup>(3)</sup> _Y_<sub>K</sub><sup>(3)</sup>
, -_y_<sub>J</sub><sup>(3)</sup> _Z_<sub>K</sub><sup>(3)</sup> ;  
] [ _H_<sub>00</sub> , _H_<sub>01</sub> , _H_<sub>02</sub> ,
 _H_<sub>10</sub> , _H_<sub>11</sub> , _H_<sub>12</sub> ,
 _H_<sub>20</sub> , _H_<sub>21</sub> , _H_<sub>22</sub> ,
 ]<sup>T</sup> = [ 0, 0, 0, 0, 0, 0, 0, 0, 0 ]<sup>T</sup>  
The simultaneous equation has insufficient constraints, and another constraint is added as below.  
[ _H_<sub>00</sub> , _H_<sub>01</sub> , _H_<sub>02</sub> ,
 _H_<sub>10</sub> , _H_<sub>11</sub> , _H_<sub>12</sub> ,
 _H_<sub>20</sub> , _H_<sub>21</sub> , _H_<sub>22</sub> ,
 ] [
_H_<sub>00</sub> , _H_<sub>01</sub> , _H_<sub>02</sub> ,
 _H_<sub>10</sub> , _H_<sub>11</sub> , _H_<sub>12</sub> ,
 _H_<sub>20</sub> , _H_<sub>21</sub> , _H_<sub>22</sub> ,
 ]<sup>T</sup> - 1.0 = 0.0 .  

Writing in simplified matrix formula as below,  
M __H__ = __0__, ----- (3)  
< __H__ , __H__ > = 0 ----- (4) .  

## Jacobian Calculation
Jacobian of the matrix part (3) is M.
Jacobian's last row is the partial derivative of (4), which is 2 __H__ .  
The Jacobian is
J = [  
&nbsp; &nbsp; M ;  
&nbsp; &nbsp; 2 __H__ ;  
] . ----- (5)  

## 3-point to 3-point mapping in 3-D real space
consider three points in source space as __p__<sub>K</sub><sup>(0)</sup>, __p__<sub>K</sub><sup>(1)</sup>
, __p__<sub>K</sub><sup>(2)</sup> and three points in destination space as
__p__<sub>J</sub><sup>(0)</sup>, __p__<sub>J</sub><sup>(1)</sup>, __p__<sub>J</sub><sup>(2)</sup> .  
__p__<sub>J</sub> = M __p__<sub>K</sub> ,  
[ _x_<sub>J</sub> , _y_<sub>J</sub> , _z_<sub>J</sub> ]<sup>T</sup> =
[  
&nbsp; &nbsp; _m_<sub>00</sub> , _m_<sub>01</sub> , _m_<sub>02</sub> ;  
&nbsp; &nbsp; _m_<sub>10</sub> , _m_<sub>11</sub> , _m_<sub>12</sub> ;  
&nbsp; &nbsp; _m_<sub>20</sub> , _m_<sub>21</sub> , _m_<sub>22</sub> ;  
] [ _x_<sub>K</sub> , _y_<sub>K</sub> , _z_<sub>K</sub> ]<sup>T</sup> =  
[  
&nbsp; &nbsp; _m_<sub>00</sub> _x_<sub>K</sub> + _m_<sub>01</sub> _y_<sub>K</sub> + _m_<sub>02</sub> _z_<sub>K</sub> ;  
&nbsp; &nbsp; _m_<sub>10</sub> _x_<sub>K</sub> + _m_<sub>11</sub> _y_<sub>K</sub> + _m_<sub>12</sub> _z_<sub>K</sub> ;  
&nbsp; &nbsp; _m_<sub>20</sub> _x_<sub>K</sub> + _m_<sub>21</sub> _y_<sub>K</sub> + _m_<sub>22</sub> _z_<sub>K</sub> ;  
] = [  
&nbsp; &nbsp; _x_<sub>K</sub> , _y_<sub>K</sub> , z<sub>K</sub> , 0 , 0 , 0 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , _x_<sub>K</sub> , _y_<sub>K</sub> , z<sub>K</sub> 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , 0 , 0 , 0 , _x_<sub>K</sub> , _y_<sub>K</sub> , z<sub>K</sub> ;  
] [ _m_<sub>00</sub> , _m_<sub>01</sub> , _m_<sub>02</sub> ,
 _m_<sub>10</sub> , _m_<sub>11</sub> , _m_<sub>12</sub> ,
  _m_<sub>20</sub> , _m_<sub>21</sub> , _m_<sub>22</sub> ]<sup>T</sup>  
<br>
Using homologies of three points,  
[ _x_<sub>J</sub><sup>(0)</sup>, _y_<sub>J</sub><sup>(0)</sup>, _z_<sub>J</sub><sup>(0)</sup>
, _x_<sub>J</sub><sup>(1)</sup>, _y_<sub>J</sub><sup>(1)</sup>, _z_<sub>J</sub><sup>(1)</sup>
, _x_<sub>J</sub><sup>(2)</sup>, _y_<sub>J</sub><sup>(2)</sup>, _z_<sub>J</sub><sup>(2)</sup> ]<sup>T</sup> =  
[  
&nbsp; &nbsp; _x_<sub>K</sub><sup>(0)</sup> , _y_<sub>K</sub><sup>(0)</sup> , z<sub>K</sub><sup>(0)</sup> , 0 , 0 , 0 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , _x_<sub>K</sub><sup>(0)</sup> , _y_<sub>K</sub><sup>(0)</sup> , z<sub>K</sub><sup>(0)</sup> , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , 0 , 0 , 0 , _x_<sub>K</sub><sup>(0)</sup> , _y_<sub>K</sub><sup>(0)</sup> , z<sub>K</sub><sup>(0)</sup> ;  
&nbsp; &nbsp; _x_<sub>K</sub><sup>(1)</sup> , _y_<sub>K</sub><sup>(1)</sup> , z<sub>K</sub><sup>(1)</sup> , 0 , 0 , 0 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , _x_<sub>K</sub><sup>(1)</sup> , _y_<sub>K</sub><sup>(1)</sup> , z<sub>K</sub><sup>(1)</sup> , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , 0 , 0 , 0 , _x_<sub>K</sub><sup>(1)</sup> , _y_<sub>K</sub><sup>(1)</sup> , z<sub>K</sub><sup>(1)</sup> ;  
&nbsp; &nbsp; _x_<sub>K</sub><sup>(2)</sup> , _y_<sub>K</sub><sup>(2)</sup> , z<sub>K</sub><sup>(2)</sup> , 0 , 0 , 0 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , _x_<sub>K</sub><sup>(2)</sup> , _y_<sub>K</sub><sup>(2)</sup> , z<sub>K</sub><sup>(2)</sup> , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 0 , 0 , 0 , 0 , _x_<sub>K</sub><sup>(2)</sup> , _y_<sub>K</sub><sup>(2)</sup> , z<sub>K</sub><sup>(2)</sup> ;  
]  [ _m_<sub>00</sub> , _m_<sub>01</sub> , _m_<sub>02</sub> ,
 _m_<sub>10</sub> , _m_<sub>11</sub> , _m_<sub>12</sub> ,
  _m_<sub>20</sub> , _m_<sub>21</sub> , _m_<sub>22</sub> ]<sup>T</sup> .  
__v__<sub>K</sub> = M<sub>K</sub> __m__ .  
__m__ = M<sub>K</sub><sup>-1</sup> __v__<sub>K</sub> .  