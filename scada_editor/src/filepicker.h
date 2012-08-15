/*
 * filepicker.h
 *
 *  Created on: 15.8.2012
 *      Author: Casey
 */

#ifndef FILEPICKER_H_
#define FILEPICKER_H_

#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

int showFileSelector(PtFileSelectionInfo_t *, const char *, const char *);

#endif /* FILEPICKER_H_ */
