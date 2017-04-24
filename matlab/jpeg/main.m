clc,clear all,close all
mat = [1 2 6; 3 5 7; 4 8 9];
% mat = magic(5)
num = size(mat, 1) * size(mat, 2);  
code = zigzagCoder(mat, num);  
disp(mat);  
disp(code');  