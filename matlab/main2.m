close('all');
% read in image from file
originalImg = imread('sweetsA03.png');

% scale image to 0-1 values
originalImg = double(originalImg)/255.0;

redImg      = originalImg(:, :, 1);
greenImg    = originalImg(:, :, 2);
blueImg     = originalImg(:, :, 3);

windowSize = 5;
h1 = ones(windowSize, 1)/windowSize;

redImgc = fd_filter(redImg, 0.01);

greenImgc = fd_filter(greenImg, 0.01);

blueImgc = fd_filter(blueImg, 0.01);

shade_gradient = redImgc + greenImgc + blueImgc;
shade_gradient = shade_gradient ./ 3;
[m, n] = size(shade_gradient);
o = ones(m, n);
shade_gradient = o - shade_gradient;

% blueImg = blueImg   .* shade_gradient;
% redImg = redImg     .* shade_gradient;
% greenImg = greenImg .* shade_gradient;

% subplot(1, 3, 1);
% imshow(redImg);
% subplot(1, 3, 2);
% imshow(redImgc);
% subplot(1, 3, 3);
% imshow(redImgf);

%greenImgc = movingAverage(greenImg);
%blueImgc = movingAverage(blueImg);

%show original
%figure('Name','Original image','NumberTitle','off');imshow(blueImg);

only_red = redImg - greenImg.*4;

only_green = greenImg;
only_green = only_green - redImg;
[max_green, ~] = max(only_green(:));
only_green = only_green.*(1/max_green);
only_green = only_green - blueImg;
[max_green, ~] = max(only_green(:));
only_green = only_green.*(1/max_green);

only_blue = blueImg;
only_blue = only_blue - greenImg;
[max_blue, ~] = max(only_blue(:));
only_blue = only_blue.*(1/max_blue);
only_blue = only_blue - redImg;
[max_blue, ~] = max(only_blue(:));
only_blue = only_blue.*(1/max_blue);

only_yellow = redImg - blueImg;
[max_yellow, ~] = max(only_yellow(:));
only_yellow = only_yellow.*(1/max_yellow);
only_yellow = only_yellow + greenImg;
[max_yellow, ~] = max(only_yellow(:));
only_yellow = only_yellow.*(1/max_yellow);

only_pink = redImg - greenImg;
[max_pink, ~] = max(only_pink(:));
only_pink = only_pink.*(1/max_pink);
only_pink = only_pink + blueImg;
[max_pink, ~] = max(only_pink(:));
only_pink = only_pink.*(1/max_pink);

only_green = only_green > 0.1;
only_blue = only_blue > 0.18;
only_red = only_red > 0.1;
only_yellow = only_yellow > 0.8;
only_pink = only_pink > 0.8;

only_orange = redImg - blueImg;
only_orange = only_orange - only_green;
only_orange = only_orange - only_blue;
only_orange = only_orange - only_yellow;
only_orange = only_orange - only_red;
only_orange = only_orange - only_pink;

only_orange = only_orange > 0.54;

% only_blue   = medianFilter(only_blue, 8);
% only_red    = medianFilter(only_red, 6);
% only_green  = medianFilter(only_green, 4);
% only_orange = medianFilter(only_orange, 14);
% only_pink = medianFilter(only_pink, 10);

subplot(3, 2, 1);
imshow(only_red);
title('red');

subplot(3, 2, 2);
imshow(only_green);
title('green');

subplot(3, 2, 3);
imshow(only_blue);
title('blue');

subplot(3, 2, 4);
imshow(only_yellow);
title('yellow');

subplot(3, 2, 5);
imshow(only_pink);
title('pink');

subplot(3, 2, 6);
imshow(only_orange);
title('orange');


% subplot(3, 2, 1);
% imshow(redImg);
% title('red');
% 
% subplot(3, 2, 3);
% imshow(greenImg);
% title('green');
% 
% subplot(3, 2, 5);
% imshow(blueImg);
% title('blue');
% 
% subplot(3, 2, 2);
% imshow(shade_gradient);
% title('shade');
% 
% subplot(3, 2, 4);
% imshow(greenImgc);
% title('green');
% 
% subplot(3, 2, 6);
% imshow(blue_imgc);
% title('blue');
















