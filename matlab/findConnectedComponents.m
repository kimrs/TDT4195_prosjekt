% Used the algorithm described here:
% http://www.cis.rit.edu/class/simg782.old/finding_connected_components.pdf

function C = findConnectedComponents(I)
    figure();imshow(I);
    C = zeros(size(I));
    L = 1;
    
    % First pixel
    if(I(1,1)>0)
        L = L + 1;
        C(1,1) = L;
    end;
    
    % first row
    for x = 2:size(I,1)
        if(I(x,1)>0)
            if(I(x,1)==I(x-1,1))
                C(x,1) = C(x-1,1);
            else
                L = L + 1;
                C(x,1) = L;
            end;
        end;
    end;
    
    memsize = 1000;
    LC = zeros(memsize,2);
    count = 1;
    
    for y = 2:size(I,2)
        % first column
        if(I(1,y)>0)
            if(I(1,y)==I(1,y-1))
                C(1,y) = C(1,y-1);
            else
                L = L + 1;
                C(1,y) = L;
            end;
        end;
        
        % rest of the pixels
        for x = 2:size(I,1)
            if(I(x,y)>0)
                if(I(x,y)==I(x-1,y) && I(x,y)~=I(x,y-1))
                    C(x,y) = C(x-1,y);
                end;
                if(I(x,y)~=I(x-1,y) && I(x,y)==I(x,y-1))
                    C(x-1,y) = C(x,y-1);
                end;
                if(I(x,y)~=I(x-1,y) && I(x,y)~=I(x,y-1))
                    L = L + 1;
                    C(x,y) = L;
                end;
                if(I(x,y)==I(x-1,y) && I(x,y)==I(x,y-1))
                    if(C(x-1,y)==C(x,y-1))
                        C(x,y) = C(x,y-1);
                    else
                        if(C(x-1,y)<C(x,y-1))
                            C(x,y) = C(x,y-1);
                            LC(count,1) = C(x,y-1);
                            LC(count,2) = C(x-1,y);
                            count = count + 1;
                        else
                            C(x,y) = C(x-1,y);
                            LC(count,1) = C(x-1,y);
                            LC(count,2) = C(x,y-1);
                            count = count + 1;
                        end
                        if (count + 10 > memsize)
                            memsize = memsize + 1000;
                            LC(memsize,2) = 0;
                        end;
                    end;
                end;
            end;
        end; 
    end;
    
    figure('Name','BEFORE','NumberTitle','off');imshow(C);
    
    % iterate and fix the components which is not already connected
    for x = 1:size(C,1)
        for y = 1:size(C,2)
            for i = 1:size(LC,1)
                if(LC(i,2)==0)
                    break;
                end;
                if(C(x,y)==LC(i,1))
                    C(x,y) = LC(i,2);
                end;
            end;
        end;
    end;