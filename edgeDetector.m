function edgeDetection(imgFile)
    
    img = imread(imgFile);
    colorTransform = makecform('srgb2lab');
    img = double(applycform(img, colorTransform));
    
    % Find x and y derivatives of the gaussian
    [hx, hy] = gradient(fspecial('gaussian',[5 5],1.0));
    gx = double(imfilter(img,hx,'replicate', 'conv'));
    gy = double(imfilter(img,hy,'replicate', 'conv'));
    
    % absolute value
    mag = sqrt((gx .* gx) + (gy .* gy));

    % Non-maxima supression
    [mag, ] = max(sqrt(mag), [], 3);
    mag = double(mag) / 20.0;
    
    imshow(mag) 
end