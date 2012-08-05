#include "ablibs.h"
#include "abimport.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "proto.h"
#include "table.h"

void loadViewAndData();
struct s_table_data *createTable(xmlNodePtr);
void parseTree(xmlNodePtr tree);
void parseTreeNode(PtTreeItem_t *parent_item, xmlNodePtr tree_node);
xmlXPathObjectPtr loadDataFromXpath(xmlChar* xpath);
int parseFile(char *, char *);

void init();
void destroy();

xmlChar* enhance_xpath(xmlChar *xpath, const xmlChar * namespace);

typedef struct s_table_data {
	PtWidget_t *table;
	xmlChar *xpath;
} t_table_data;


typedef struct s_variable_list{
	xmlChar *name;
	xmlChar *value;
	struct s_variable_list *next;

}t_variable_list;

t_variable_list *first = NULL;
t_variable_list *last = NULL;

