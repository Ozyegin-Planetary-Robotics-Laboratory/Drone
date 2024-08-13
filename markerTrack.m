p = parrot(); % Connecting parrot drone
cameraObj = camera(p); % Connecting drone camera

arucoDict = arucoDictionary('4x4', 50);  % Example with a 4x4 dictionary of 50 markers
detectorParams = cameraParams; % Load default parameters for the detector
arucoDetector = arucoDetector(arucoDict, detectorParams);


% Loop for continuous video processing
while true
    % Capture a snapshot from the live stream
    img = snapshot(cameraObj);
    
    % Detect ArUco markers in the image
    [markerCorners, markerIds] = detectMarkers(arucoDetector, img);

    % If markers are detected, annotate the image
    if ~isempty(markerIds)
        img = insertMarker(img, markerCorners, 'Size', 10, 'Color', 'red');
        img = insertText(img, markerCorners(1,:,1), num2str(markerIds(1)), 'BoxOpacity', 1, 'FontSize', 16);
    end
    
    % Display the image with annotations
    imshow(img);
    title('ArUco Marker Tracking');
    
    % Pause to allow for continuous processing without overloading
    pause(0.1);
end



takeoff(p); % Takeoff command



