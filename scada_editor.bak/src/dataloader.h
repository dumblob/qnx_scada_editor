#ifndef DATALOADER_H
#define DATALOADER_H

#include "ablibs.h"
#include "abimport.h"
#include "libxml/parser.h"
//#include <libxml/tree.h>  //not needed?
#include "libxml/xpath.h"
#include "libxml/xpathInternals.h"
#include "proto.h"
#include "table.h"

typedef struct s_table_data {
  PtWidget_t *table;
  xmlChar *xpath;
  xmlChar *enhanced_xpath;
} t_table_data;

typedef enum {
  SCADA_EDITOR_XML_ATTR_TYPE_STRING,
  SCADA_EDITOR_XML_ATTR_TYPE_NUMBER,
  SCADA_EDITOR_XML_ATTR_TYPE_CHAR,
  SCADA_EDITOR_XML_ATTR_TYPE_BOOL
} t_xml_attr_type;

typedef struct {
  xmlChar *source;
  t_xml_attr_type type;
} t_xml_info;

void loadViewAndData();
struct s_table_data *createTable(xmlNodePtr);
void parseTree(xmlNodePtr tree);
void parseVarNode(xmlNodePtr, xmlChar *);
void parseTreeNode(xmlNodePtr,xmlNodePtr);
void generateXML(t_table_data *data);
int parseFile(char *, char *);

t_table_data *newTableData(PtWidget_t *, xmlNodePtr);
int setTypeAndContentOfCell(PtWidget_t *, int, int, const char *, t_xml_attr_type);

void init();
void destroy();

#endif
