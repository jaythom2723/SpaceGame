# Engine Architecture
The `engine_context` structure has all of the necessary data
for the game to run successfully. Most engine functions need a reference
to the structure created at run-time, without it, they will fail.

The engine itself is split into different subsections

## [Subsection 1] = [Core]
The core of the engine specifically refers to the `engine_context`
structure, which is always initialized first. The core contains all
relevant information required for a game to function properly while
running.

## [Subsection 2] = [Utility]
The utility subsection is dedicated to the `engine_linked_list` structure
as well as reading from text files such as a `shader` source file.

## [Subsection 3] = [ShaderLoader]
The shader loader subsection is responsible for creating, loading,
compiling, and debugging any and all shaders and shader programs that are
created for the game at run time.

The shader loader subsection is initialized second, as the utility
subsection doesn't need to be initialized.

## [Subsection 4] = [TextureLoader]
The texture loader subsection is responsible for creating, loading,
generating, deleting, and binding textures with OpenGL. If a texture
fails to load, a missing texture is replaced in its place under the same
key.

## [Subsection 5] = [EntityManager]
The entity manager is responsible for the creation, deletion, and
management of all entities that are created at runtime as well as their
attached components and their funcionality.

Each entity is processed using the `engineProcessEntities` function.

## [Subsection 6] = [Renderer]
The renderer does not actually need to be initialized, however, is has
been split into its own subsection as the Renderer is intended to grow
in size over time, such as text or other variations of rendering.

## [Subsection 7] = [InputHandler]
The input handler subsection is the last and final section to be
initialized in the engine. It is responsible for the linked list
that shares keypress/release information with entities with the
`ENGINE_COMPONENT_CONTROLLER` component.

# Render Pipeline
The render pipeline used for the engine is OpenGL alongside GLFW.

*subject to change*
In order to render something to the screen you must:
(1) Initialize the engine
(2) Create the window
(3) Create the shaders
(4) Create the shader program
(5) Compile the shaders
(6) Attach the shaders to the shader program
(7) Link the shader program
(8) Initialize the projection matrix
(9) Initialize the VAO and VBO for rendering
(10) Create the game loop
(11) Poll events
(12) Clear the screen
(13) Process entities
(14) Swap window buffers
(15) Load a valid texture
(16) Create an entity
(17) Add the renderer component to the entity