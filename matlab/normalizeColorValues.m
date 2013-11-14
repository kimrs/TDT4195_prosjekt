% This function normalizes the colors, so its easier to
% overcome those shadow-related problems.
function [R, G, B] = normalizeColorValues(R,G,B)
    R = R./sqrt(R.^2 + G.^2 + B.^2);
    G = G./sqrt(R.^2 + G.^2 + B.^2);
    B = B./sqrt(R.^2 + G.^2 + B.^2);