clc
clear all;
I = imread('..\1.jpg');
I = rgb2gray(I);

muAll = mean(I(:))

T1 = 10;
C1 = find(I>T1);
C2 = find(I<=T1);

muC1 = 0,muC2 = 0;
if(length(C1)~=0)
    muC1 = mean(I(C1));
end
if(length(C2)~=0)
    muC2 = mean(I(C2));
end

T2 = (muC1+muC2)/2


while T2-T1>1
    T1 = T2;
    C1 = find(I>T2);
    C2 = find(I<=T2);
    muC1 = 0,muC2 = 0;
    if(length(C1)~=0)
        muC1 = mean(I(C1));
    end
    if(length(C2)~=0)
        muC2 = mean(I(C2));
    end
    T2 = (muC1+muC2)/2;
end
T2
imshow(I>T2);