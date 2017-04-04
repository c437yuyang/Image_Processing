
I = imread('..\lena.jpg');

a=rgb2gray(I);  
level = graythresh(a);  
a=im2bw(a,level);  
imshow(a,[]);  
