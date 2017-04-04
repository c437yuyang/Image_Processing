function [ff,flag1]=findline(k3,k4,flag,m,n)

flag1=flag;
  m1=m+1;n1=n+1;
  while(m~=m1||n~=n1)%若m和n都不发生变化，表明line已到终点
  flagg=0;
  for i=1:3
      if(flagg==1) break;          
      end
      for j=1:3
          if k3(m-2+i,n-2+j)~=0
              k4(m-2+i,n-2+j)=255;
              m1=m-2+i;n1=n-2+j;%新的[m,n]点
              flag1(m,n)=1;%标记已检测过
              flagg=1;break;
      end
      end
  end
    m=m1;n=n1;
  end
  ff=k4;
  
  