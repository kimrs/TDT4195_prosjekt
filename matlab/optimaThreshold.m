function bitmask = optimaThreshold(I)

% get each color plane and do a moving average
img_red = movingAverage(I(:,:,1));
img_green = movingAverage(I(:,:,2));
img_blue = movingAverage(I(:,:,3));

% get mean values
mean_red = mean(img_red(:));
mean_green = mean(img_green(:));
mean_blue = mean(img_blue(:));

% get standard deviations
std_red = std(img_red(:));
std_green = std(img_green(:));
std_blue = std(img_blue(:));

% vector Normalize
img_red = (img_red - mean_red) / std_red;
img_green = (img_green - mean_green) / std_green;
img_blue = (img_blue - mean_blue) / std_blue;

% multivariable thresholding
d = sqrt((img_red - mean_red).^2 + (img_green - mean_green).^2 + (img_blue - mean_blue).^2);

bitmask = d > 1.9;

% fill holes in circles
bitmask = imfill(bitmask,'holes');