
x = [0:0.01:20];
b=8;a=2;
p = heaviside(x-a).*2./b.*(x-a).*exp((-(x-a).^2)./b);
% p = raylpdf(x,0.5);
plot(x,p)


x1 = [0:0.01:20];

b=8;a=2;
p = heaviside(x-a).*2./b.*(x-a).*exp((-(x-a).^2)./b);