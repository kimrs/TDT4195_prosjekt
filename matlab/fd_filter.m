function [ output_args ] = fd_filter( img, rad )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
[m, n] = size(img);
radius = m / 2 * rad;

if(m > n)
    img_kva = imresize(img, [m, m]);
else
    img_kva = imresize(img, [n, n]);
end

max_s = max(m, n);

x = meshgrid(1:max_s, 1:max_s) - (max_s + 1) / 2;
y = meshgrid(1:max_s, 1:max_s)'- (max_s + 1) / 2;

lowpass_filter = ( x.*x+y.*y <= radius^2 );
highpass_filter = ( x.*x + y.*y > radius^2 );
img_highpass = uint8(highpass_filter * 255);

F = fft2(img_kva, max_s, max_s);

G = fftshift(highpass_filter).*F;
img_hp_filtered = ifft2(G);
img_hp_filtered = imresize(img_hp_filtered, [m, n]);

G = fftshift(lowpass_filter).*F;
img_lp_filtered = ifft2(G);
img_lp_filtered = imresize(img_lp_filtered, [m, n]);

% subplot(1, 3, 1)
% imshow(img);
% title('orig');

%subplot(1, 3, 2);
%imshow(img_highpass)
%title('filter');
% 
% subplot(1, 3, 2);
% imshow(img_hp_filtered)
% title('high pass');
% 
% subplot(1, 3, 3);
% imshow(img_lp_filtered)
% title('low pass');

output_args = img_lp_filtered;

end

