function img = saturateCast(src)
src(src>255) = 255;
src(src<0) = 0;
img = src;