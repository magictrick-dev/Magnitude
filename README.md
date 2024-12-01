<p align="center">
  <img src="assets/icons/app.png" width="300px" />
</p>

# Magnitude Graphics Suite

**Magnitude is a computer graphics video rendering suite with a built-in scripting
engine for designing and creating real-time scenes, model loading system, drawing, and 2D
animation support.**

> **We choose to choose to reinvent the wheel in this decade and do the other things, not because they
> are easy, but because they are hard, because that goal will serve to organize and measure the best of our energies 
> and skills, because that challenge is one that we are willing to accept, one we are unwilling to postpone, and one 
> which we intend to win, and the others, too.**<br/>
> \- <cite>[Someone with too much time on their hands.](https://github.com/magictrick-dev)</cite>

The goal of this project is to create frame-by-frame, high resolution renders of
scenes. These scenes are highly scriptable, using an intermediate programmable
scripting language called "render view" to generate, place, and transform models and
lights in a scene.

# Getting Started with Magnitude

Before you begin, checkout the **[complete render view documentation](./RDVIEWS.md)** page
for a full break-down of the language syntax and commands available to use. The language
is designed to be imperative and simple to use. Once you're familiarize yourself with the
language syntax, open the executable (or build from source) and begin editing.

### Building Magnitude from Source

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

# Project Feature List

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

# Legal Information

**Copyright 2024 Christopher DeJong / magictrick-dev**

Magnitude is a closed-source project.