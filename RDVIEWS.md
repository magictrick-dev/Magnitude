# RDView Script Files

The rdview script files are a derivation of rdview files designed by Dr. Duffin
at Northern Illinois University with some modifications made to work in-context
with this application. The scripting language is designed to be simple and imperative.
The file format is raw-text, with the extension `.rd`. All keywords in rdview are case-sensitive.
The language ignores white space, but the commands must be well-formed. Depending on
where the error occured, the parser will refuse to generate the scene, or certain frames
will not render for you until they are corrected. Generally speaking, errors are easily
caught and shown to the user much like any compiler provides errors.

# Markup Headers

All rdview files must have a correctly formatted markup header that explains how
the scene and subsequent frames should be handled. The most important is the `display`
markup header which dictates how the rdview scene should look like. If the display header
is not present, no frames are generated and an appropriate error is shown. A single
frame is required to be defined in order for the scene viewer to become active. A typical,
barebones document would look like this:

```
# The required display header. Runs at 24fps with a resolution of 1920x1080.
Display "Example" 24 1920 1080

# The first frame.
FrameBegin 1
    CameraEye -5.0 -6.0 7.0
    CameraAt 1.0 1.0 -1.0
    CameraUp 0 0 1
    CameraFOV  30.0
    
    # This is what will be rendered.
    WorldBegin

        # A cube is a primitive, built-in geometry.
        Cube

    WorldEnd
FrameEnd
```

As you can see, setup is fairly straightforward and doesn't require a lot of work to
get started with rendering. Magnitude provides geometric primitives as well as a system
to create polygon sets. In most cases, you probably aren't interested in geometric primitives
and would like to render actual objects, which is what the object loader is designed to do.

**Comments begin with `#` and extend until end-of-line.**

Despite the formatting of the above document, *tab-spacing is not required*, it's only done
to visual break up different elements of the frame so that it is easier to see. One final note
to make is that there is not "include" system yet. The original specification did not have this.
So yes, if you intend to go crazy with this, text files will be quite expensive to parse.

### `Display "[name]" [frame-rate] [width] [height]`

>   The display parameter must appear as the first markup parameter in your document.
>   If the display parameter is not present, the rdview file can not render *any* frames
>   listed in the document. The `name` element is the title of your project. This can be
>   any valid name, in quotes. For now, this value is reserved. The `frame-rate` option
>   sets the output frame rate of your project. This does not correlate to the frame rate
>   of the live viewport (generally that just runs at whatever the application is running at).
>   Finally, the `width` and `height` parameters correspond to the frame size of the output.
>   Once again, this doesn't correspond to the live viewport which can be any size.

### `FrameBegin [number]`, `FrameEnd`

>   Each frame must be enclosed with a frame begin and end tags. Each frame must be given
>   a number. The number of the frame is used to determine which frame it is in the sequence.
>   Frames don't necessarily need to come in order.

### `WorldBegin`, `WorldEnd`

>   Before you can place objects in a scene, you must enclose them in world tags. World tags
>   enclose entities in the scene and compose the world within a frame. You can think of it
>   like a static list of all the entities in the frame.

### `CameraEye [x] [y] [z]`

>   The camera eye position places the camera within the world. This is a frame-local property
>   that can only be set between `FrameBegin` and `WorldBegin`. The default value of this
>   is (0, 0, 0), at the origin.

### `CameraAt [x] [y] [z]`

>   The camera at defines what the camera is pointing at. This is a frame-local property
>   that can only be set between `FrameBegin` and `WorldBegin`. The default value of this
>   is (0, 0, -1), looking down at the -z axis.

### `CameraUp [x] [y] [z]`

>   The camera at defines what is considered "up" to the camera. This is a frame-local
>   property that can only be set between `FrameBegin` and `WorldBegin`. The default
>   value of this is (0, 1, 0), making the +y axis the up coordinate.

### `CameraFOV [angle:degrees]`

>   The camera field of view defines the field of view of the camera. This is a frame-local
>   property that can only be set between `FrameBegin` and `WorldBegin`. The default
>   value of this is 90 degrees.

### `Clipping [near] [far]`

>   The clipping value determines how far things are before they're clipped between
>   the relative camera "near", and the camera "far" distance. THis is a frame-local
>   property that can only be set between `FrameBegin` and `WorldBegin`. The default
>   value of this is 1.0 for near, 1.0e+9 for far.

# Primitive Geometry

Primitive geometry allows you to start rendering directly to the screen without providing your
own external 2D/3D assets. Most of these are holdovers from the original language specification
and may provide useful in certain contexts while designing your scenes.

### `Point [x] [y] [z] [radius]`

>   Draws a single point on the screen at the x, y, z coordinates specified. The radius
>   determines how big that point should be. Generally, drawing points aren't super useful
>   and are a holdover from the original language specification. They're cool if you want to
>   play around, though.

**Documentation Last Updated:** 12/01/2024
