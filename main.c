#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>

// we fix channels to 3
#define CHANNEL 3

/// Helper function to print a image to stdout.
/// # Params
/// * image - Pointer to the image.
/// * width - width of the image.
/// * height - height of the image.
void print_image(unsigned char *image, int width, int height) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      unsigned char *pixel = image + (i * width) + j;

      printf("[ x: %d, y : %d ] ( ", i, j);
      for (size_t channel = 0; channel < 3; channel++) {
        printf("%d  ", *(pixel + channel));
      }
      printf(")\n");
    }
  }
}

/// Nearest-neighbor interpolation
/// # Params
/// * o_image - pointer to the original image.
/// * o_width - width of the original image.
/// * o_height - height of the original image.
/// * new_image - pointer to the new image.
/// * new_width - width of the resized image.
/// * new_height - height of the resized image.
/// * resize_scale - scaling factor of new image.
/// see : https://en.wikipedia.org/wiki/Nearest-neighbor_interpolation
void nearest_neighbor(unsigned char *o_image, int o_width, int o_height,
                      unsigned char *new_image, int new_width, int new_height,
                      float resize_scale) {}

// entry point
int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage:\n\tmain [ratio] [file path]\n");
    return 0;
  }

  // resizing scale to apply.
  int resize_scale = atof(argv[1]);

  // filepath
  char *filepath = argv[2];

  // width, height, channels of original image.
  int image_width, image_height, image_channels;

  // load image.
  unsigned char *image_data = stbi_load(filepath, &image_width, &image_height,
                                        &image_channels, CHANNEL);

  // incase image loading fails.
  if (image_data == NULL) {
    fprintf(stderr, "[ERROR] : Failed to load image : %s\n", filepath);
    return 1;
  }

  // if loading image succeds.
  printf("[INFO] : Loaded image : %s\n", filepath);

  // new width and height according to resize factor.
  int resize_width = image_width * resize_scale;
  int resize_height = image_height * resize_scale;

  printf("[INFO] : Original image width : %d, image height : %d\n", image_width,
         image_height);
  printf("[INFO] : Resize width : %d, Reside height : %d\n", resize_width,
         resize_height);

  printf("[INFO] : Loaded image data:\n");
  print_image(image_data, image_width, image_height);

  // creating new image buffer.
  unsigned char *new_image = (unsigned char *)calloc(
      sizeof(unsigned char) * CHANNEL * resize_width * resize_height, 1);

  printf("[INFO] : New Image :\n");
  print_image(new_image, resize_width, resize_height);

  // writing new image.
  int result = stbi_write_jpg("output.jpg", resize_width, resize_height, 3,
                              new_image, 100);

  // depending of the implementation call resizing algorithm.
  nearest_neighbor(image_data, image_width, image_height, new_image,
                   resize_width, resize_height, resize_scale);

  if (result == 0) {
    fprintf(stderr, "[ERROR] : Failed to write new image\n");
  } else {
    printf("[INFO] : Wrote new image.\n");
  }

  // freeing image.
  stbi_image_free(image_data);
  free(new_image);

  return 0;
}
