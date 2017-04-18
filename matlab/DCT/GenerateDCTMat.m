function DCTMat = GenerateDDCTMat(N)
DCTMat=zeros(N);
for i=0:N-1
    for j=0:N-1
        if i==0
            a=sqrt(1/N);
        else
            a=sqrt(2/N);
        end            
        DCTMat(i+1,j+1)=a*cos(pi*(j+0.5)*i/N);
    end
end
% return DCTMat