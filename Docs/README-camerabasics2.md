# Camera matrix
## Variable naming convention
Bold : vector  
Italic : scalar  
Non-bold, Non-italic : matrix  
Upper case : in homogeneous coordinate  
Lower case : in real coordinate  
subscript  
character | description
--|--
C | in camera coordinate
P | in unit projection coordinate
R | in viewport coordinate
W | in world coordinate
<br>

superscript  
character | description
--|--
D | delta, step, small change
E | end, edge
C | center
O | origin
<br>

__P__ = [ _X_ , _Y_ , _Z_ ]<sup>T</sup> : point in 2D homogeneous coordinate  
__P__ = [ _X_ , _Y_ , _Z_ , _W_ ]<sup>T</sup> : point in 3D homogeneous coordinate  
__p__ = [ _x_, _y_ ]<sup>T</sup> : point in 2D real coordinate  
__p__ = [ _x_, _y_ , _z_ ]<sup>T</sup> : point in 3D real coordinate

## Projection matrix from camera coordinate to viewport coordinate
__P__<sub>C</sub> , homogeneous camera coordinate is transformed to __P__<sub>R</sub> by  
__P__<sub>R</sub> = M<sub>I</sub> M<sub>0</sub>  __P__<sub>C</sub> ----- (1)  
, where M<sub>I</sub> is a camera intrinsic matrix and M<sub>0</sub> is the dimension reduction matrix,  
M<sub>I</sub> = [  
&nbsp; &nbsp; _m_<sub>I00</sub> , _m_<sub>I01</sub> , _m_<sub>I02</sub> ;  
&nbsp; &nbsp; _m_<sub>I10</sub> , _m_<sub>I11</sub> , _m_<sub>I12</sub> ;  
&nbsp; &nbsp; _m_<sub>I20</sub> , _m_<sub>I21</sub> , 1 ;  
] ,  
M<sub>0</sub> = [  
&nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 1 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
] .  
M<sub>I</sub> is defined as an upper triangle matrix in many textbooks but I think that
M<sub>I</sub> is a homography matrix mapping a genera four corners in 3D space to another four corners
which can be projected to a rectangle in a rendering plane.  
The real coordinate of __P__<sub>R</sub> is  
__p__<sub>R</sub> = [ _x_<sub>R</sub> , _y_<sub>R</sub> ]<sup>T</sup> = 
[ _X_<sub>R</sub> / _Z_<sub>R</sub> , _Y_<sub>R</sub> / _Z_<sub>R</sub>]<sup>T</sup>  
<br>
<!--
  relationship among viewport parameters and the canonical projection plane coords
-->
In many non-metrological cases, M<sub>I</sub> can be approximated as an upper triangle matrix as  
M<sub>I</sub> = [  
&nbsp; &nbsp; _&alpha;_<sub>0</sub> , 0 , _&beta;_<sub>0</sub> ;  
&nbsp; &nbsp; 0 , _&alpha;_<sub>1</sub> , _&beta;_<sub>1</sub> ;  
&nbsp; &nbsp; 0 , 0 , 1 ;  
]  
with four unknown parameters, _&alpha;_<sub>0</sub>, _&alpha;_<sub>1</sub> , &beta;<sub>0</sub> , &beta;<sub>1</sub> .
Derivation process of the parameters are described below.  
![Fig1](Images/Fig1.jpg)  
Figure 1. Camera parameter description<br>  
M = M<sub>I</sub> M<sub>0</sub> = [  
&nbsp; &nbsp; _&alpha;_<sub>0</sub> , 0 , _&beta;_<sub>0</sub> , 0 ;  
&nbsp; &nbsp; 0 , _&alpha;_<sub>1</sub> , _&beta;_<sub>1</sub> , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
]  
__P__<sub>R</sub><sup>TR</sup> = M __P__<sub>C</sub><sup>TR</sup> ,  
__P__<sub>R</sub><sup>BL</sup> = M __P__<sub>C</sub><sup>BL</sup> ,  
where superscript `TR`, `BL` mean top-right and bottom-left.  
__P__<sub>R</sub><sup>TR</sup> = [  
&nbsp; &nbsp; _&alpha;_<sub>0</sub> , 0 , _&beta;_<sub>0</sub> , 0 ;  
&nbsp; &nbsp; 0 , _&alpha;_<sub>1</sub> , _&beta;_<sub>1</sub> , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
] [ _X_<sub>C</sub><sup>TR</sup> , _Y_<sub>C</sub><sup>TR</sup> , _Z_<sub>C</sub><sup>TR</sup> ]<sup>T</sup>  
= [ _&alpha; X_<sub>C</sub><sup>TR</sup> + _&beta;_<sub>0</sub> _Z_<sub>C</sub><sup>TR</sup> ,
_&alpha; Y_<sub>C</sub><sup>TR</sup> + _&beta;_<sub>1</sub> _Z_<sub>C</sub><sup>TR</sup> ,
_Z_<sub>C</sub><sup>TR</sup> ]  
Converting a projection coordinate __P__<sub>R</sub><sup>TR</sup> to __p__<sub>R</sub><sup>TR</sup> ,  
__p__<sub>R</sub><sup>TR</sup> = [ _W_, 0 ] =  
[
  (_&alpha;<sub>0</sub> X_<sub>C</sub><sup>TR</sup> + _&beta;_<sub>0</sub> _Z_<sub>C</sub><sup>TR</sup>) /
  _Z_<sub>C</sub><sup>TR</sup> ,
  (_&alpha;<sub>1</sub> Y_<sub>C</sub><sup>TR</sup> + _&beta;_<sub>1</sub> _Z_<sub>C</sub><sup>TR</sup>) /
  _Z_<sub>C</sub><sup>TR</sup>
]  
Similarily,  
__p__<sub>R</sub><sup>BL</sup> = [ 0, _H_ ] =  
[
  (_&alpha;<sub>0</sub> X_<sub>C</sub><sup>BL</sup> + _&beta;_<sub>0</sub> _Z_<sub>C</sub><sup>BL</sup>) /
  _Z_<sub>C</sub><sup>BL</sup> ,
  (_&alpha;<sub>1</sub> Y_<sub>C</sub><sup>BL</sup> + _&beta;_<sub>1</sub> _Z_<sub>C</sub><sup>BL</sup>) /
  _Z_<sub>C</sub><sup>BL</sup>
]  
Writing in a matrix equation,  
[ _W_ , 0 , 0 , _H_ ]<sup>T</sup> = [  
&nbsp; &nbsp; _X_<sub>C</sub><sup>TR</sup> / _Z_<sub>C</sub><sup>TR</sup> , 0 , 1 , 0 ;  
&nbsp; &nbsp; 0 , _Y_<sub>C</sub><sup>TR</sup> / _Z_<sub>C</sub><sup>TR</sup> , 0 , 1 ;  
&nbsp; &nbsp; _X_<sub>C</sub><sup>BL</sup> / _Z_<sub>C</sub><sup>BL</sup> , 0 , 1 , 0 ;  
&nbsp; &nbsp; 0 , _Y_<sub>C</sub><sup>BL</sup> / _Z_<sub>C</sub><sup>BL</sup> , 0 , 1 ;  
] [ _&alpha;_<sub>0</sub> , _&alpha;_<sub>1</sub> , _&beta;_<sub>0</sub> , _&beta;_<sub>1</sub> ]<sup>T</sup> ,  
where  
_X_<sub>C</sub><sup>TR</sup> = _W D_<sub>35</sub> / (2 _D_) ,  
_Y_<sub>C</sub><sup>TR</sup> = _H D_<sub>35</sub> / (2 _D_) ,  
_X_<sub>C</sub><sup>BL</sup> = -_W D_<sub>35</sub> / (2 _D_) ,  
_Y_<sub>C</sub><sup>BL</sup> = -_H D_<sub>35</sub> / (2 _D_) ,  
_Z_<sub>C</sub><sup>TR</sup> = _Z_<sub>C</sub><sup>BL</sup> = -_F_<sub>35</sub> .  
That is the derivation of camera intrinsic matrix approximated with 35 mm equivalent focal length.  

## Camera Extrinsic Matrix
It is also called Camera Position Matrix. It is an inverse of a transform matrix moving a camera from the origin
to an actual camera origin. Decomposing the transform matrix, camera position movement is intuitively understandable.  
In the original position, the camera optical axis is aligned to Z axis negative direction. The camera upward direction
is aligned to Y-axis positive direction. The camera origin is at the world origin.  
Decomposed movements are  
1. rotation about optical axis,
2. rotation about X-axis (elevation angle),
3. rotation about Y-axis (azimuth angle),
4. linear movement.

## Camera Viewline
It is needed to calculate viewlines in world coordinates from viewport pixel coordinate.
The process is described below.
A projection matrix M is defined as  
M = [  
&nbsp; &nbsp; _m_<sub>00</sub> ,  _m_<sub>01</sub> ,  _m_<sub>02</sub> ,  _m_<sub>03</sub> ;  
&nbsp; &nbsp; _m_<sub>10</sub> ,  _m_<sub>11</sub> ,  _m_<sub>12</sub> ,  _m_<sub>13</sub> ;  
&nbsp; &nbsp; _m_<sub>20</sub> ,  _m_<sub>21</sub> ,  _m_<sub>22</sub> ,  _m_<sub>23</sub> ;  
]  

__P__<sub>R</sub> = M __P__<sub>W</sub>  
[ _X_<sub>R</sub> , _Y_<sub>R</sub> , _Z_<sub>R</sub> ]<sup>T</sup> = [  
&nbsp; &nbsp; _m_<sub>00</sub> ,  _m_<sub>01</sub> ,  _m_<sub>02</sub> ,  _m_<sub>03</sub> ;  
&nbsp; &nbsp; _m_<sub>10</sub> ,  _m_<sub>11</sub> ,  _m_<sub>12</sub> ,  _m_<sub>13</sub> ;  
&nbsp; &nbsp; _m_<sub>20</sub> ,  _m_<sub>21</sub> ,  _m_<sub>22</sub> ,  _m_<sub>23</sub> ;  
] [ _x_<sub>W</sub> , _y_<sub>W</sub> , _z_<sub>W</sub> , 1 ]<sup>T</sup>  
_X_<sub>R</sub> = _m_<sub>00</sub> _x_<sub>W</sub> + _m_<sub>01</sub> _y_<sub>W</sub> +
_m_<sub>02</sub> _z_<sub>W</sub> + _m_<sub>03</sub> ,  
_Y_<sub>R</sub> = _m_<sub>10</sub> _x_<sub>W</sub> + _m_<sub>11</sub> _y_<sub>W</sub> +
_m_<sub>12</sub> _z_<sub>W</sub> + _m_<sub>03</sub> ,  
_Z_<sub>R</sub> = _m_<sub>20</sub> _x_<sub>W</sub> + _m_<sub>21</sub> _y_<sub>W</sub> +
_m_<sub>22</sub> _z_<sub>W</sub> + _m_<sub>23</sub> ,  
Converting 2D projection coordinate __P__<sub>R</sub> to real coordinate __p__<sub>R</sub>,  
_x_<sub>R</sub> = _X_<sub>R</sub> / _Z_<sub>R</sub> =
(_m_<sub>00</sub> _x_<sub>W</sub> + _m_<sub>01</sub> _y_<sub>W</sub> + _m_<sub>02</sub> _z_<sub>W</sub> + _m_<sub>03</sub>) /
(_m_<sub>20</sub> _x_<sub>W</sub> + _m_<sub>21</sub> _y_<sub>W</sub> + _m_<sub>22</sub> _z_<sub>W</sub> + _m_<sub>23</sub>) ,  
_y_<sub>R</sub> = _Y_<sub>R</sub> / _Z_<sub>R</sub> =
(_m_<sub>10</sub> _x_<sub>W</sub> + _m_<sub>11</sub> _y_<sub>W</sub> + _m_<sub>12</sub> _z_<sub>W</sub> + _m_<sub>13</sub>) /
(_m_<sub>20</sub> _x_<sub>W</sub> + _m_<sub>21</sub> _y_<sub>W</sub> + _m_<sub>22</sub> _z_<sub>W</sub> + _m_<sub>23</sub>) ,  
Linearizing these equations, two equations defining two planes are obtained as follows,  
_x_<sub>R</sub> (_m_<sub>20</sub> _x_<sub>W</sub> + _m_<sub>21</sub> _y_<sub>W</sub> + _m_<sub>22</sub> _z_<sub>W</sub> + _m_<sub>23</sub>) =
_m_<sub>00</sub> _x_<sub>W</sub> + _m_<sub>01</sub> _y_<sub>W</sub> + _m_<sub>02</sub> _z_<sub>W</sub> + _m_<sub>03</sub> ,  
_y_<sub>R</sub> (_m_<sub>20</sub> _x_<sub>W</sub> + _m_<sub>21</sub> _y_<sub>W</sub> + _m_<sub>22</sub> _z_<sub>W</sub> + _m_<sub>23</sub>) =
_m_<sub>10</sub> _x_<sub>W</sub> + _m_<sub>11</sub> _y_<sub>W</sub> + _m_<sub>12</sub> _z_<sub>W</sub> + _m_<sub>13</sub> ,  
_x_<sub>W</sub> ( _x_<sub>R</sub> _m_<sub>20</sub> - _m_<sub>00</sub> ) +
_y_<sub>W</sub> ( _x_<sub>R</sub> _m_<sub>21</sub> - _m_<sub>01</sub> ) +
_z_<sub>W</sub> ( _x_<sub>R</sub> _m_<sub>22</sub> - _m_<sub>02</sub> ) +
_x_<sub>R</sub> _m_<sub>23</sub> - _m_<sub>03</sub> = 0 ----- plane0 ,  
_x_<sub>W</sub> ( _y_<sub>R</sub> _m_<sub>20</sub> - _m_<sub>10</sub> ) +
_y_<sub>W</sub> ( _y_<sub>R</sub> _m_<sub>21</sub> - _m_<sub>11</sub> ) +
_z_<sub>W</sub> ( _y_<sub>R</sub> _m_<sub>22</sub> - _m_<sub>12</sub> ) +
_y_<sub>R</sub> _m_<sub>23</sub> - _m_<sub>13</sub> = 0 ----- plane1 ,  
The intersection of plane0 and plane1 is the viewline of the camera.
The camera origin must be on the viewline. Therefore the reference point of the viewline is
the camera origin. The direction vector shall be the crossproduct of the normal vectors of the planes or
its negated. The dot product of the direction vector and a vector from the camera origin to the look-at
point shall be positive. Using the condition, a valid direction vector is selected.  
__V__<sub>N0</sub> = [ _x_<sub>R</sub> _m_<sub>20</sub> - _m_<sub>00</sub> ,
_x_<sub>R</sub> _m_<sub>21</sub> - _m_<sub>01</sub> , _x_<sub>R</sub> _m_<sub>22</sub> - _m_<sub>02</sub> ] ,  
__V__<sub>N1</sub> = [ _y_<sub>R</sub> _m_<sub>20</sub> - _m_<sub>10</sub> ,
_y_<sub>R</sub> _m_<sub>21</sub> - _m_<sub>11</sub> ,
_y_<sub>R</sub> _m_<sub>22</sub> - _m_<sub>12</sub> ] ,  
The direction vector candidate is  
__V__<sub>DIRCAN</sub> = __V__<sub>N0</sub> &Cross; __V__<sub>N1</sub> .  
if &lt;__V__<sub>DIRCAN</sub> , (__P__<sub>LOOKAT</sub> - __O__<sub>C</sub>)&gt; &lt; 0 ,  
__V__<sub>DIR</sub> = -__V__<sub>DIRCAN</sub>  
else  
__V__<sub>DIR</sub> = __V__<sub>DIRCAN</sub> .  
