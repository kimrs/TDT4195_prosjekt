% returns the moving average

function J = movingAverage(I)
J = conv2(I, ones(3)/9, 'same');