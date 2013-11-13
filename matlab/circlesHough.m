function [I, circles] = circlesHough(I)

c = findConnectedComponents(I);
figure(); imshow(c);