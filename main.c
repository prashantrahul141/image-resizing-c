#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>

// #define DEBUG

#ifdef DEBUG
#define LOG(fmt, args...)                                                      \
  fprintf(stderr, "%s:%d: " fmt, __FILE__, __LINE__, ##args)
#else
#define LOG(fmt, args...)
#endif

#define USAGE "Usage:\n\tmain [scaling factor] [file path]\n"

/// Helper function to print a pixel.
/// # Params
/// * pixel - pointer to the pixel data.
/// * channels - number of channels in the pixel
void print_pixel(unsigned char *pixel, int channels) {
#ifdef DEBUG
  printf("( ");
  for (int i = 0; i < channels; i++) {
    printf("%d", pixel[i]);
    if (i != channels - 1) {
      printf(", ");
    }
  }
  printf(" )\n");
#else
  do {
  } while (0 && pixel + channels);
#endif /* ifdef  DEBUG */
}

/// Copies data from the given color to the given pixel pointer.
/// # Params
/// * color - the color to copy.
/// * pixel - pixel to where copy to
/// * channels - number of channels in the pixel
void copy_pixel(unsigned char *color, unsigned char *pixel, int channels) {
  for (int i = 0; i < channels; i++) {
    *pixel = *color;
    pixel++;
    color++;
  }
}

/// Basically a wrapper around print_pixel for all the pixels in the image.
/// # Params
/// * image - Pointer to the image.
/// * width - width of the image.
/// * height - height of the image.
void print_image(unsigned char *image, int image_size, int channels) {
  for (unsigned char *pixel = image; pixel != image + image_size;
       pixel += channels) {
    print_pixel(pixel, channels);
  }
}

/// Nearest-neighbor interpolation
/// see for more info:
/// https://en.wikipedia.org/wiki/Nearest-neighbor_interpolation
///
/// # Params
/// * o_image - pointer to the original image.
/// * o_width - width of the original image.
/// * o_height - height of the original image.
/// * new_image - pointer to the new image.
/// * new_width - width of the resized image.
/// * new_height - height of the resized image.
/// * resize_scale - scaling factor of new image.
/// * channels - channels in each pixel.
void nearest_neighbor(unsigned char *o_image, int o_height,
                      unsigned char *new_image, int new_width, int new_height,
                      float resize_scale, int channels) {
  for (double new_y = 0.0; new_y < new_height; new_y++) {
    for (double new_x = 0.0; new_x < new_width; new_x++) {

      // original value for new_x and new_y.
      LOG("new(y, x)         = %.1f, %.1f\n", new_y, new_x);

      // interpolated x and y according to resize scale.
      int interpolated_y = floor(new_y / resize_scale);
      int interpolated_x = floor(new_x / resize_scale);

      LOG("iterpolated(y, x) = %d,   %d\n", interpolated_y, interpolated_x);

      unsigned char *color =
          o_image + (channels * (interpolated_y * o_height + interpolated_x));

      LOG("color pick offset : %d\n",
          channels * (interpolated_y * o_height + interpolated_x));
      LOG("color : ");
      print_pixel(color, channels);

      unsigned char *pixel =
          new_image + (channels * ((int)new_y * new_height + (int)new_x));
      LOG("new image offset : %d", (int)new_y * new_height + (int)new_x);
      LOG("\n\n");

      copy_pixel(color, pixel, channels);
    }
  }
}

/// Bilinear interpolation
/// see for more info:
/// https://en.wikipedia.org/wiki/Bilinear_interpolation
///
/// # Params
/// * o_image - pointer to the original image.
/// * o_width - width of the original image.
/// * o_height - height of the original image.
/// * new_image - pointer to the new image.
/// * new_width - width of the resized image.
/// * new_height - height of the resized image.
/// * resize_scale - scaling factor of new image.
/// * channels - channels in each pixel.
void bilinear_interpolation(unsigned char *o_image, int o_height,
                            unsigned char *new_image, int new_width,
                            int new_height, float resize_scale, int channels) {}

// entry point
int main(int argc, char *argv[]) {
  // if argc is not 3
  if (argc != 3) {
    fprintf(stderr, USAGE);
    return 0;
  }

  // resizing scale to apply.
  float resize_scale = atof(argv[1]);
  if (resize_scale == 0.0f) {
    fprintf(stderr, "Invalid scaling factor : %s\n%s", argv[1], USAGE);
    return 1;
  }

  // filepath
  char *filepath = argv[2];
  printf("[INFO] : Loading image : %s\n", filepath);

  // width, height, channels of original image.
  int image_width, image_height, image_channels;

  // load image.
  unsigned char *image_data =
      stbi_load(filepath, &image_width, &image_height, &image_channels, 0);

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
  printf("[INFO] : resize width : %d, resize height : %d\n", resize_width,
         resize_height);

  printf("[INFO] : Loaded image data\n");

  // creating new image buffer.
  unsigned char *new_image =
      calloc(image_channels * resize_width * resize_height, 1);

  // call nearest neighbor interpolation.
  nearest_neighbor(image_data, image_height, new_image, resize_width,
                   resize_height, resize_scale, image_channels);

  // writing new image.
  int result = stbi_write_jpg("output_nearest_neighbor.jpg", resize_width,
                              resize_height, image_channels, new_image, 100);

  if (result == 0) {
    fprintf(stderr, "[ERROR] : Failed to write new image from nearest neighbor "
                    "interpolation.\n");
  } else {
    printf("[INFO] : Wrote new image: output_nearest_neighbor.jpg\n");
  }

  // call bilinear interpolation.
  bilinear_interpolation(image_data, image_height, new_image, resize_width,
                         resize_height, resize_scale, image_channels);

  // writing new image.
  result = stbi_write_jpg("output_bilinear.jpg", resize_width, resize_height,
                          image_channels, new_image, 100);

  if (result == 0) {
    fprintf(
        stderr,
        "[ERROR] : Failed to write new image from bilinear interpolation\n");
  } else {
    printf("[INFO] : Wrote new image: output_bilinear.jpg\n");
  }

  // freeing image.
  stbi_image_free(image_data);
  free(new_image);

  return 0;
}
