#ifndef DATALOADER_H
#define DATALOADER_H

#include "ablibs.h"
#include "abimport.h"
#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xpathInternals.h"
#include "proto.h"
#include "table.h"
#include "xml_func.h"

typedef struct s_table_data {
  PtWidget_t *table;
  xmlChar *xpath;
  xmlChar *enhanced_xpath;
} t_table_data;

typedef enum {
  SCADA_ED_XML_ATTR_TYPE_STRING,
  SCADA_ED_XML_ATTR_TYPE_NUMBER,
  SCADA_ED_XML_ATTR_TYPE_CHAR,
  SCADA_ED_XML_ATTR_TYPE_BOOL
} t_xml_attr_type;

typedef struct {
  xmlChar *source;
  t_xml_attr_type type;
} t_xml_info;

int parseFile(char *, char *);
char *getCfgviewNameFromData(xmlDocPtr);
void loadViewAndData(xmlDocPtr, const xmlDocPtr);
void parseTreeNode(xmlNodePtr, xmlNodePtr, const xmlDocPtr,
    PtTreeItem_t **, t_variable_list **, t_variable_list **);
int setTypeAndContentOfCell(PtWidget_t *, int, int,
    const char *, t_xml_attr_type);
int setHeaderCell(PtWidget_t *, int, int,
    PtWidgetClassRef_t *, const char *, void *);
struct s_table_data *createTable(xmlNodePtr, t_variable_list *, const xmlDocPtr);
t_table_data *newTableData(PtWidget_t *, xmlChar *, t_variable_list *);

#endif
