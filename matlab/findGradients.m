function [x, y] = findGradients(I)
    w = [-1 0 1; -2 0 2; -1 0 1];
    h = [1 2 1; 0 0 0; -1 -2 -1];
    
    x = conv2(I,w,'same');
    y = conv2(I,h,'same');