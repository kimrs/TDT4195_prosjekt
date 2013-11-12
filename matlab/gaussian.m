function J = gaussian(I,size,sigma)
    
    % create the gaussian
    [x, y] = meshgrid(round(-size/2):round(size/2), round(-size/2):round(size/2));
    g = exp(- (x.*x/(2 * sigma * sigma))-(y.^2/(2 * sigma * sigma)));
    g = g./sum(g(:));

    % apply it by convolution
    J = conv2(I,g,'same');