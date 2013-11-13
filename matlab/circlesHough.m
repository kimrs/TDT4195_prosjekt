% https://files.nyu.edu/jb4457/public/files/research/bristol/hough-report.pdf
function C = circlesHough(I, min_rad, max_rad)
% find edges with canny edge detector
[edges, theta] = edgeCanny(I, 0.2, 0.1);
figure('Name','edges','NumberTitle','off'); imshow(edges);

houghDomain = zeros(size(I));
dx = 0;
dy = 0;
px1 = 0;
px2 = 0;
py1 = 0;
py2 = 0;

size_x = size(edges,1);
size_y = size(edges,2);

for i = 1:size(edges,1)
    for j = 1:size(edges,2)
        if(edges(i,j)>0)
            a = min_rad * cosd(theta(i,j));
            b = min_rad * sind(theta(i,j));
            
            if(theta(i,j) < pi/4 && theta(i,j) > -pi/4)
                dx = ((a > 0) * 2) - 1;
                dy = dx * tand(theta(i,j));
            else
                dy = ((b > 0) * 2) - 1;
                dx = dy * tand(theta(i,j));
            end;
            
            while (sqrt(a^2 + b^2) > max_rad)
                px1 = int16(j + a);
                px2 = int16(j - a);
                py1 = int16(j - b);
                py2 = int16(j + b);
                
                if (px1 >= 0 && px1 < size_x && py1 >= 0 && py1 < size_y)
                    houghDomain(px1,py1) = houghDomain(px1,py1) + edges(i,j)./sqrt(a^2+b^2);
                end;
                if (px2 >= 0 && px2 < size_x && py2 >= 0 && py2 < size_y)
                    houghDomain(px2,py2) = houghDomain(px2,py2) + edges(i,j)./sqrt(a^2+b^2);
                end;
                
                a = a + dx;
                b = b + dy;
            end;
        end;
    end;
end;

figure('Name','Hough','NumberTitle','off');imshow(houghDomain);
C = 0;

% 
% #define MAX(a,b) ((a)>(b) ? (a) : (b))
% #define MIN(a,b) ((a)<(b) ? (a) : (b))
% #define ABS(a) ((a)>0 ? (a) : -(a))
% #define SIGN(a) ((a)>0 ? 1 : -1)
