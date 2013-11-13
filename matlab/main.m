close('all');
% read in image from file
originalImg = imread('sweetsA01.png');

% show original
figure('Name','Original image','NumberTitle','off');imshow(originalImg);

% scale image to 0-1 values
originalImg = double(originalImg)/255.0;

% do a threshold with moving average filter
bitmask = optimaThreshold(originalImg);

% create copy of original
maskedImage = originalImg;

% mask image (all colors)
for i = 1:1:3
    maskedImage(:,:,i) = double(originalImg(:,:,i).*bitmask);
end

% Closing and opening filters
bitmask = imclose(bitmask,strel('disk',1));
bitmask = imopen(bitmask,strel('disk',8));
bitmask = imopen(bitmask,strel('disk',8));
bitmask = imclose(bitmask,strel('disk',1));

figure('Name','Final masked image','NumberTitle','off');imshow(bitmask);

% show seperate RGB layers
% figure('Name','Channels','NumberTitle','off'); subplot(3,1,1); imshow(originalImg(:,:,1));
% subplot(3,1,2); imshow(originalImg(:,:,2));
% subplot(3,1,3); imshow(originalImg(:,:,3));

% convert to gray image values.
grayImg = rgb2gray(maskedImage);

figure('Name','Gray Image','NumberTitle','off'); imshow(grayImg);

% find sweets
[c,r] = imfindcircles(bitmask,[10,20]);

cc=1:80; 
rr=cc.';

cx1=40; cy1=40; R1=20;
cx2=10; cy2=10; R2=10;


f=@(xx,yy) (xx-cx1).^2+(yy-cy1).^2 <=R1^2 | (xx-cx2).^2+(yy-cy2).^2 <=R2^2 ; 
 

circ=bsxfun(f,rr,cc); %Logical map of 2 circles

C = circlesHough(double(circ),10,20);

%figure(); imshow(l);
%display detected circles
figure('Name','Detected sweets','NumberTitle','off'); imshow(originalImg);viscircles(c,r);

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

