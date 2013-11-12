function J = edgeCanny(I)
    
    
    % Smooth image
    J = gaussian(I,7,2.5);
    
    % Find gradients
    %L = findGradients(J);
    [Dx, Dy] = findGradients(J);
    
    figure('Name','Horizontal gradients','NumberTitle','off'); imshow(Dx);
    figure('Name','Vertical gradients','NumberTitle','off'); imshow(Dy);
    
    % Combine both x and y direction (get the magnitude)
    % for faster computation use this
    % aprox_magnitude = abs(Dx)+abs(Dy);
    
    magnitude = sqrt(Dx.^2+Dy.^2);
    % The direction of edges
    theta = atand(Dy./Dx);
    
    figure('Name','Theta','NumberTitle','off'); imshow(theta);
    
    nomaxsup = zeros(size(J,1),size(J,2));
    
    % Non maximum supression
    % Only local maxima should be marked as edges
    % Thresholding
    
    for x = 2:size(nomaxsup,1)
        for y = 2:size(nomaxsup,2)
            
            % Putting each angle inside their respective quadrant
            % and do a non-maximum suppression
       
            if ((theta(x,y) > 0 && theta(x,y) < 22.5) || (theta(x,y) > 157.5) && (theta(x,y) < -157.5))
                if (y - 1 > 0 && y + 1 < size(nomaxsup,2))
                    if (magnitude(x, y) > magnitude(x, y-1) && magnitude(x, y) > magnitude(x, y+1))
                        nomaxsup(x,y) = magnitude(x,y);
                    else
                        nomaxsup(x,y) = 0;
                    end;
                end;
            end;

            if ((theta(x,y) > 22.5) && (theta(x,y) < 67.5) || (theta(x,y) < -112.5) && (theta(x,y) > -157.5))
                if (y - 1 > 0 && y + 1 < size(nomaxsup,2) && x - 1 > 0 && x + 1 < size(nomaxsup,1))
                    if (magnitude(x, y) > magnitude(x+1, y-1) && magnitude(x, y) > magnitude(x-1, y+1))
                        nomaxsup(x,y) = magnitude(x,y);
                    else
                        nomaxsup(x,y) = 0;
                    end;
                end;
            end;

            if ((theta(x,y) > 67.5 && theta(x,y) < 112.5) || (theta(x,y) < -67.5 && theta(x,y) > 112.5))
                if (x - 1 > 0 && x + 1 < size(nomaxsup,1))
                    if (magnitude(x, y) > magnitude(x-1, y) && magnitude(x, y) > magnitude(x+1, y))
                        nomaxsup(x,y) = magnitude(x,y);
                    else
                        nomaxsup(x,y) = 0;
                    end;
                end;
            end;

            if ((theta(x,y) > 112.5 && theta(x,y) <= 157.5) || (theta(x,y) < -22.5 && theta(x,y) > -67.5))
                if (y - 1 > 0 && y + 1 < size(nomaxsup,2) && x - 1 > 0 && x + 1 < size(nomaxsup,1))
                    if (magnitude(x, y) > magnitude(x-1, y-1) && magnitude(x, y) > magnitude(x+1, y+1))
                        nomaxsup(x,y) = magnitude(x,y);
                    else
                        nomaxsup(x,y) = 0;
                    end;
                end;
            end;
        end
    end
    
    highThresh = 0.15;
    lowThresh = 0.05;
    
    out = zeros(size(nomaxsup,1),size(nomaxsup,2));
    
    % Hysteresis and thresholding
    for x = 2:size(nomaxsup,1)
        for y = 2:size(nomaxsup,2)
            if(nomaxsup(x,y) > highThresh)
                out(x,y) = 1;
            else
                if(nomaxsup(x,y) >= lowThresh && nomaxsup(x,y) <= highThresh )
                    out(x,y) = 1;
                else
                    if (nomaxsup(x,y) < lowThresh)
                        out(x,y) = 0;
                    end
                end
            end
            %if theta is blablabla?
            if (x-1 > 0 && y-1 > 0 && y+1 < size(nomaxsup,2) && x+1 < size(nomaxsup,1))
                if (nomaxsup(x-1,y-1) > highThresh || nomaxsup(x,y-1) > highThresh || nomaxsup(x+1,y-1) > highThresh || nomaxsup(x+1,y) > highThresh || nomaxsup(x+1,y+1) > highThresh || nomaxsup(x,y+1) > highThresh || nomaxsup(x-1,y+1) > highThresh || nomaxsup(x-1,y) > highThresh)
                    out(x,y) = 1;
                else 
                    out(x,y) = 0;
                end;
            end;
        end
    end
    
    J = out;