# Image Editor

## Overview
This image editing program is designed for basic image manipulation including loading, editing, and saving images in both gray-scale and RGB formats. The program operates through a series of commands interpreted and executed to perform different image processing tasks.

## Functions Description

### General Functions
- **convert**: Maps string commands to specific integer codes to facilitate switch-case execution in C, as direct string comparison is not supported in switch statements.
- **MAX & MIN**: Utility functions to determine the maximum and minimum of two numbers, used in various image processing contexts.
- **ascii_or_binary & photo_type**: Determine the format (ASCII/Binary) and type (Gray-Scale/RGB) of the loaded image.

### Image Manipulation
- **photo_copy**: Copies the characteristics and pixel data from one image structure to another, useful for operations like crop and apply effects.
- **alloc_mat_grey & alloc_mat_color**: Dynamically allocate memory for gray-scale and RGB images, respectively.
- **LOAD, LOAD_T, and LOAD_B**: Load an image from a file, supporting both text and binary formats.
- **SAVE_T and SAVE_B**: Save the edited image back to a file in either text or binary format.
- **SELECT**: Select a specific area of the image for focused editing or apply changes to the whole image.
- **CROP**: Trims the image to the selected area, discarding the rest.

### Image Effects and Transformation
- **APPLY**: Applies a specified effect (e.g., EDGE, SHARPEN, BLUR, GAUSSIAN_BLUR) to the selected area or the whole image if applicable.
- **ROTATE, ROTATE_SELECTED_AREA, ROTATE_ALL**: Rotate the entire image or a selected area by a specified angle (90, 180, 270 degrees).
- **EQUALIZE**: Applies histogram equalization to a gray-scale image, enhancing contrast.
- **HISTOGRAM**: Generates and displays a histogram for the image, helpful for analyzing pixel intensity distribution.

Each function is carefully designed to handle specific aspects of image processing, ensuring the program is modular, reusable, and easy to understand.
