# Camera matrix formulation
## Camera local projection
Normalized P<sup>2</sup> coordinate on the standard image projection plane at _Z_ = -1 is  
__m__ = [ _u_ , _v_ , 1 ]<sup>T</sup> = [ _x_ , _y_ , _z_ ]<sup>T</sup>  
&nbsp; &nbsp; `P`<sub>0</sub> [ _X_ , _Y_ , _Z_ , _T_ ]<sup>T</sup>  
, where
`P`<sub>0</sub> = [  
&nbsp; &nbsp; 1 , 0 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 1 , 0 , 0 ;  
&nbsp; &nbsp; 0 , 0 , 1 , 0 ;  
]  
Generic prjection matrix `P` is  
`P` = [  
&nbsp; &nbsp; &alpha;<sub>u</sub> , &gamma; , _u_<sub>0</sub> ;  
&nbsp; &nbsp; 0 , &alpha;<sub>v</sub> , _v_<sub>0</sub> ;  
&nbsp; &nbsp; 0 , 0 , 1 ;  
] `P`<sub>0</sub> [  
&nbsp; &nbsp; `R` , `t` ;  
&nbsp; &nbsp; 0<sub>3</sub><sup>T</sup> , 1 ;  
]  
&nbsp; &nbsp; = `A` `P`<sub>0</sub> `D` =  `A` [ `R`, `t` ]  

## Calibration; deriving a generic projection matrix `P`
Firstly, `P` is represented by its row vectors as  
`P` = [  
    __U__<sup>T</sup> ;  
    __V__<sup>T</sup> ;  
    __W__<sup>T</sup> ;  
]  

The proojection equiation is rewritten as  
[ _u_ , _v_ , 1 ]<sup>T</sup> = `P` __M__ =  
[  
&nbsp; &nbsp; __U__<sup>T</sup> __M__ ;  
&nbsp; &nbsp; __V__<sup>T</sup> __M__ ;  
&nbsp; &nbsp; __W__<sup>T</sup> __M__ ;  
] --- (1)  
, where __M__ is a general form of reference points measure in a 3-D coordinate frame.  
__Note__ : A standard calibration device, a flat plate with regular tick can be measure on itself.  
Expand (1),  
_u_ = __U__<sup>T</sup> __M__ / __W__<sup>T</sup> __M__ ,  
_v_ = __V__<sup>T</sup> __M__ / __W__<sup>T</sup> __M__  
<br>
_u_ __W__<sup>T</sup> __M__ - __U__<sup>T</sup> __M__ = 0 ,  
_v_ __W__<sup>T</sup> __M__ - __V__<sup>T</sup> __M__ = 0  
<br>
