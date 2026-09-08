# Obsidian Texture Converter
## What is Obsidian Texture Converter?
Obsidian Texture Converter (ObTexCvtr/obtexcvtr) is a CLI/GUI-based tool for converting standard file formats into the Obsidian-supported texture format.

## Command-Line Arguments
```
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

# Obsidian Texture Format
The Obsidian Texture Format (OBTF/obtf) is the file format and file extension used for any and all textures loaded into the Obsidian engine.

8-bit RGBA color channels are provided in the texture format file. Due to the uncompressed nature of the textures, the pixels are all aligned in raw format, meaning that header information is extraneous and removed from the conversion process alongside decompression methods for certain file types.

Pixels of a certain color become transparent when being converted, meaning they cannot be used during conversion or during sprite making with the intent to use as decoration/art. The following color(s) cannot show-up in the converter as they are treated as transparent space:
Bright Magenta

If the original image isn't in RGBA supported color formats, the converter will automatically create an alpha channel for the converted image.
