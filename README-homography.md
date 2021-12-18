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
_x_<sub>J</sub> ( _H_<sub>20</sub> _X_<sub>K</sub> + _H_<sub>21</sub> _Y_<sub>K</sub> + _H_<sub>22</sub> _Z_<sub>K</sub> ) = _H_<sub>00</sub> _X_<sub>K</sub> + _H_<sub>01</sub> _Y_<sub>K</sub> + _H_<sub>02</sub> _Z_<sub>K</sub> ,  
_y_<sub>J</sub> ( _H_<sub>20</sub> _X_<sub>K</sub> + _H_<sub>21</sub> _Y_<sub>K</sub> + _H_<sub>22</sub> _Z_<sub>K</sub> ) = _H_<sub>10</sub> _X_<sub>K</sub> + _H_<sub>11</sub> _Y_<sub>K</sub> + _H_<sub>12</sub> _Z_<sub>K</sub> .  
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