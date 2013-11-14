% Described here:
% http://www.cse.iitd.ernet.in/~pkalra/csl783/canny.pdf
function [J, theta, magnitude] = edgeCanny(I, highThresh, lowThresh)
    
    
    % Smooth image
    J = gaussian(I,7,1.4);
    
    % Find gradients
    %L = findGradients(J);
    [Dx, Dy] = findGradients(J);
    
    %figure('Name','Horizontal gradients','NumberTitle','off'); imshow(Dx);
    %figure('Name','Vertical gradients','NumberTitle','off'); imshow(Dy);
    
    % Combine both x and y direction (get the magnitude)
    % for faster computation use this
    % aprox_magnitude = abs(Dx)+abs(Dy);
    
    magnitude = sqrt(Dx.^2+Dy.^2);
    % The direction of edges
    theta = zeros(size(Dx));
    theta = atand(Dy./Dx);
    
    %figure('Name','Theta','NumberTitle','off'); imshow(theta);
    
    nomaxsup = zeros(size(J,1),size(J,2));
    
    % normalize angles
    
    for x = 1:size(nomaxsup,1)
        for y = 1:size(nomaxsup,2)
            
            % Putting each angle inside their respective quadrant
       
            if ((theta(x,y) > -22.5 && theta(x,y) < 22.5) || (theta(x,y) > 157.5) && (theta(x,y) < -157.5))
                if (y - 1 > 0 && y + 1 < size(nomaxsup,2))
                    % do a non-maximum suppression
                    if (magnitude(x, y) > magnitude(x, y+1) && magnitude(x, y) > magnitude(x, y-1))
                        nomaxsup(x,y) = magnitude(x,y);
                    else
                        nomaxsup(x,y) = 0;
                    end;
                end;
            end;

            if ((theta(x,y) < -112.5) && (theta(x,y) > -157.5) || (theta(x,y) > 22.5) && (theta(x,y) < 67.5))
                if (y - 1 > 0 && y + 1 < size(nomaxsup,2) && x - 1 > 0 && x + 1 < size(nomaxsup,1))
                    % do a non-maximum suppression
                    if (magnitude(x, y) > magnitude(x-1, y+1) && magnitude(x, y) > magnitude(x+1, y-1))
                        nomaxsup(x,y) = magnitude(x,y);
                    else
                        nomaxsup(x,y) = 0;
                    end;
                end;
            end;

            if ((theta(x,y) > 67.5 && theta(x,y) < 112.5) || (theta(x,y) < -67.5 && theta(x,y) > -112.5))
                if (x - 1 > 0 && x + 1 < size(nomaxsup,1))
                    % do a non-maximum suppression
                    if (magnitude(x, y) > magnitude(x+1, y) && magnitude(x, y) > magnitude(x-1, y))
                        nomaxsup(x,y) = magnitude(x,y);
                    else
                        nomaxsup(x,y) = 0;
                    end;
                end;
            end;

            if ((theta(x,y) < -22.5 && theta(x,y) > -67.5) || (theta(x,y) > 112.5 && theta(x,y) <= 157.5))
                if (y - 1 > 0 && y + 1 < size(nomaxsup,2) && x - 1 > 0 && x + 1 < size(nomaxsup,1))
                    % do a non-maximum suppression
                    if (magnitude(x, y) > magnitude(x-1, y-1) && magnitude(x, y) > magnitude(x+1, y+1))
                        nomaxsup(x,y) = magnitude(x,y);
                    else
                        nomaxsup(x,y) = 0;
                    end;
                end;
            end;
        end
    end
    
    
    % normalize values max gets to be 1, etc.
    %max_intensity = max(nomaxsup(:));
    
    %mul = 1 / max_intensity;
    
    %nomaxsup = nomaxsup.*mul;

    
    %figure(); imshow(nomaxsup);
    
    L = zeros(size(nomaxsup,1),size(nomaxsup,2));
    H = zeros(size(nomaxsup,1),size(nomaxsup,2));
    
    % Hysteresis and thresholding
    
    % Define which pixel is high threshold, which is low. Store in seperate
    % matrixes
    for x = 1:size(nomaxsup,1)
        for y = 1:size(nomaxsup,2)
            if(nomaxsup(x,y) > highThresh)
                H(x,y) = 1;
                L(x,y) = 0;
            else
                if(nomaxsup(x,y) >= lowThresh && nomaxsup(x,y) <= highThresh )
                    H(x,y) = 0;
                    L(x,y) = 1;
                else
                    if (nomaxsup(x,y) < lowThresh)
                        L(x,y) = 0;
                        H(x,y) = 0;
                    end
                end
            end
        end
    end
    
    %figure('Name','LowThreshold','NumberTitle','off');imshow(L);
    %figure('Name','HighThreshold','NumberTitle','off');imshow(H);
    
    % Go through the low threshold matrix and see whether the low threshold
    % pixels are adjecent to a high threshold blob. Dooin it twice.
    for x = 1:size(nomaxsup,1)
        for y = 1:size(nomaxsup,2)
            if (x-1 > 0 && y-1 > 0 && y+1 < size(nomaxsup,2) && x+1 < size(nomaxsup,1) && L(x,y) == 1)
                if (nomaxsup(x,y) > highThresh || nomaxsup(x-1,y) > highThresh || nomaxsup(x-1,y-1) > highThresh || nomaxsup(x,y-1) > highThresh || nomaxsup(x+1,y) > highThresh || nomaxsup(x+1,y+1) > highThresh || nomaxsup(x,y+1) > highThresh || nomaxsup(x-1,y+1) > highThresh || nomaxsup(x+1,y-1) > highThresh)
                    H(x,y) = 1;
                else 
                    H(x,y) = 0;
                end;
            end;
        end;
    end;
    
%     for y = 1:size(nomaxsup,2)
%         for x = 1:size(nomaxsup,1)
%             if (x-1 > 0 && y-1 > 0 && y+1 < size(nomaxsup,2) && x+1 < size(nomaxsup,1) && L(x,y) == 1)
%                 if (nomaxsup(x,y) > highThresh || nomaxsup(x-1,y) > highThresh || nomaxsup(x-1,y-1) > highThresh || nomaxsup(x,y-1) > highThresh || nomaxsup(x+1,y) > highThresh || nomaxsup(x+1,y+1) > highThresh || nomaxsup(x,y+1) > highThresh || nomaxsup(x-1,y+1) > highThresh || nomaxsup(x+1,y-1) > highThresh)
%                     H(x,y) = 1;
%                 else 
%                     H(x,y) = 0;
%                 end;
%             end;
%         end;
%     end;
   
    J = H;
    theta = ((H > 0).*(theta)./2);
    
    