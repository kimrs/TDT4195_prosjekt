function [ output_args ] = separateColors( img )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
[m, n, l] = size(img);

redImg      = img(:, :, 1);
greenImg    = img(:, :, 2);
blueImg     = img(:, :, 3);

[redImg, greenImg, blueImg] = normalizeColorValues(redImg,greenImg,blueImg);

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

only_green = only_green > -0.4;
only_blue = only_blue > -0.6;
only_red = only_red > 0.42;

 %only_blue   = medianFilter(only_blue, 4);
 only_red    = medianFilter(only_red, 4);
 only_green  = medianFilter(only_green, 4);

only_yellow = redImg - blueImg;
[max_yellow, ~] = max(only_yellow(:));
only_yellow = only_yellow.*(1/max_yellow);
only_yellow = only_yellow + greenImg;
[max_yellow, ~] = max(only_yellow(:));
only_yellow = only_yellow.*(1/max_yellow);

only_yellow = only_yellow > 0.9;


only_pink = redImg - greenImg;
[max_pink, ~] = max(only_pink(:));
only_pink = only_pink.*(1/max_pink);
only_pink = only_pink + blueImg;
[max_pink, ~] = max(only_pink(:));
only_pink = only_pink.*(1/max_pink);


only_pink = only_pink > 0.8;
only_pink   = medianFilter(only_pink, 10);

only_orange = redImg - blueImg;
only_orange = only_orange - only_green;
only_orange = only_orange - only_blue;
only_orange = only_orange - only_yellow;
only_orange = only_orange - only_red;
only_orange = only_orange - only_pink;

only_orange = only_orange > 0.45;


only_orange = medianFilter(only_orange, 8);
%only_yellow = medianFilter(only_yellow, 14);

% weird

only_red = imfill(only_red,'holes');
only_green = imfill(only_green,'holes');
only_blue = imfill(only_blue,'holes');
only_yellow = imfill(only_yellow,'holes');
only_pink = imfill(only_pink,'holes');
only_orange = imfill(only_orange,'holes');

 only_blue = only_blue & ~only_pink;
 only_pink = only_pink & ~only_red;
 only_green = only_green & ~only_yellow;
 
 
 only_red = openAndClose(only_red);
 only_green = openAndClose(only_green);
 only_blue = openAndClose(only_blue);
 only_yellow = openAndClose(only_yellow);
 only_pink = openAndClose(only_pink);
 only_orange = openAndClose(only_orange);




output_args = zeros(m, n, 6);

output_args(:,:, 1) = only_red;
output_args(:,:, 2) = only_green;
output_args(:,:, 3) = only_blue;
output_args(:,:, 4) = only_yellow;
output_args(:,:, 5) = only_pink;
output_args(:,:, 6) = only_orange;

end

