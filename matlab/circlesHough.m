% https://files.nyu.edu/jb4457/public/files/research/bristol/hough-report.pdf
function C = circlesHough(I, min_rad, max_rad)
% find edges with canny edge detector
[edges, theta, magnitude] = edgeCanny(I, 0.2, 0.1);
figure('Name','edges','NumberTitle','off'); imshow(edges);

houghDomain = zeros(size(I));
dx = 0;
dy = 0;
px1 = 1;
px2 = 1;
py1 = 1;
py2 = 1;
figure('Name','Hough','NumberTitle','off');imshow(theta);

size_x = size(edges,1);
size_y = size(edges,2);

for i = 1:size(edges,1)
    for j = 1:size(edges,2)
        if(edges(i,j)>0)
            a = min_rad * cos(theta(i,j));
            b = min_rad * sin(theta(i,j));
            
            if(theta(i,j) < pi/4 && theta(i,j) > -pi/4)
                dx = ((a > 0) * 2) - 1;
                dy = dx * tan(theta(i,j));
            else
                dy = ((b > 0) * 2) - 1;
                dx = dy / tan(theta(i,j));
            end;
            
            while (sqrt(a^2 + b^2) < max_rad)
                px1 = int32(j + a);
                px2 = int32(j - a);
                py1 = int32(i - b);
                py2 = int32(i + b);
                
                if (px1 >= 1 && px1 < size_y && py1 >= 1 && py1 < size_x)
                    houghDomain(py1,px1) = houghDomain(py1,px1) + (magnitude(i,j)./sqrt(a^2+b^2));
                end;
                if (px2 >= 1 && px2 < size_y && py2 >= 1 && py2 < size_x)
                    houghDomain(py2,px2) = houghDomain(py2,px2) + (magnitude(i,j)./sqrt(a^2+b^2));
                end;
                a = a + dx;
                b = b + dy;
                
            end;
           
        end;
    end;
end;
maxVal = max(houghDomain(:));
houghDomain = houghDomain./maxVal;

figure('Name','Hough','NumberTitle','off');imshow(houghDomain);

houghDomain = medianFilter(houghDomain,6);
bitmask = houghDomain > 0.2;

houghDomain(~bitmask) = 0;

figure('Name','Hough hotspot','NumberTitle','off');imshow(houghDomain);
C = 0;
