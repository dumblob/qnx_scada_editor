#ifndef DATASAVER_H_
#define DATASAVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>  /* toupper() */

#include "ablibs.h"
#include "abimport.h"
/* the QNX libxml2 does not provide the full libxml2 functionality */
#include "libxml/tree.h"
#include "libxml/parser.h"
#include "proto.h"
#include "table.h"


void save_data();
void exportToSrc(char *);

void generateSrcFromTree(FILE *);
void printTableLines(PtWidget_t *, int, int, int, int, FILE *, char *);
void saveAttrToSrc(PtGenTreeItem_t *, FILE *, unsigned short);
int getGenTreeItemCount(PtGenTreeItem_t *);
void saveValToSrc(PtGenTreeItem_t *, FILE *, unsigned short);
void walkOverTreeBranch(PtGenTreeItem_t *, xmlDocPtr, xmlNsPtr);

xmlChar* getAttrNameFrom(xmlChar *);
xmlChar* getAttrValueFrom(xmlChar *);
xmlChar* getPureNodeNameFrom(xmlChar *);
int node_have_attribude(xmlChar *);
xmlNodePtr process_node(xmlNodePtr, xmlChar *, xmlNsPtr);

#endif
