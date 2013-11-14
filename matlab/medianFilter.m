% Remove some noise with a median filter. SLOW!
function [ output_args ] = medianFilter( img, filterSize )
   [m, n] = size(img);
   output_args = zeros(m, n);
   l = floor(filterSize / 2);
   for x = 1 + l: n - l
       for y = 1 + l: m - l
           set = img(y - l:y + l, x - l: x + l);
           output_args(y, x) = median(set(:));
       end
   end
end

