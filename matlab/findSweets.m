function [sweets, radius] = findSweets(I, m)
% to do: replace this
[sweets,radius] = imfindcircles(bitmask,m);