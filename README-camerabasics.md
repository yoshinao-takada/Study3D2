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
R | in image sensor coordinate
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

__P__ = [ _X_ , _Y_ , _Z_ ]<sup>T</sup> : point in homogeneous coordinate  
__p__ = [ _x_, _y_ , _z_ ]<sup>T</sup> : point in real coordinate

## Projection matrix from camera coordinate to viewport coordinate
__P__<sub>C</sub> , homogeneous camera coordinate is transformed to __P__<sub>P</sub> by  
__P__<sub>P</sub> = [  
    &nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
    &nbsp; &nbsp; 0 , 1 , 0 , 0 ;  
    &nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
] __P__<sub>C</sub>  
Its real coordinate is  
__p__<sub>P</sub> = [ _x_<sub>P</sub> , _y_<sub>P</sub> ]<sup>T</sup> = 
[ _X_<sub>P</sub> / _Z_<sub>P</sub> , _Y_<sub>P</sub> / _Z_<sub>P</sub>]<sup>T</sup>  
<br>
End of real viewport coordinate and center of real viewport coordinate is related by  
(_x_<sub>R</sub><sup>E</sup> - _x_<sub>R</sub><sup>C</sup>) _x_<sub>R</sub><sup>D</sup> = 
_x_<sub>P</sub><sup>E</sup>  
(_y_<sub>R</sub><sup>E</sup> - _y_<sub>R</sub><sup>C</sup>) _y_<sub>R</sub><sup>D</sup> = 
_y_<sub>P</sub><sup>E</sup>  
_x_<sub>R</sub><sup>D</sup> = _x_<sub>p</sub><sup>E</sup> / (_x_<sub>R</sub><sup>E</sup> - _x_<sub>R</sub><sup>C</sup>)  
_y_<sub>R</sub><sup>D</sup> = _y_<sub>P</sub><sup>E</sup> / (_y_<sub>R</sub><sup>E</sup> - _y_<sub>R</sub><sup>C</sup>)   
General point in real viewport is  
_x_<sub>R</sub> = _x_<sub>p</sub> / _x_<sub>R</sub><sup>D</sup> + _x_<sub>R</sub><sup>C</sup>  
_y_<sub>R</sub> = _y_<sub>p</sub> / _y_<sub>R</sub><sup>D</sup> + _y_<sub>R</sub><sup>C</sup>  

[ _X_<sub>R</sub> , _Y_<sub>R</sub> , _Z_<sub>R</sub> ]<sup>T</sup> = [  
    &nbsp; &nbsp; 1 / _x_<sub>R</sub><sup>D</sup> , 0 , _x_<sub>R</sub><sup>C</sup> ;  
    &nbsp; &nbsp; 0 , 1 / _y_<sub>R</sub><sup>D</sup> , _y_<sub>R</sub><sup>C</sup> ;  
    &nbsp; &nbsp; 0 , 0 , 1 ;  
] [ _X_<sub>p</sub> , _Y_<sub>p</sub> , _Z_<sub>p</sub> ]<sup>T</sup>

## Generic projection matrix consisting of intrinsic matrix and extrinsic matrix
From the investigation of camera matrix, the intrinsic matrix is  
P<sub>I</sub> = [  
    &nbsp; &nbsp; _&Alpha;_<sub>I00</sub> , 0 , _T_<sub>I0</sub> ;  
    &nbsp; &nbsp; 0 , _&Alpha;_<sub>I11</sub> , _T_<sub>I1</sub> ;  
    &nbsp; &nbsp; 0 , 0 , 1 ;  
] = [  
    &nbsp; &nbsp; 1 / _x_<sub>R</sub><sup>D</sup> , 0 , _x_<sub>R</sub><sup>C</sup> ;  
    &nbsp; &nbsp; 0 , 1 / _y_<sub>R</sub><sup>D</sup> , _y_<sub>R</sub><sup>C</sup> ;  
    &nbsp; &nbsp; 0 , 0 , 1 ;  
]  

Extrinsic matrix is defined by inverse of camera position as  
P<sub>E</sub> = [  
    &nbsp; &nbsp; _&Alpha;_<sub>E00</sub> , _&Alpha;_<sub>E01</sub> ,  _&Alpha;_<sub>E02</sub> , _T_<sub>E0</sub>  ;  
    &nbsp; &nbsp; _&Alpha;_<sub>E10</sub> , _&Alpha;_<sub>E11</sub> ,  _&Alpha;_<sub>E12</sub> , _T_<sub>E1</sub>  ;  
    &nbsp; &nbsp; _&Alpha;_<sub>E20</sub> , _&Alpha;_<sub>E21</sub> ,  _&Alpha;_<sub>E22</sub> , _T_<sub>E2</sub>  ;  
    &nbsp; &nbsp; 0 , 0 , 0 , 1 ;  
]  

__P__<sup>3</sup> - __P__<sup>4</sup> space conversion matrix P<sub>0</sub> is  
P<sub>0</sub> = [  
    &nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
    &nbsp; &nbsp; 0 , 1 , 0 , 0 ;  
    &nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
]  

Generic projection matrix is a product of the three matrices.  
P = P<sub>I</sub> P<sub>0</sub> P<sub>E</sub> =  [  
    &nbsp; &nbsp; _&Alpha;_<sub>I00</sub> _&Alpha;_<sub>E00</sub> + _T_<sub>I0</sub> _&Alpha;_<sub>E20</sub> ,
    &nbsp; &nbsp; _&Alpha;_<sub>I00</sub> _&Alpha;_<sub>E01</sub> + _T_<sub>I0</sub> _&Alpha;_<sub>E21</sub> ,
    &nbsp; &nbsp; _&Alpha;_<sub>I00</sub> _&Alpha;_<sub>E02</sub> + _T_<sub>I0</sub> _&Alpha;_<sub>E22</sub> ,
    &nbsp; &nbsp; _&Alpha;_<sub>I00</sub> _T_<sub>E0</sub> + _T_<sub>I0</sub> _T_<sub>E2</sub> ;  
    &nbsp; &nbsp; _&Alpha;_<sub>I11</sub> _&Alpha;_<sub>E10</sub> + _T_<sub>I1</sub> _&Alpha;_<sub>E20</sub> ,
    &nbsp; &nbsp; _&Alpha;_<sub>I11</sub> _&Alpha;_<sub>E11</sub> + _T_<sub>I1</sub> _&Alpha;_<sub>E21</sub> ,
    &nbsp; &nbsp; _&Alpha;_<sub>I11</sub> _&Alpha;_<sub>E12</sub> + _T_<sub>I1</sub> _&Alpha;_<sub>E22</sub> ,
    &nbsp; &nbsp; _&Alpha;_<sub>I11</sub> _T_<sub>E1</sub> + _T_<sub>I0</sub> _T_<sub>E2</sub> ;  
    &nbsp; &nbsp; _&Alpha;_<sub>E20</sub> ,
    &nbsp; &nbsp; _&Alpha;_<sub>E21</sub> ,
    &nbsp; &nbsp; _&Alpha;_<sub>E22</sub> ,
    &nbsp; &nbsp; _T_<sub>E2</sub> ;  
] = [  
    &nbsp; &nbsp; _P_<sub>00</sub> , _P_<sub>01</sub> , _P_<sub>02</sub> , _P_<sub>03</sub> ;   
    &nbsp; &nbsp; _P_<sub>10</sub> , _P_<sub>11</sub> , _P_<sub>12</sub> , _P_<sub>13</sub> ;   
    &nbsp; &nbsp; _P_<sub>20</sub> , _P_<sub>21</sub> , _P_<sub>22</sub> , _P_<sub>23</sub> ;   
]

Generic projection matrix is a generic 3x4 dense matrix.

[ _X_<sub>R</sub> , _Y_<sub>R</sub> , _Z_<sub>R</sub> ]<sup>T</sup> = 
P [ _x_<sub>W</sub> , _y_<sub>W</sub> , _z_<sub>W</sub> , 1 ]<sup>T</sup>  
<br>
_X_<sub>R</sub> = _P_<sub>00</sub> _x_<sub>W</sub> + _P_<sub>01</sub> _y_<sub>W</sub> +
_P_<sub>02</sub> _z_<sub>W</sub> + _P_<sub>03</sub>  
_Y_<sub>R</sub> = _P_<sub>10</sub> _x_<sub>W</sub> + _P_<sub>011</sub> _y_<sub>W</sub> +
_P_<sub>12</sub> _z_<sub>W</sub> + _P_<sub>13</sub>  
_Z_<sub>R</sub> = _P_<sub>20</sub> _x_<sub>W</sub> + _P_<sub>21</sub> _y_<sub>W</sub> +
_P_<sub>22</sub> _z_<sub>W</sub> + _P_<sub>23</sub>  
_x_<sub>R</sub> = _X_<sub>R</sub> / _Z_<sub>R</sub>  
_y_<sub>R</sub> = _Y_<sub>R</sub> / _Z_<sub>R</sub>  
0 = _X_<sub>R</sub> - _x_<sub>R</sub> _Z_<sub>R</sub>  
0 = _Y_<sub>R</sub> - _y_<sub>R</sub> _Z_<sub>R</sub>   
0 = _P_<sub>00</sub> _x_<sub>W</sub> + _P_<sub>01</sub> _y_<sub>W</sub> +
_P_<sub>02</sub> _z_<sub>W</sub> + _P_<sub>03</sub> - _x_<sub>R</sub> (
    _P_<sub>20</sub> _x_<sub>W</sub> + _P_<sub>21</sub> _y_<sub>W</sub> +
_P_<sub>22</sub> _z_<sub>W</sub> + _P_<sub>23</sub>
)  
0 = _P_<sub>10</sub> _x_<sub>W</sub> + _P_<sub>011</sub> _y_<sub>W</sub> +
_P_<sub>12</sub> _z_<sub>W</sub> + _P_<sub>13</sub> - _y_<sub>R</sub> (
    _P_<sub>20</sub> _x_<sub>W</sub> + _P_<sub>21</sub> _y_<sub>W</sub> +
_P_<sub>22</sub> _z_<sub>W</sub> + _P_<sub>23</sub>
)  
Set a fixed value, _P_<sub>23</sub> = 1 ,  
_x_<sub>R</sub> = _P_<sub>00</sub> _x_<sub>W</sub> + _P_<sub>01</sub> _y_<sub>W</sub> +
_P_<sub>02</sub> _z_<sub>W</sub> + _P_<sub>03</sub> - _x_<sub>R</sub> (
    _P_<sub>20</sub> _x_<sub>W</sub> + _P_<sub>21</sub> _y_<sub>W</sub> +
_P_<sub>22</sub> _z_<sub>W</sub> )  
_y_<sub>R</sub> = _P_<sub>10</sub> _x_<sub>W</sub> + _P_<sub>011</sub> _y_<sub>W</sub> +
_P_<sub>12</sub> _z_<sub>W</sub> + _P_<sub>13</sub> - _y_<sub>R</sub> (
    _P_<sub>20</sub> _x_<sub>W</sub> + _P_<sub>21</sub> _y_<sub>W</sub> +
_P_<sub>22</sub> _z_<sub>W</sub> )  
