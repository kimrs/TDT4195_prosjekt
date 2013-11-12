function [x, y] = findGradients(I)
    h = [-1 0 1; -2 0 2; -1 0 1];
    v = [1 2 1; 0 0 0; -1 -2 -1];
    
    x = conv2(I,h,'same');
    y = conv2(I,v,'same');