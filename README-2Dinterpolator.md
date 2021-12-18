# 2-D interpolator
## Linear Function over a triangle in 3-D space
Coordinate in 3D geometry space G is represented by
__P__<sub>G</sub> = [ _X_<sub>G</sub> , _Y_<sub>G</sub> , _Z_<sub>G</sub> , _W_<sub>G</sub> ]<sup>T</sup> .  
Projecting to 2D texture space S is represented by
__P__<sub>S</sub> = [ _X_<sub>S</sub> , _Y_<sub>S</sub> , _Z_<sub>S</sub> ]<sup>T</sup>  
__P__<sub>S</sub> = A P __P__<sub>G</sub>  
, where
A = [  
&nbsp; &nbsp; _&alpha;_<sub>00</sub> , _&alpha;_<sub>01</sub> , _&alpha;_<sub>02</sub> ;  
&nbsp; &nbsp; _&alpha;_<sub>10</sub> , _&alpha;_<sub>11</sub> , _&alpha;_<sub>12</sub> ;  
&nbsp; &nbsp; _&alpha;_<sub>20</sub> , _&alpha;_<sub>21</sub> , _&alpha;_<sub>22</sub> ;  
] ,  
P = [  
&nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 1 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
]  

Texture coordinates,  
[ _x_<sub>S0</sub> , _y_<sub>S0</sub> ]<sup>T</sup> =
[ _X_<sub>S0</sub> / _Z_<sub>S0</sub> , _Y_<sub>S0</sub> / _Z_<sub>S0</sub> ]  

[ _X_<sub>S</sub> , _Y_<sub>S</sub> , _Z_<sub>S</sub> ]<sup>T</sup> = [  
&nbsp; &nbsp; _&alpha;_<sub>00</sub> , _&alpha;_<sub>01</sub> , _&alpha;_<sub>02</sub> ;  
&nbsp; &nbsp; _&alpha;_<sub>10</sub> , _&alpha;_<sub>11</sub> , _&alpha;_<sub>12</sub> ;  
&nbsp; &nbsp; _&alpha;_<sub>20</sub> , _&alpha;_<sub>21</sub> , _&alpha;_<sub>22</sub> ;  
] [ _X_<sub>G</sub> , _Y_<sub>G</sub> , _Z_<sub>G</sub> ]

## Bilinear interpolation in R2 space
The function is defined as  
&nbsp; &nbsp; f(_x_, _y_) = f(_x_<sub>0</sub> + _&delta;x_ , _y_<sub>0</sub> + _&delta;y_) =
_a_<sub>0</sub> + _a_<sub>1</sub> _&delta;x_ + _a_<sub>2</sub> _&delta;y_ + _a_<sub>3</sub> _&delta;x &delta;y_  
f(_x_<sub>0</sub> , _y_<sub>0</sub>) = _F_<sub>0</sub> ,  
f(_x_<sub>0</sub> + _&Delta;x_ , _y_<sub>0</sub>) = _F_<sub>1</sub> ,  
f(_x_<sub>0</sub> , _y_<sub>0</sub> + _&Delta;y_) = _F_<sub>2</sub> ,  
f(_x_<sub>0</sub> + _&Delta;x_ , _y_<sub>0</sub> + _&Delta;y_) = _F_<sub>3</sub> ,  
<br>
[ _F_<sub>0</sub> , _F_<sub>1</sub> , _F_<sub>2</sub> , _F_<sub>3</sub> ]<sup>T</sup> = [  
&nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 1 , _&Delta;x_ , 0 , 0 ;  
&nbsp; &nbsp; 1 , 0 , _&Delta;y_ , 0 ;  
&nbsp; &nbsp; 1 , _&Delta;x_ , _&Delta;y_ , _&Delta;x&Delta;y_ ;  
] [ _a_<sub>0</sub> , _a_<sub>1</sub> , _a_<sub>2</sub> , _a_<sub>3</sub> ]<sup>T</sup>  
<br>
[ _a_<sub>0</sub> , _a_<sub>1</sub> , _a_<sub>2</sub> , _a_<sub>3</sub> ]<sup>T</sup> = [  
&nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 1 , _&Delta;x_ , 0 , 0 ;  
&nbsp; &nbsp; 1 , 0 , _&Delta;y_ , 0 ;  
&nbsp; &nbsp; 1 , _&Delta;x_ , _&Delta;y_ , _&Delta;x&Delta;y_ ;  
]<sup>-1</sup> [ _F_<sub>0</sub> , _F_<sub>1</sub> , _F_<sub>2</sub> , _F_<sub>3</sub> ]<sup>T</sup>

if _&Delta;x_ = 1, _&Delta;y_ = 1, then  
[  
&nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 1 , _&Delta;x_ , 0 , 0 ;  
&nbsp; &nbsp; 1 , 0 , _&Delta;y_ , 0 ;  
&nbsp; &nbsp; 1 , _&Delta;x_ , _&Delta;y_ , _&Delta;x&Delta;y_ ;  
]<sup>-1</sup> =  [  
&nbsp; &nbsp; 1, 0, 0, 0 ;  
&nbsp; &nbsp; -1, 1, 0, 0 ;  
&nbsp; &nbsp; -1, 0, 1, 0 ;  
&nbsp; &nbsp; 1, -1, -1, 1 ;  
]  
_a_<sub>0</sub> = _F_<sub>0</sub>  
_a_<sub>1</sub> = -_F_<sub>0</sub> + _F_<sub>1</sub>  
_a_<sub>2</sub> = -_F_<sub>0</sub> + _F_<sub>2</sub>  
_a_<sub>3</sub> = _F_<sub>0</sub> - _F_<sub>1</sub> - _F_<sub>2</sub> + _F_<sub>3</sub>  

