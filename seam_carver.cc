#include "seam_carver.hpp"
#include "pixel.hpp"
#include "image_ppm.hpp"


// implement the rest of SeamCarver's functions here
const ImagePPM& SeamCarver::GetImage() const {
  return image_;
}
int SeamCarver::GetHeight() const {
  return height_;
}
int SeamCarver::GetWidth() const {
  return width_;
}
int SeamCarver::GetEnergy(int row, int col) const {
  Pixel above = Pixel();
  Pixel below = Pixel();
  Pixel right = Pixel();
  Pixel left = Pixel();
  if (row - 1 < 0) {
     above = image_.GetPixel(height_ -1 ,col);
  } else {
     above = image_.GetPixel(row -1 ,col);
  }
  if (row + 1 > height_- 1) {
    below = image_.GetPixel(0 ,col);
  } else {
    below = image_.GetPixel(row +1 ,col);
  }
  if (col - 1 < 0) {
    left = image_.GetPixel(row ,width_-1);
  } else {
    left = image_.GetPixel(row ,col -1);
  }
  if (col +1 > width_ -1) {
     right = image_.GetPixel(row ,0);
  } else {
     right = image_.GetPixel(row ,col + 1);
  }
 
  return (((above.GetRed() - below.GetRed())*(above.GetRed() - below.GetRed()))+ ((above.GetGreen() - below.GetGreen())*(above.GetGreen() - below.GetGreen()))+ ((above.GetBlue() - below.GetBlue())*(above.GetBlue() - below.GetBlue()))+((left.GetRed() - right.GetRed())*(left.GetRed() - right.GetRed()))+((left.GetGreen() - right.GetGreen())*(left.GetGreen() - right.GetGreen()))+((left.GetBlue() - right.GetBlue())*(left.GetBlue() - right.GetBlue())));
}
int Getstart(int** energies,int height) {
  int lowest = energies[0][0];
  int r = 0;
  for (int row = 0; row < height;row++) {
    if (lowest > energies[row][0]) {
      r = row;
      lowest = energies[row][0];
    }
  }
  return r;
}
int Getnext(int** energies, int height,  int col, int row) {
  int lowest = energies[row][col];
  int r = row;
  if (row -1 >= 0) {
    if (lowest > energies[row -1][col]) {
      lowest = energies[row-1][col];
      r = row-1;
    }
  }
  if (row +1 < height) {
    if (lowest > energies[row +1][col]) {
      lowest = energies[row+1][col];
      r = row+1;
    }
  }
  if (lowest >= energies[row][col]) {
    r = row;
  }
  return r;
}
int Getstart2(int** energies,int width) {
  int lowest = energies[0][0];
  int r = 0;
  for (int col = 0; col < width;col++) {
    if (lowest > energies[0][col]) {
      r = col;
      lowest = energies[0][col];
    }
  }
  return r;
}
int Getnext2(int** energies, int width,  int col, int row) {
  int lowest = energies[row][col];
  int r = col;
  if (col -1 >= 0) {
    if (lowest > energies[row][col-1]) {
      lowest = energies[row][col-1];
      r = col-1;
    }
  }
  if (col +1 < width) {
    if (lowest > energies[row][col+1]) {
      lowest = energies[row][col+1];
      r = col+1;
    }
  }
  if (lowest >= energies[row][col]) {
    r = col;
  }
  return r;
}
int* SeamCarver::GetHorizontalSeam() const {
  int** energies = new int*[height_];
  for (int row = 0; row < height_;row++) {
    energies[row] = new int[width_];
  }
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col++) {
      energies[row][col] = GetEnergy(row, col);
    }
  }
  for (int col = width_ -2; col >= 0; col--) {
    for (int row = 0; row < height_;row++) {
      int lowestenergy = energies[row][col+1];
      if (row - 1 >=0) {
        if (lowestenergy > energies[row -1][col+1]) {
          lowestenergy = energies[row -1][col+1];
        } 
      }
      if (row + 1 < height_) {
        if (lowestenergy > energies[row +1][col+1]) {
          lowestenergy = energies[row +1][col+1];
        } 
      }
      
      energies[row][col] = energies[row][col] + lowestenergy;
    }
  }
  int* final = new int[width_];
  int start = Getstart(energies,height_);
  final[0] = start;
  for (int col = 1; col < width_;col++) {
    start = Getnext(energies,height_,col,start);
    final[col] = start;
  }
  for(int row = 0; row < height_;row++) {
    delete [] energies[row];
  }
  delete [] energies;
  energies = nullptr;
  return final;
}
int* SeamCarver::GetVerticalSeam() const {
  int** energies = new int*[height_];
  for (int row = 0; row < height_;row++) {
    energies[row] = new int[width_];
  }
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col++) {
      energies[row][col] = GetEnergy(row, col);
    }
  }
  for (int row = height_ -2; row >= 0; row--) {
    for (int col = 0; col < width_;col++) {
      int lowestenergy = energies[row+1][col];
      if (col - 1 >=0) {
        if (lowestenergy > energies[row+1][col-1]) {
          lowestenergy = energies[row +1][col-1];
        } 
      }
      if (col+1 < width_) {
        if (lowestenergy > energies[row +1][col+1]) {
          lowestenergy = energies[row +1][col+1];
        } 
      }
      energies[row][col] = energies[row][col] + lowestenergy;
    }
  }
  int* final = new int[height_];
  int start = Getstart2(energies,width_);
  final[0] = start;
  for (int row = 1; row < height_;row++) {
    start = Getnext2(energies,width_,start,row);
    final[row] = start;
  }
  for(int row = 0; row < height_;row++) {
    delete [] energies[row];
  }
  delete [] energies;
  energies = nullptr;
  return final;
}
void SeamCarver::RemoveHorizontalSeam() {
  int* thing = GetHorizontalSeam();
  image_.RemoveH(thing);
  SetImage(image_);
  delete[] thing;
  thing = nullptr;
}
void SeamCarver::RemoveVerticalSeam() {
  int* thing = GetVerticalSeam();
  image_.RemoveV(thing);
  SetImage(image_);
  delete[] thing;
  thing = nullptr;
}
// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
