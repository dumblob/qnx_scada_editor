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
} t_table_data;


typedef struct s_variable_list {
  xmlChar *name;
  xmlChar *value;
  struct s_variable_list *next;
} t_variable_list;

typedef enum {
  SCADA_EDITOR_XML_ATTR_TYPE_STRING,
  SCADA_EDITOR_XML_ATTR_TYPE_NUMBER,
  SCADA_EDITOR_XML_ATTR_TYPE_HEXNUMBER,
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
//void parseTableNode(xmlNodePtr,xmlNodePtr);
//void (*parseTagPtr(xmlNodePtr))(xmlNodePtr,xmlNodePtr);
xmlXPathObjectPtr loadDataFromXpath(xmlChar* xpath);
int parseFile(char *, char *);
xmlChar * process_variable(xmlChar *xpath);
xmlChar * get_variable_value(xmlChar * var_name);
t_table_data *newTableData(PtWidget_t *, xmlNodePtr);

void init();
void destroy();

xmlChar* enhance_xpath(const xmlChar *xpath, const xmlChar * namespace);

void generateSrcFromTree(FILE *);
int getGenTreeItemCount(PtGenTreeItem_t *);
void saveAttrToSrc(PtGenTreeItem_t *, FILE *, unsigned short);
void saveValToSrc(PtGenTreeItem_t *, FILE *, unsigned short);

#endif
