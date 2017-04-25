#ifndef IO_MTX_FILE_FORMAT_H
#define IO_MTX_FILE_FORMAT_H

#include <fstream>
#include <vector>

namespace taco {
class TensorBase;

namespace io {
namespace mtx {

void readFile(std::ifstream &mtxfile, int blockSize,
              int* nrow, int* ncol, int* nnzero,
              TensorBase* tensor);

void writeFile(std::ofstream &mtxfile, std::string name,
               const std::vector<int> dimensions, int nnzero);

/// Read an mtx matrix from a file.
TensorBase readTensor(std::string filename, std::string name="");

/// Read an mtx matrix from a stream.
TensorBase readTensor(std::istream& stream, std::string name="");

}}}

#endif
