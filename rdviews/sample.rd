# Sample is the base test project.
# Runs at 24 frames per second at 1280 by 720 res.
Display "Sample" 24 1280 720

FrameBegin 1

    # Camera settings for the frame.
    CameraEye 320 240 -240
    CameraAt  320 240 0
    CameraUp  0.0 -1.0 0.0
    CameraFOV 90

    # Begin the world.
    WorldBegin
        
        # Display a single point at (300,300,0) with radius 1.
        Point 300 300 0 1

    WorldEnd
FrameEnd
