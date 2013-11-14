function [R, G, B] = normalizeColorValues(R,G,B)
    R = R./sqrt(R.^2 + G.^2 + B.^2);
    G = G./sqrt(R.^2 + G.^2 + B.^2);
    B = B./sqrt(R.^2 + G.^2 + B.^2);
    
    figure();imshow(cat(3,R,G,B));