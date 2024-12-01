# Magnitude Graphics Visualization Utility

Magnitude is a computer graphics visualization utility with a built-in scripting
engine for designing and creating real-time scenes, model loading system, and a
custom software renderer for developing and testing computer graphics concepts.
The main idea of the utility is to provide a complete tool chain for creating and
rendering scenes using a well-defined scripting language.

The goal of this project is to create frame-by-frame, high resolution renders of
scenes. These scenes are highly scriptable, using an intermediate programmable
scripting language called "rdview" to generate, place, and transform models and
lights in a scene. (The language itself is a scripting language used at Northern
Illinois University, by Professor Duffin.) As you edit the scene file, the scene
will dynamically update on screen. If you edit the scene in the editor, the script
also dynamically updates for you.

### Using Magnitude

**NOTE:** The following documentation is the theory of *how* Magnitude will work,
not how it works *currently*. Magnitude is in alpha stages of development and many
features may not exist or work properly.

**[Complete Render View Documentation](./RDVIEWS.md)**

Magnitude is both a CLI and GUI tool. If you invoke `magnitude ./rdviews/sample.rd`, it will
open the project to that scene. Otherwise, opening it will bring up the editor (just like
any editor you've ever used before) and you can manually open a scene file there.

A scene file is your save file. The scene file must be parsed, constructed, and
dynamically rendered to the screen. For this reason, large scene files will take
longer to parse and load your scenes. It is for this reason that the editor only
loads one frame at a time. Each frame is composed of transforms, objects, lights,
and a camera. Each entity is shown in the inspector with its appropriate transform.
You can freely adjust the transform of the frame to adjust how it looks in the frame.
Afterwards, you can commit this back to the script.

Audio can be added to a scene. Audio files, like entities, have transforms. The
transform is relative to the camera, where (0, 0, 0) is "background music", and
any spatial adjustments from the camera adjust where it can be heard. Properties
such as volume and pitch can be editted. The amount of audio that is sampled is
dependent on the frames-per-second attribute of the scene. Magnitude will automatically
account for how much to sample for you based on this.

### Building Magnitude

Since Magnitude is still a work in progress, you will need to build this project
from source. This project currently only supports Windows and modern OpenGL (4.3+).

1. Configure the project.

    ```
    cmake -B ./comp
    ```

    This will create the required build files needed for your current platform. For
    Windows, you will need [Visual Studio's C/C++ MSVC Compiler](https://visualstudio.microsoft.com/#vs-section)
    in order to do this. On most UNIX platforms, [the GCC compiler is installed by default](https://gcc.gnu.org/).

2.  Build the project.

    ```
    cmake --build ./comp
    ```

    Once the project is built, you will find the binary file in `./bin` directory.
    Since this is both a CLI utility, you will need to append the `--free-form`
    flag to load the GUI application with a minimum rdview file that you can edit.
    For more advanced users, you can just supply the `.rdview` file as a parameter
    and it will render the scene for you.

### Project Features

The following list of features outline what Magnitude supports/will support:

- [X] Window System: Launching
- [X] Window System: OpenGL Support
- [ ] Window System: Non-blocking Fix (Potentially conflicts with ImGUI)
- [X] Vendor: Dear ImGUI
- [ ] Vendor: ASSIMP
- [X] Vendor: STBImage
- [X] Resource Management System: Handles
- [X] Resource Management System: File Loading
- [X] Resource Management System: File Saving
- [ ] Resource Management System: File Streaming
- [X] Input System: Keyboard
- [X] Input System: Mouse
- [ ] Input System: Controller
- [X] RDView Parser: Tokenizer
- [X] RDView Parser: Parser
- [ ] RDView Parser: Dynamic Parsing
- [X] RDView Parser: Language Documentation
- [ ] RDView Parser: Editor Highlighting
- [X] User Interface: Text Editor Layout
- [ ] User Interface: Frame Explorer
- [ ] User Interface: Object List
- [X] User Interface: Object Inspector
- [X] User Interface: Runtime Information
- [ ] User Interface: Asset Explorer
- [X] User Interface: Console
- [X] User Interface: Metrics (Frame Time)
- [ ] User Interface: Metrics (Memory Useage)
- [ ] User Interface: Metrics (Runtime Statistics)
- [X] Memory Management: Stack Allocator
- [ ] Memory Management: Dynamic Allocator
- [ ] Memory Management: Context Settings
- [ ] Memory Management: Tracking
- [ ] Modeling: 2D Primitives (Points and lines)
- [ ] Modeling: 3D Primitives (Cubes, spheres, cylinders, and cones)
- [ ] Modeling: Object File Loading (.obj in particular)
- [ ] Renderer: Point Drawing
- [ ] Renderer: Line Drawing
- [ ] Renderer: Common Attributed Points
- [ ] Renderer: 3D Rendering
- [ ] Renderer: Lighting
- [ ] Renderer: Texturing
- [X] Renderer: Shader Loader System (Vert. & Frag.)
- [ ] Renderer: 3D Rendering
- [ ] Renderer: Lighting & Texturing
- [ ] Renderer: Pipelining
- [X] Behavioral: Application Icon
- [X] Behavioral: Custom Dear ImGUI Font
- [X] Behavioral: Custom Dear ImGUI UI Color Scheme
- [X] Mathematics Library: Vectors (2D, 3D, 4D Homogenous)
- [X] Mathematics Library: Matrix (4D)
- [X] Mathematics Library: Vector Operations
- [X] Mathematics Library: Matrix Operations
- [ ] Mathematics Library: Common Matrix Transforms (Projections, Rotations, etc.)
- [ ] Video Rendering: FFMPEG Support
- [ ] Video Rendering: Scrubable Timeline Interface
- [ ] Audio Engine: Audio playback
- [ ] Audio Engine: Interface controls
- [ ] Audio Engine: Volume Controls
- [ ] Audio Engine: Basic mixing
- [ ] Audio Engine: Spatial sound system

Magnitude should eventually support all major hardware accelerations APIs which
can be triggered and modified in the rdview language specification. Due to the design
of the system, hardware acceleration must be used regardless if software rendering
is enabled to make Dear ImGUI work. Obviously, the software rendered images will be
rendered and displayed as a texture (very cool!). Since all major graphics APIs support
render-to-texture, it's possible export frame output as images *and* video, if multiple
frames are created. This is a backburner feature that will require something FFMPEG to
work. Alternatively, I can just have it export the series of images as bitmaps or .png
files for the interim.

# License

Copyright 2024 Christopher DeJong / magictrick-dev

Permission is hereby granted, free of charge, to any person obtaining a copy of 
this software and associated documentation files (the “Software”), to deal in 
the Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
