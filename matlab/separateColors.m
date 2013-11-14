function [ output_args ] = separateColors( img )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
[m, n, l] = size(img);

redImg      = img(:, :, 1);
greenImg    = img(:, :, 2);
blueImg     = img(:, :, 3);

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

only_blue   = medianFilter(only_blue, 8);
only_red    = medianFilter(only_red, 6);
only_green  = medianFilter(only_green, 4);
only_orange = medianFilter(only_orange, 14);
only_pink   = medianFilter(only_pink, 10);



output_args = zeros(m, n, 6);

output_args(:,:, 1) = only_red;
output_args(:,:, 2) = only_green;
output_args(:,:, 3) = only_blue;
output_args(:,:, 4) = only_yellow;
output_args(:,:, 5) = only_pink;
output_args(:,:, 6) = only_orange;

end

