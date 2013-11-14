function J = openAndClose(I)
    % Closing and opening filters, to remove some 
    I = imclose(I,strel('disk',1));
    I = imopen(I,strel('disk',10));
    I = imopen(I,strel('disk',10));
    J = imclose(I,strel('disk',1));