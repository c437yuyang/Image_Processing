function linkEdge(mag_low,mag_high,flag_low,flag_high,xc,yc,width,height,loopCount)

if xc==1 
    xl = xc;
else
    xl = xc-1;
end
if xc==width  
    xr = xc;
else
    xr = xc+1;
end

if yc==1 
    yu = yc;
else
    yu = yc-1;
end
if yc==height  
    yd = yc;
else
    yd = yc+1;
end

if flag_high(xc,yc)==0
    for x=xl:xr
        for y=yu:yd
            if((x~=xc || y~=yc) && mag_low(x,y)~=0 && flag_low(x,y)==0)
                mag_high(x,y) = mag_low(x,y);
                flag_low(x,y) = 1;
                linkEdge(mag_low,mag_high,flag_low,flag_high,x,y,width,height,loopCount);
                loopCount = loopCount + 1;
                if(loopCount > 1000)
                    break;
                end
            end
        end
    end
    flag_high(xc,yc) = 1;
end