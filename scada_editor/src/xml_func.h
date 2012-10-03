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

xmlChar * process_variable(xmlChar *xpath);
xmlChar * get_variable_value(xmlChar *var_name);
xmlXPathObjectPtr loadDataFromXpathNS(xmlChar *, xmlDocPtr, bool);

xmlChar* enhance_xpath(const xmlChar *xpath, const xmlChar * namespace);

#endif /* XML_FUNC_H_ */
