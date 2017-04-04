clc,close all,clear all;

BW = imread('..\lena.jpg');

BW = rgb2gray(BW);

thresh = [0.01,0.17];

sigma = 2;

f = edge(double(BW),'canny',thresh,sigma);

figure(1);imshow(f);title('canny¼ì²â±ßÔµ');

[H,theta,rho] = hough(f,'RhoResolution',0.5);
% figure(2);
% imshow(theta,rho,H,[]);
% axis on,axis normal;
% xlabel('\theta');ylabel('\rho');

peak = houghpeaks(H,5);
lines = houghlines(f,theta,rho,peak);
figure(2);imshow(f,[]);title('HoughLine Detect Result');hold on;
for k = 1:length(lines)
    xy = [lines(k).point1;lines(k).point2];
     plot(xy(:,1),xy(:,2),'LineWidth',4,'Color',[.6 .6 .6]);
% plot(xy(:,1),xy(:,2));
end