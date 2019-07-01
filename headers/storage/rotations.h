

#ifndef ROTATIONS_H
#define ROTATIONS_H

#include <utils.h>
#include <storage/storage.h>

void leafNodeDivision(Address father, int sonPosition);

void internalNodeDivision(Address father, int sonPosition);

void operation3A(Address father, int sonPosition);

void operation3B(Address father, int sonPosition);

#endif