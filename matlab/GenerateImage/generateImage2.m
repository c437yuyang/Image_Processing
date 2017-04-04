clc
bw = zeros(512,512);
bw(2:4:512,2:4:512) = 1;
imshow(bw);

imfinfo('../0.jpg')