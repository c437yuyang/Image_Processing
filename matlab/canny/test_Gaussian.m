x = -20:0.01:20;
sigma = 5;
mu = 0;

y = 1/(sqrt(2*pi)*sigma) * exp(-(x-mu).^2/2/sigma.^2);

plot(x,y)