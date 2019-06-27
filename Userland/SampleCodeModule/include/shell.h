#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <constantPrints.h>
#include <stdlib.h>
#include <blobsFront.h>
#include <digitalTime.h>
#include <exceptions.h>

void shell();
int managingCases(char *option);
int changeTextColor(char *color);
int changeBackGroundColor(char *color);
int wichColor(char *color);

#endif