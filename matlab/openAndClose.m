function J = openAndClose(I)
    % Closing and opening filters, to remove some 
    I = imclose(I,strel('disk',1));
    I = imopen(I,strel('disk',8));
    I = imopen(I,strel('disk',8));
    J = imclose(I,strel('disk',1));