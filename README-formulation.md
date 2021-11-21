# Formulataion of Camera calibration with known flat plate
__`X`__ = [`X`,`Y`,`Z`,`W`]<sup>T</sup> : flat plate coordinate used as a calibration standard  
__`x`__ = [`x`,`y`,`w`]<sup>T</sup> : plate coordinate projected to a captured image  
`C`<sub>I</sub> : camera intrinsic matrix, 3x4 matrix  
`C`<sub>E</sub> : camera extrinsic matrix of captured image, 4x4 homogenous matrix  
__`x`__ = `C`<sub>I</sub> `C`<sub>E</sub> __`X`__ = `C` __`X`__  
`C`<sub>I</sub> = `A` `P`<sub>0</sub>  
`A` = [  
&nbsp; &nbsp; &alpha;<sub>x</sub> , &gamma; , `x`<sub>0</sub> ;  
&nbsp; &nbsp; 0 , &alpha;<sub>y</sub> , `y`<sub>0</sub> ;  
&nbsp; &nbsp; 0 , 0 , 1 ]  
`P`<sub>0</sub> = [  
&nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 1 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ]  
`C`<sub>E</sub> = [  
&nbsp; &nbsp; `R`, **`t`** ;  
&nbsp; &nbsp; 0<sub>3</sub><sup>T</sup> , 1 ]  
<br>
, where  
`R` : 3D rotation matrix  
**`t`** : transport vector
<br>

**NOTE**  
Known four points on the plate in matrix representation
[`X`<sub>0</sub> `X`<sub>1</sub> `X`<sub>2</sub> `X`<sub>3</sub> ] = `X`<sub>K</SUB> is a 4x4 matrix.  
`x`<sub>_i_ K</sub> = `C`<sub>I</sub> `C`<sub>E _i_</sub> `X`<sub>K</SUB>  
`x`<sub>_i_ K</sub> `X`<sub>K</SUB><sup>-1</sup> = `C`<sub>I</sub> `C`<sub>E _i_</sub>  


## Camera frustum to reference image plane matrix
`(u, v)` : coordinate in reference image plane  
`(X, Y, Z, W)` : coordinate in camera local coordinate in P3 space  
`u` = `-X / Z`, `v` = `-Y / Z` : The negative signs are included because the camera sees `-Z` direction.  
As camera specifications, horizontal and vertical FOV angles are given.  
(_&Phi;_<sub>u</sub>, _&Phi;_<sub>v</sub>) : (horizontal FOV, vertical FOV)  
image plane bottom-left corner is  
(u<sub>0</sub>, v<sub>0</sub>) = (-tan(_&Phi;_<sub>u</sub> / 2), -tan(_&Phi;_<sub>v</sub> / 2))  
image plane top-right corner is  
(u<sub>1</sub>, v<sub>1</sub>) = (tan(_&Phi;_<sub>u</sub> / 2), tan(_&Phi;_<sub>v</sub> / 2))  

camera frustum to image plane conversion matrix is  
[  
&nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 1 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
]

## Reference image plane to real viewport matrix
Camera intrinsic matrix `M` is a upper triangle matrix like  
`M` = [  
&nbsp; &nbsp; _m_<sub>00</sub> , _m_<sub>01</sub> , _m_<sub>02</sub> , 0 ;  
&nbsp; &nbsp; 0 , _m_<sub>11</sub> , _m_<sub>12</sub> , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
]  
An ideally rectangle camemra frustum makes skew factor zero. In such cases, an ideal camear intrinsic matrix
`M`<sub>I</sub> is represented as  
`M`<sub>I</sub> = [  
&nbsp; &nbsp; _m_<sub>00</sub> , 0 , _m_<sub>02</sub> , 0 ;  
&nbsp; &nbsp; 0 , _m_<sub>11</sub> , _m_<sub>12</sub> , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
]  

Any points on the top-right viewline are  
__P__<sub>TR</sub> = [_Z_ tan(_&Phi;_<sub>u</sub> / 2) , _Z_ tan(_&Phi;_<sub>v</sub> / 2) , -_Z_ , 1]<sup>T</sup>  
Any points on the bottom-left viewline are  
__P__<sub>BL</sub> = [-_Z_ tan(_&Phi;_<sub>u</sub> / 2) , -_Z_ tan(_&Phi;_<sub>v</sub> / 2) , -_Z_ , 1]<sup>T</sup>  
Corresponding projected viewport points are  
__Q__<sub>TR</sub> = `M`<sub>I</sub> __P__<sub>TR</sub>  
__Q__<sub>BL</sub> = `M`<sub>I</sub> __P__<sub>BL</sub>  

[_x_<sub>TR</sub> , _y_<sub>TR</sub> , _z_<sub>TR</sub>]<sup>T</sup> = [  
&nbsp; &nbsp; _m_<sub>00</sub> , 0 , _m_<sub>02</sub> , 0 ;  
&nbsp; &nbsp; 0 , _m_<sub>11</sub> , _m_<sub>12</sub> , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
] [-_Z_ tan(_&Phi;_<sub>u</sub> / 2) , -_Z_ tan(_&Phi;_<sub>v</sub> / 2) , -_Z_ , 1]<sup>T</sup>  

= [-_m_<sub>00</sub> _Z_ tan(_&Phi;_<sub>u</sub> / 2) - _m_<sub>02</sub> _Z_ ,
-_m_<sub>11</sub> _Z_ tan(_&Phi;_<sub>v</sub> / 2) - _m_<sub>12</sub> _Z_ , -_Z_ ]<sup>T</sup>  

[_x_<sub>BL</sub> , _y_<sub>BL</sub> , _z_<sub>BL</sub>]<sup>T</sup> = [  
&nbsp; &nbsp; _m_<sub>00</sub> , 0 , _m_<sub>02</sub> , 0 ;  
&nbsp; &nbsp; 0 , _m_<sub>11</sub> , _m_<sub>12</sub> , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
] [_Z_ tan(_&Phi;_<sub>u</sub> / 2) , _Z_ tan(_&Phi;_<sub>v</sub> / 2) , -_Z_ , 1]<sup>T</sup>  

= [ _m_<sub>00</sub> _Z_ tan(_&Phi;_<sub>u</sub> / 2) - _m_<sub>02</sub> _Z_ ,
_m_<sub>11</sub> _Z_ tan(_&Phi;_<sub>v</sub> / 2) - _m_<sub>12</sub> _Z_ , -_Z_ ]<sup>T</sup>  

real pixel coord  
[_U_<sub>TR</sub>, _V_<sub>TR</sub>]<sup>T</sup> =
[_x_<sub>TR</sub> / _z_<sub>TR</sub> , _y_<sub>TR</sub> / _z_<sub>TR</sub>]<sup>T</sup>  
= [ _m_<sub>00</sub> tan(_&Phi;_<sub>u</sub> / 2) + _m_<sub>02</sub> ,
_m_<sub>11</sub> tan(_&Phi;_<sub>v</sub> / 2) + _m_<sub>12</sub> ]<sup>T</sup>  

[_U_<sub>BL</sub>, _V_<sub>BL</sub>]<sup>T</sup> =
[_x_<sub>BL</sub> / _z_<sub>BL</sub> , _y_<sub>BL</sub> / _z_<sub>BL</sub>]<sup>T</sup>  
= [ -_m_<sub>00</sub> tan(_&Phi;_<sub>u</sub> / 2) + _m_<sub>02</sub> ,
-_m_<sub>11</sub> tan(_&Phi;_<sub>v</sub> / 2) + _m_<sub>12</sub> ]<sup>T</sup>  
