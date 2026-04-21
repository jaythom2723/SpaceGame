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

(10) Deprecated Functions

## Type Definitions
### Redefined standard types
| redefined type			| standard type			|
|---------------------------|-----------------------|
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
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `WindowConfig`	| A window configuration	|

#### Return Value
Returns `EFALSE` if the operation was unsuccessful. Otherwise, `ETRUE`.

### void engineClose(EngineContext*)
#### Description
Destroys everything the engine created during its lifetime as well as itself.

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### void engineInitProjectionMatrix(EngineContext*)
#### Description
Initializes the OpenGL camera to be used in orthographic mode

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### EBOOL engineWindowShouldClose(EngineContext*)
#### Description
A GLFW wrapper for `glfwWindowShouldClose(GLFWwindow*)`

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value
`EFALSE` if the window shouldn't close
`ETRUE` if the window should close

### void engineSetWindowClose(EngineContext*,EBOOL)
#### Description
A GLFW wrapper for `glfwSetWindowShouldClose(GLFWwindow*,int)`

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `EBOOL`			| The value to set			|

#### Return Value

### void enginePollEvents(EngineContext* ctx)
#### Description
A GLFW wrapper for `glfwPollEvents(void)`

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### void engineWindowSwapBuffers(EngineContext* ctx)
#### Description
A GLFW wrapper for `glfwSwapBuffers(GLFWwindow*)`

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### void engineWindowGetSize(EngineContext*,int*,int*)
#### Description
A GLFW wrapper for `glfwGetWindowSize(GLFWwindow*,int*,int*)`

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `int*`			| width						|
| `int*`			| height					|

#### Return Value
Returns the width and height of the engine's internal window via pointer parameters

### void engineSetClearColor(float,float,float,float)
#### Description
A OpenGL wrapper for `glClearColor(float,float,float,float)`

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `float`			| normalized red			|
| `float`			| normalized green			|
| `float`			| normalized blue			|
| `float`			| normalized alpha			|

#### Return Value

### void engineClear(void)
#### Description
A OpenGL wrapper for `glClear(GLenum)`

#### Parameters
#### Return Value

## Utiliy Documentation
### EngineLinkedList* engineCreateLinkedList(char*,char*,size_t,size_t)
#### Description
Create a linked list, copying the values from the first and second parameters
into the first and second list stored within the linked list.

#### Parameters
| type				| description						|
|-------------------|-----------------------------------|
| `char*`			| The first list					|
| `char*`			| The second list					|
| `size_t`			| The max elements for the list		|
| `size_t`			| The size of an element in bytes	|

#### Return Value
The created linked list

### void engineDestroyLinkedList(EngineLinkedList* list)
#### Description
Destroy a linked list

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EngineLinkedList*`	| The list to destroy		|

### Return value

### const EBOOL engineSetValue(EngineLinkedList*,int,int)
#### Description
Sets the values of a linked list for both lists at an index

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EnginedLinkedList*`	| The list to set values	|
| `int`					| The index					|
| `int`					| The value					|

#### Return Value
`EFALSE` if unsuccessful
`ETRUE` if successful

### char* engineReadFile(const char*)
#### Description
Read a text file

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `const char*`		| The path to the file		|

#### Return Value
The contents of the file that was read.
MUST FREE

## Shader Loader Documentation

### EngineShader engineCreateShader(int)
#### Description
Create a shader that OpenGL can use to render

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `int`				| The shader type			|

#### Return Value
Returns the created OpenGL shader

### const EBOOL engineCompileShader(EngineShader*, const char*)
#### Description
Compile a single shader

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineShader*`	| The shader to compile		|
| `const char*`		| The path to the file		|

#### Return Value
`EFALSE` if unsuccessful
`ETRUE` if successful

### void engineDeleteShader(EngineShader*)
#### Description
Delete a shader from OpenGL

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineShader*`	| The shader to delete		|

#### Return Value

### void engineCreateProgram(EngineContext*)
#### Description
Create a shader program for OpenGL rendering

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### const EBOOL engineAttachAndLink(EngineContext*,EngineShader*,EngineShader*,EngineShader*)
#### Description
Attach shaders to a shader program and then link the shader program

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `EngineShader*`	| The vertex shader			|
| `EngineShader*`	| The fragment shader		|
| `EngineShader*`	| The geometry shader/NULL  |

#### Return Value
`EFALSE` if unsuccessful
`ETRUE` if successful

### void engineDeleteProgram(EngineContext*)
#### Description
Delete the engine's shader program

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### void engineUseShader(EngineContext*)
#### Description
Use the shader program

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### void engineSetFloat(EngineContext*,const char*,float)
#### Description
Set a shader uniform of type float to a value

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `float`			| The value					|

#### Return Value

### void engineSetInteger(EngineContext*,const char*,int)
#### Description
Set a shader uniform of type integer to a value

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `int`				| The value					|

#### Return Value

### void engineSetVector2f(EngineContext*,const char*,float,float)
#### Description
Set a shader uniform of type vector2f

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `float`			| x							|
| `float`			| y							|

#### Return Value

### void engineSetVector2fv(EngineContext*,const char*,vec2)
#### Description
Set a shader uniform of type vector2f

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `vec2`			| The value					|

#### Return Value

### void engineSetVector3f(EngineContext*,const char*,float,float,float)
#### Description
Set a shader uniform of type vector3f

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `float`			| x							|
| `float`			| y							|
| `float`			| z							|

#### Return Value

### void engineSetVector3fv(EngineContext*,const char*,vec3)
#### Description
Set a shader uniform of type vector3f

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `vec3`			| The value					|

#### Return Value

### void engineSetVector4f(EngineContext*,const char*,float,float,float,float)
#### Description
Set a shader uniform of type vector4f

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `float`			| x							|
| `float`			| y							|
| `float`			| z							|
| `float`			| w							|

#### Return Value

### void engineSetVector4fv(EngineContext*,const char*,vec4)
#### Description
Set a shader uniform of type vector4f

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `vec4`			| The value					|

#### Return Value

### void engineSetMatrix4fv(EngineContext*,const char*,mat4)
#### Description
Set a shader uniform of type matrix4f

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `const char*`		| The name of the uniform	|
| `mat4`			| The value					|

#### Return Value

## Texture Loader Documentation
### const EBOOL engineCreateTexture(EngineContext*,unsigned int)
#### Description
Create a texture through the engine

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `unsigned int`	| A texture key				|

#### Return Value
`EFALSE` if unsuccessful
`ETRUE` if successful

### const EBOOL engineGenerateTexture(EngineContext*,unsigned int,const char*,const EBOOL)
#### Description
Generate a texture through OpenGL for rendering purposes

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `unsigned int`	| A texture key				|
| `const char*`		| The path to the file		|
| `const EBOOL`		| Should use alpha channel	|

#### Return Value
`EFALSE` if unsuccessful
`ETRUE` if successful

### void engineGenTextureCheckMissing(EngineContext*,unsigned int,const char*,const EBOOL)
#### Description
Generate a texture through OpenGL for rendering purposes.
However, if generating the original texture fails, replace it with a missing texture.

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `unsigned int`	| A texture key				|
| `const char*`		| The path to the file		|
| `const EBOOL`		| Should use alpha channel	|

#### Return Value

### void engineBindTexture(EngineContext*,unsigned int)
#### Description
Bind a given texture to be active for OpenGL

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `unsigned int`	| The texture to bind (id)	|

#### Return Value

### void engineDestroyAllTextures(EngineContext*)
#### Description
Destroy all textures created by the engine

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### void engineDestroyTexture(EngineContext*,unsigned int)
#### Description
Destroy a single texture, based on its key

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `unsigned int`	| A texture key				|

#### Return Value

## Entity Manager Documentation
### const EBOOL engineCreateEntity(EngineContext*,vec2,vec2,const EngineEntityKey,const EngineTextureKey)
#### Description
Create an entity

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EngineContext*`		| A valid context			|
| `vec2`				| The position				|
| `vec2`				| The size					|
| `EngineEntityKey`		| The key for the entity	|
| `EngineTextureKey`	| The texture key			|

#### Return Value
`EFALSE` if unsuccessful
`ETRUE` if successful

### void engineDestroyAllEntities(EngineContext*)
#### Description
Destroy all entities created by the engine at runtime.

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|

#### Return Value

### void engineDestroyEntity(EngineContext*,EngineEntityKey)
#### Description
Destroy a single entity created by the engine at runtime

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `EngineEntityKey`	| An entity					|

#### Return Value

### EngineEntity* engineGetEntity(EngineContext*,EngineEntityKey)
#### Description
Get a single entity from the engine and return it as a pointer.

#### Parameter
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `EngineEntityKey`	| An entity					|

#### Return Value
Returns the entity or `NULL`

### void engineInitEntityComponentList(EngineEntity*)
#### Description
Initialize the components list by heap allocating it.

This is mostly used ass an internal function as it gets called whenever
`engineAddComponent` is called on a new entity.

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineEntity*`	| The entity				|

### void engineProcessEntities(EngineContext*,float)
#### Description
Process all entities created by the engine

#### Parameters
| type				| description				|
|-------------------|---------------------------|
| `EngineContext*`	| A valid context			|
| `float`			| Delta Time				|

#### Return Value

### void engineAddComponent(EngineContext*,EngineEntityKey,EngineComponentType)
#### Description
Add a component to an entity

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EngineContext*`		| A valid context			|
| `EngineEntityKey`		| An entity					|
| `EngineComponentType`	| The type of component		|

#### Return Value

### void engineSetComponentCallback(EngineContext*,EngineEntityKey,EngineComponentType,EngineComponentFunc)
#### Description
Set a component's callback function

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EngineContext*`		| A valid context			|
| `EngineEntityKey`		| An entity					|
| `EngineComponentType`	| The type of component		|
| `EngineComponentFunc`	| The new callback function |

#### Return Value

### const EBOOL engineRemoveComponent(EngineContext*,EngineEntityKey,EngineComponentType)
#### Description
Remove a component from an entities component list

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EngineContext*`		| A valid context			|
| `EngineEntityKey`		| An entity					|
| `EngineComponentType`	| The type of component		|

#### Return Value
`EFALSE` if unsuccessful
`ETRUE` if successful

### const EBOOL engineHasComponent(EngineContext*,EntityEntityKey,EngineComponentType)
#### Description
Check if an entity has a component of a specific type

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EngineContext*`		| A valid context			|
| `EngineEntityKey`		| An entity					|
| `EngineComponentType`	| The type of component		|

#### Return Value
`ETRUE` if the entity has the component of the specified type.
`EFALSE` otherwise

### void engineRunComponentFunc(EngineContext*,EngineEntity*,EngineComponent*,float)
#### Description
Run a component's set callback function or a default callback function

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EngineContext*`		| A valid context			|
| `EngineEntity*`		| The entity				|
| `EngineComponent*`	| The component				|
| `float`				| Delta Time				|

#### Return Value

## Renderer Documentation
### void engineRenderEntity(EngineContext*,EngineEntityKey)
#### Description
Render a single entity to the window

#### Parameters
| type					| description				|
|-----------------------|---------------------------|
| `EngineContext*`		| A valid context			|
| `EngineEntityKey`		| An entity					|

#### Return Value

## Deprecated Functions
### void engineDrawSprite(...)
#### Description
A deprecated original way of drawing sprites to the screen.
Effectively completely removed in this version, only serving as a
"historical" artifact in the code.