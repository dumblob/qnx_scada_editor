



#ifndef DATASAVER_H_
#define DATASAVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ablibs.h"
#include "abimport.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "proto.h"


void save_data();
void generateXMLfromTree();
void save_table(PtWidget_t *table, xmlChar *xpath, xmlNodePtr *root_node);


#endif /* DATASAVER_H_ */
