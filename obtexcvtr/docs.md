# Obsidian Texture Converter
## What is Obsidian Texture Converter?
Obsidian Texture Converter (ObTexCvtr/obtexcvtr) is a CLI/GUI-based tool for converting standard file formats into the Obsidian-supported texture format.

## What is the Obsidian Texture Format?
The Obsidian Texture Format (OBTF/obtf) is the file format and file extension used for any and all textures loaded into the Obsidian engine.

TODO: read up on file formats, especially image formats
TODO: should transparency be supported?

## Command-Line Arguments
```
    -g      graphics mode
    -o      set output path
    -x      delete origin image
    -v      verbose mode
    -l      set log output path
    -f      input file format
    -q      quiet mode
```

## Usage
```bash
obtexcvtr.exe -f PNG -v -x -o example.obtf example.png -l example.log  # png, verbose mode, delete origin, output specified, log output specified
obtexcvtr.exe -f JPG -v -x example.jpg -o example.obtf  # jpg/jpeg, verbose mode, delete origin, output specified
obtexcvtr.exe -f BMP -q example.bmp                     # bump-map, quiet mode
obtexcvtr.exe -g                                        # for window
```