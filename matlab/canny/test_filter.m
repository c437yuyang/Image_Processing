w=fspecial('sobel');
h = w';

%ֵ��ˮƽ�ڱ䵫�Ǳ�Ե�Ǵ�ֱ�ģ�

%I=imread('lena.jpg')

I = zeros(5,5);
for i=1:5
    for j =1:5
        I(i,j) = j+5;
    end
end

w_I = imfilter(I,w,'replicate');
h_I = imfilter(I,h,'replicate');
figure(1);
imshow(I,[]);
figure(2);
imshow(w_I,[])
figure(3);
imshow(h_I,[])