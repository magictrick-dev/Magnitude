# Magnitude Graphics Visualization Utility

Magnitude is a computer graphics visualization utility with a built-in scripting
engine for designing and creating real-time scenes, model loading system, and a
custom software renderer for developing and testing computer graphics concepts.
The main idea of the utility is to provide a complete tool chain for creating and
rendering scenes using a well-defined scripting language.

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
- [ ] Window System: Non-blocking Fix
- [X] Vendor: Dear ImGUI
- [ ] Vendor: ASSIMP
- [X] Vendor: STBImage
- [X] Resource Management System: Handles
- [X] Resource Management System: File Loading
- [ ] Resource Management System: File Saving
- [ ] Resource Management System: File Streaming
- [ ] Input System: Keyboard
- [ ] Input System: Mouse
- [ ] Input System: Controller
- [ ] RDView Parser: Tokenizer
- [ ] RDView Parser: Parser
- [ ] RDView Parser: Dynamic Parsing
- [ ] RDView Parser: Language Documentation
- [ ] RDView Parser: Editor Highlighting
- [X] Text Editor: Editor GUI System (Github: BalazsJako)
- [ ] Text Editor: Editor Highlighting
- [ ] Text Editor: Performant Refactor of BalazsJako's code.
- [ ] User Interface: Text Editor Layout
- [ ] User Interface: Frame Explorer
- [ ] User Interface: Object List
- [ ] User Interface: Object Inspector
- [ ] User Interface: Runtime Information
- [ ] User Interface: Asset Explorer
- [ ] Modeling: 2D Primitives
- [ ] Modeling: 3D Primitives
- [ ] Modeling: Object File Loading
- [ ] Software Renderer: Line Drawing
- [ ] Software Renderer: Attributed Points
- [ ] Software Renderer: 3D Rendering
- [ ] Software Renderer: Lighting
- [ ] Software Renderer: Texturing
- [ ] Software Renderer: Shader Loader System (Vert. & Frag.)
- [ ] Hardware Renderer: 3D Rendering
- [ ] Hardware Renderer: Lighting & Texturing
- [ ] Behavioral: Renderer Swap from Software to Hardware, vice-versa.
- [ ] Behavioral: Application Icon
- [X] Behavioral: Custom Dear ImGUI Font
- [X] Behavioral: Custom Dear ImGUI UI Color Scheme
- [ ] OPTIONAL: Video Rendering, FFMPEG Support?

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
