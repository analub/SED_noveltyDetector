#include <iostream>
#include <cstdlib>

#ifndef PARAMETERS_H
#define PARAMETERS_H

#define NCH 8
#define NSAMPLES 8
#define PATH_MAX 2000

#define FL 60
#define SPC 256
#define FS (SPC * FL)
#define TS 1.0 / FS

#define BATCHSIZE 1

#define BUFFERSIZE (int)(20000.0 / (int)BATCHSIZE)

#define WINDOWSIZE 256
#define NWINDOWS 1
#define NSHOWWINDOWS 12
#define ERRORMARGIN 0.05

#define DEBUG 0

#define MAXLOST 5

#endif
