# Image to ASCII Art Converter

An image to ASCII art converter written in C++ that generates both text output and a PNG image with colored ASCII characters on a transparent background.

## Features

- Converts any image to ASCII art
- Generates a PNG file with colored ASCII characters and transparent background
- Supports resizing to optimize output quality
- Configurable transparency filter to control which pixels to include
- Option to generate text file output (configurable in code)

## Dependencies

The project requires the following libraries:

- **Armadillo**: for matrix operations
- **Cairo**: for PNG image generation
- **stb_image** and **stb_image_write**: for image reading and writing (included in project)

### Installing dependencies on macOS

```bash
brew install armadillo cairo
```

### Installing dependencies on Ubuntu/Debian

```bash
sudo apt-get install libarmadillo-dev libcairo2-dev
```

## Compilation

Use the included Makefile to compile the project:

```bash
make
```

To clean object files and executable:

```bash
make clean
```

## Usage

```bash
./main [file_path] [transparency_level] [pixel_scale]
```

### Parameters

- **file_path**: Path to the image file to convert
- **transparency_level**: Brightness threshold (0-255)
  - `256`: Include all pixels (full color)
  - `0-255`: Include only pixels with average brightness below this value
- **pixel_scale**: Scaling factor
  - `1`: Original size
  - `2`: Half size
  - `4`: Quarter size
  - Higher values = smaller image

### Examples

```bash
# Standard conversion with all pixels and half size
./main photo.jpg 256 2

# Conversion with transparency filter for dark pixels and 1:4 scale
./main image.png 100 4

# High definition conversion
./main artwork.jpg 256 1
```

## Output

The program generates:

1. **ascii.png**: PNG image with colored ASCII characters on transparent background
2. **output.txt**: Text file with black and white ASCII representation (optional, controlled by `txt` variable in code)

## ASCII Characters Used

The program uses these ASCII characters ordered by density:
```
@%# 
```

- `@`: Darkest pixels
- `%`: Medium brightness pixels  
- `#`: Lighter pixels
- ` `: Space for very light or transparent pixels

## Configuration

To enable text file generation, modify this line in the code:

```cpp
#define txt true  // Change from false to true
```

## Quick recompile and run

```bash
make clean && make && make run
```

## Technical Notes

- Original colors are preserved in PNG output
- Size reduction is performed by averaging RGB values of adjacent pixels
- Supports images with alpha channel (transparency)

## Troubleshooting

- **Compilation error**: Verify all dependencies are correctly installed
- **Cannot open image**: Check that file path is correct and format is supported
- **Output too large/small**: Adjust the `pixel_scale` parameter
- **Too many/few details**: Modify the `transparency_level`
