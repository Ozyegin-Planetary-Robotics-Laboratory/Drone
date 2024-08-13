parrotObj = parrot();
cameraObj = camera(parrotObj);

arucoDict = arucoDictionary('4x4', 50);
detectorParams = arucoDetectorParameters;
arucoDetector = arucoDetector(arucoDict, detectorParams);

forwardPitch = deg2rad(-5);  % Pitch to move forward (negative for forward movement)
rotationSpeed = deg2rad(5);  % Rotation speed for aligning with the marker
markerSizeThreshold = 50;    % Minimum marker size (in pixels) to consider close enough
centerXThreshold = 20;       % Margin in pixels for centering the marker horizontally

% Takeoff
takeoff(parrotObj);

% Main control loop
while true
    % Capture a snapshot from the live stream
    img = snapshot(cameraObj);
    
    % Detect ArUco markers in the image
    [markerCorners, markerIds] = detectMarkers(arucoDetector, img);

    if ~isempty(markerIds)
        % Calculate the center of the detected marker
        markerCenter = mean(markerCorners, 1);

        % Calculate the size of the marker (average of width and height in pixels)
        markerSize = mean(sqrt(sum(diff(markerCorners).^2, 2)));

        % Calculate the horizontal offset from the center of the image
        imgCenterX = size(img, 2) / 2;
        offsetX = markerCenter(1, 1) - imgCenterX;

        % Control logic to fly towards the marker
        if abs(offsetX) > centerXThreshold
            % Rotate to align with the marker
            if offsetX > 0
                move(parrotObj, 'RotationSpeed', rotationSpeed);
            else
                move(parrotObj, 'RotationSpeed', -rotationSpeed);
            end
        elseif markerSize < markerSizeThreshold
            % Move forward if the marker is too small (far away)
            move(parrotObj, 'Pitch', forwardPitch);
        else
            % If marker is centered and close enough, stop moving (the drone will hover automatically)
            break;  % Exit the loop if desired
        end
    else
        % If no marker is detected, maintain current position (hover)
        % No need to explicitly hover; just don't issue movement commands.
    end

    % Pause to allow for continuous processing without overloading
    pause(0.1);
end

% Land the drone once the operation is complete
land(parrotObj);

