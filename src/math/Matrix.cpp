#include <cstdio>
#include "Matrix.h"

namespace renderbox {

    void printMatrix(glm::mat4x4 matrix) {
        fprintf(stdout, "%.4f\t%.4f\t%.4f\t%.4f\n%.4f\t%.4f\t%.4f\t%.4f\n%.4f\t%.4f\t%.4f\t%.4f\n%.4f\t%.4f\t%.4f\t%.4f\n",
                matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
                matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
                matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
                matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
    }

}