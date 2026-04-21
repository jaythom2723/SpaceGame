# Documentation

## Contents
(1) Architecture *make link*
(2) Getting Started *make link*
(3) Type Definitions
(4) Core Documentation
(5) Utility Documentation
(6) Shader Loader Documentation
(7) Texture Loader Documentation
(8) Entity Manager Documentation
(9) Renderer Documentation
(10) Input Handler Documentation
(11) Deprecated Functions

## Type Definitions
### Redefined standard types
| `EBOOL`					| `unsigned int`		|
| `EngineShader`			| `unsigned int`		|
| `EngineProgram`			| `unsigned int`		|
| `EngineTextureKey`		| `unsigned int`		|
| `EngineEntityKey`			| `unsigned int`		|
| `EngineComponentID`		| `unsigned int`		|

### EngineComponentFunc
#### Type Definition
`typedef void (*EngineComponentFunc)(EngineEntity*,EngineContext*,float)`

#### Members

#### Description
`EngineComponentFunc` is a type definition specifically used to make the
usage of component callback functions a bit easier to read.

### EngineComponentType
#### Type Definition
`typedef enum component_type { ... } EngineComponentType`

#### Members
`ENGINE_COMPONENT_RENDERER`
`ENGINE_COMPONENT_TICK`
`ENGINE_COMPONENT_CONTROLLER`

#### Description
Used to destinguish components apart

### WindowConfig
#### Type Definition
`typedef struct window_config { ... } WindowConfig`

#### Members
`char* title`
`unsigned int width`
`unsigned int height`
`EBOOL resizable`
`EBOOL doublebuffer`

#### Description
Used to tell the engine how to create the window

### EngineTexture
#### Type Definition
`typedef struct engine_texture {...} EngineTexture`

#### Members
`unsigned int id`
`unsigned int width`
`unsigned int height`
`unsigned int internalformat`
`unsigned int imageformat`
`unsigned int wraps`
`unsigned int wrapt`
`unsigned int filtermax`
`unsigned int filtermin`

#### Description
Contains all relevant information for texture generation

### EngineTexturePair
#### Type Definition
`typedef struct engine_texture_pair { ... } EngineTexturePair`

#### Members
`EngineTextureKey key`
`EngineTexture* value`

#### Description
Used to make a pseudo-dictionary containing all textures created by the engine

### EngineEntity
#### Type Definition
`typedef struct engine_entity { ... } EngineEntity`

#### Members
`float x`
`float y`
`float width`
`float height`
`EngineEntityKey key`
`EngineTextureKey texkey`
`EngineComponent* components`

#### Description
An entity serves as the primary container for all points of functionality via components.

### EngineComponent
#### Type Definition
`typedef struct engine_component { ... } EngineComponent`

#### Members
`EngineComponentID id`
`EngineComponentType type`
`EngineComponentFunc func`

#### Description
Serves as the primary provider for functionality provided to entities

### EngineLinkedList
#### Type Definition
`typedef struct linklist { ... } EngineLinkedList`

#### Members
`char* lista`
`char* listb`
`size_t typesize`
`size_t arrsize`
`EBOOL aalloc`
`EBOOL balloc`

#### Description
A standard linked list data structure for use in-tandem with the input handler

### EngineContext
#### Type Definition
`typedef struct engine_context { ... } EngineContext`

#### Members
`GLFWwindow* window`
`EngineTexturePair textures[1024]`
`EngineEntity entities[65535]`
`EBOOL keys[1024]`
`EngineProgram program`
`unsigned int vao`
`unsigned int ntextures`
`unsigned int nentities`

#### Description
The engine itself, nothing will work without a valid engine context!

## Core Documentation
### EngineContext* engineInit(void)
#### Description
Initializes the entire engine including all subsections.

#### Parameters

#### Return Value
Returns the initialized engine context or NULL if unsuccessful.

### EBOOL engineCreateWindow(EngineContext*,WindowConfig)
#### Description
Creates the game's window according to the `WindowConfig` parameter.
Makes sure to initialize OpenGL via GLAD as well.

#### Parameters
| `EngineContext*`	| A valid context			|
| `WindowConfig`	| A window configuration	|

#### Return Value
Returns `EFALSE` if the operation was unsuccessful. Otherwise, `ETRUE`.

### void engineClose(EngineContext*)
#### Description
Destroys everything the engine created during its lifetime as well as itself.

#### Parameters
| `EngineContext*`	| A valid context	|

#### Return Value

### void engineInitProjectionMatrix(EngineContext*)
#### Description
Initializes the OpenGL camera to be used in orthographic mode

#### Parameters
| `EngineContext*`	| A valid context	|

#### Return Value

### EBOOL engineWindowShouldClose(EngineContext*)
#### Description
A GLFW wrapper for `glfwWindowShouldClose(GLFWwindow*)`

#### Parameters
| `EngineContext*`	| A valid context	|

#### Return Value
`EFALSE` if the window shouldn't close
`ETRUE` if the window should close

### void engineSetWindowClose(EngineContext*,EBOOL)
#### Description
A GLFW wrapper for `glfwSetWindowShouldClose(GLFWwindow*,int)`

#### Parameters
| `EngineContext*`	| A valid context	|
| `EBOOL`			| The value to set	|

#### Return Value

### void enginePollEvents(EngineContext* ctx)
#### Description
A GLFW wrapper for `glfwPollEvents(void)`

#### Parameters
| `EngineContext*`	| A valid context	|

#### Return Value

### void engineWindowSwapBuffers(EngineContext* ctx)
#### Description
A GLFW wrapper for `glfwSwapBuffers(GLFWwindow*)`

#### Parameters
| `EngineContext*`	| A valid context	|

#### Return Value

### void engineWindowGetSize(EngineContext*,int*,int*)
#### Description
A GLFW wrapper for `glfwGetWindowSize(GLFWwindow*,int*,int*)`

#### Parameters
| `EngineContext*`	| A valid context	|
| `int*`			| width				|
| `int*`			| height			|

#### Return Value
Returns the width and height of the engine's internal window via pointer parameters

### void engineSetClearColor(float,float,float,float)
#### Description
A OpenGL wrapper for `glClearColor(float,float,float,float)`

#### Parameters
| `float`	| normalized red	|
| `float`	| normalized green	|
| `float`	| normalized blue	|
| `float`	| normalized alpha	|

#### Return Value

### void engineClear(void)
#### Description
A OpenGL wrapper for `glClear(GLenum)`

#### Parameters
#### Return Value

## Utiliy Documentation