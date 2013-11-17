close('all');

% the file which we process
fileToLoad = 'sweetsA03';

% read in image from file
originalImg = imread([fileToLoad '.png']);

% scale image to 0-1 values
originalImg = double(originalImg)/255.0;

% Get the separate color masks.
seperateColoredObjects = separateColors(originalImg);

red_sweets = seperateColoredObjects(:,:,1);
green_sweets = seperateColoredObjects(:,:,2);
blue_sweets = seperateColoredObjects(:,:,3);
yellow_sweets = seperateColoredObjects(:,:,4);
pink_sweets = seperateColoredObjects(:,:,5);
orange_sweets = seperateColoredObjects(:,:,6);

% merge all maskes to one complete mask
bitmask = red_sweets | green_sweets | blue_sweets | yellow_sweets | pink_sweets | orange_sweets;

% create copy of original
maskedImage = originalImg;

% mask image (all colors)
for i = 1:1:3
    maskedImage(:,:,i) = double(originalImg(:,:,i).*bitmask);
end

% find sweets in every color
[red_c,red_r] = imfindcircles(red_sweets,[3,23]);
[green_c,green_r] = imfindcircles(green_sweets,[2,22]);
[blue_c,blue_r] = imfindcircles(blue_sweets,[3,23]);
[yellow_c,yellow_r] = imfindcircles(yellow_sweets,[3,23]);
[pink_c,pink_r] = imfindcircles(pink_sweets,[3,23]);
[orange_c,orange_r] = imfindcircles(orange_sweets,[2,22]);

% self implemented hough
%C = circlesHough(double(bitmask),3,23);

%display detected circles
f = figure('Name','Detected sweets','NumberTitle','off','visible','off'); imshow(originalImg);
viscircles(red_c,red_r,'EdgeColor','r');
viscircles(green_c,green_r,'EdgeColor','g');
viscircles(blue_c,blue_r,'EdgeColor','b');
viscircles(yellow_c,yellow_r,'EdgeColor','yellow');
viscircles(pink_c,pink_r,'EdgeColor',[1,0.4,0.6]);
viscircles(orange_c,orange_r,'EdgeColor',[1,0.56,0]);
detectedSweets = getframe(f);

% Write images to files.
% imwrite(originalImg,['results/' fileToLoad '_original.png']);
% imwrite(maskedImage,['results/' fileToLoad '_masked.png']);
% imwrite(bitmask,['results/' fileToLoad '_total_mask.png']);
% imwrite(detectedSweets.cdata,['results/' fileToLoad '_detected_sweets.png']);
% imwrite(red_sweets,['results/' fileToLoad '_red_sweets.png']);
% imwrite(green_sweets,['results/' fileToLoad '_green_sweets.png']);
% imwrite(blue_sweets,['results/' fileToLoad '_blue_sweets.png']);
% imwrite(yellow_sweets,['results/' fileToLoad '_yellow_sweets.png']);
% imwrite(pink_sweets,['results/' fileToLoad '_pink_sweets.png']);
% imwrite(orange_sweets,['results/' fileToLoad '_orange_sweets.png']);

% write coordinates for the circles to file
% colorstate, radius, x-pos, y-pos
%fileName = [fileToLoad '_test.txt'];
fileName = 'test.txt';

fileId = fopen(fileName,'w');
if fileId ~= -1
    for i = 1:size(red_c,1)
      fprintf(fileId,'%d%d%4.4d%4.4d\r\n',1,int16(red_r(i,1)),int16(red_c(i,1)),int16(red_c(i,2)));
    end;
    for i = 1:size(green_c,1)
      fprintf(fileId,'%d%d%4.4d%4.4d\r\n',2,int16(green_r(i,1)),int16(green_c(i,1)),int16(green_c(i,2)));
    end;
    for i = 1:size(blue_c,1)
      fprintf(fileId,'%d%d%4.4d%4.4d\r\n',3,int16(blue_r(i,1)),int16(blue_c(i,1)),int16(blue_c(i,2)));
    end;
    for i = 1:size(yellow_c,1)
      fprintf(fileId,'%d%d%4.4d%4.4d\r\n',4,int16(yellow_r(i,1)),int16(yellow_c(i,1)),int16(yellow_c(i,2)));
    end;
    for i = 1:size(pink_c,1)
      fprintf(fileId,'%d%d%4.4d%4.4d\r\n',5,int16(pink_r(i,1)),int16(pink_c(i,1)),int16(pink_c(i,2)));
    end;
    for i = 1:size(orange_c,1)
      fprintf(fileId,'%d%d%4.4d%4.4d\r\n',6,int16(orange_r(i,1)),int16(orange_c(i,1)),int16(orange_c(i,2)));
    end;
    fclose(fileId);
end

