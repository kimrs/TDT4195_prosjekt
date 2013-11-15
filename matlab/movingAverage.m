% returns the moving average

function J = movingAverage(I)
J = conv2(I, ones(5)/12, 'same');