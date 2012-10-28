/*
 * xml_func.h
 *
 *  Created on: 14.8.2012
 *      Author: xjerab13
 */

#ifndef XML_FUNC_H_
#define XML_FUNC_H_

#include "libxml/parser.h"
#include "libxml/xpath.h"
#include "libxml/xpathInternals.h"
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"
#include "stdbool.h"

typedef struct s_variable_list {
  xmlChar *name;
  xmlChar *value;
  struct s_variable_list *next;
} t_variable_list;

xmlChar *process_variable(xmlChar *, t_variable_list *);
xmlChar *get_variable_value(xmlChar *);
xmlXPathObjectPtr loadDataFromXpathNS(xmlChar *, xmlDocPtr, bool, t_variable_list *);

xmlChar* enhance_xpath(const xmlChar *, const xmlChar *);

#endif /* XML_FUNC_H_ */
