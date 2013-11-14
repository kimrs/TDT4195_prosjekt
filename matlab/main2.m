close('all');

% the file which we process
fileToLoad = 'sweetsA03';

% read in image from file
originalImg = imread([fileToLoad '.png']);

% scale image to 0-1 values
originalImg = double(originalImg)/255.0;

%normalizedColors = optimaThreshold(originalImg);

seperateColoredObjects = separateColors(originalImg);

red_sweets = seperateColoredObjects(:,:,1);
green_sweets = seperateColoredObjects(:,:,2);
blue_sweets = seperateColoredObjects(:,:,3);
yellow_sweets = seperateColoredObjects(:,:,4);
pink_sweets = seperateColoredObjects(:,:,5);
orange_sweets = seperateColoredObjects(:,:,6);

red_sweets = imfill(red_sweets,'holes');
green_sweets = imfill(red_sweets,'holes');
blue_sweets = imfill(blue_sweets,'holes');
yellow_sweets = imfill(yellow_sweets,'holes');
pink_sweets = imfill(pink_sweets,'holes');
orange_sweets = imfill(orange_sweets,'holes');

% merge all maskes to one complete mask
bitmask = red_sweets | green_sweets | blue_sweets | yellow_sweets | pink_sweets | orange_sweets;

% 
% % do a threshold with moving average filter
% bitmask = optimaThreshold(originalImg);
% 
% % Closing and opening filters, to remove some 
% bitmask = imclose(bitmask,strel('disk',1));
% bitmask = imopen(bitmask,strel('disk',8));
% bitmask = imopen(bitmask,strel('disk',8));
% bitmask = imclose(bitmask,strel('disk',1));

% create copy of original
maskedImage = originalImg;

% mask image (all colors)
for i = 1:1:3
    maskedImage(:,:,i) = double(originalImg(:,:,i).*bitmask);
end

% find sweets
[c,r] = imfindcircles(bitmask,[10,30]);

% cc=1:80; 
% rr=cc.';
% 
% cx1=40; cy1=40; R1=20;
% 
% 
% f=@(xx,yy) (xx-cx1).^2+(yy-cy1).^2 <=R1^2; 
%  
% 
% circ=bsxfun(f,rr,cc); %Logical map of 2 circles

%C = circlesHough(double(circ),20,21);

C = circlesHough(double(bitmask),10,25);
%figure(); imshow(l);
%display detected circles
figure('Name','Detected sweets','NumberTitle','off'); imshow(originalImg);viscircles(c,r);

% Write images to files.
imwrite(originalImg,['results/' fileToLoad '_original.png']);
imwrite(maskedImage,['results/' fileToLoad '_masked.png']);
imwrite(bitmask,['results/' fileToLoad '_total_mask.png']);
imwrite(red_sweets,['results/' fileToLoad '_red_sweets.png']);
imwrite(green_sweets,['results/' fileToLoad '_green_sweets.png']);
imwrite(blue_sweets,['results/' fileToLoad '_blue_sweets.png']);
imwrite(yellow_sweets,['results/' fileToLoad '_yellow_sweets.png']);
imwrite(pink_sweets,['results/' fileToLoad '_pink_sweets.png']);
imwrite(orange_sweets,['results/' fileToLoad '_orange_sweets.png']);

% write coordinates for the circles to file
% colorstate, radius, x-pos, y-pos
fileName = 'test.txt';
fileId = fopen(fileName,'w');
if fileId ~= -1
    for i = 1:size(c,1)
      fprintf(fileId,'%d%d%4.4d%4.4d\r\n',1,int16(r(i,1)),int16(c(i,1)),int16(c(i,2)));
    end;
    fclose(fileId);
end

