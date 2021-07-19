#the root directory of MiMeS
rootDir=../../../

#######################################-precision-#######################################
# LONG=long : in order to use long doubles. This is slower, but generally more accurate, since we can use tolerances down to 10^-11 (or below that). 
# LONG= : in order to use doubles. This is faster. However the tolerances of the ODE solver cannt be below 1e-8.

# use long doubles 
LONG=long
# use doubles
# LONG= 