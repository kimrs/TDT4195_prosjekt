close('all');
% read in image from file
originalImg = imread('sweetsA03.png');

% normalize image to 0-1 values
originalImg = double(originalImg)/255.0;
figure('Name','Original Image','NumberTitle','off'); imshow(originalImg);

% normalize intensity

% convert to gray image values.
grayImg = rgb2gray(originalImg);
figure('Name','Gray Image','NumberTitle','off'); imshow(grayImg);

% find edges with canny edge detector
edges = edgeCanny(grayImg);
figure('Name','Edges from canny edge detector','NumberTitle','off'); imshow(edges);

% use hough transform to find circles
%[houghImg circles] = circlesHough(grayImg);
%call imfindcircles
[c,r] = imfindcircles(originalImg,[10,20]);
%display detected circles
figure('Name','Detected sweets','NumberTitle','off'); imshow(originalImg);viscircles(c,r);


% Draw circles for testing

% write coordinates for the circles.