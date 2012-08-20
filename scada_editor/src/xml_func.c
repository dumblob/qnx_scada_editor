#include "xml_func.h"
#include "assert.h"

t_variable_list *first = NULL;
t_variable_list *last = NULL;

#define SCADA_EDITOR_NS_URI BAD_CAST "http://www.disam.cz/Xmlns/Scada/Configuration"
#define SCADA_EDITOR_NS_PREFIX BAD_CAST "disam"

#define myXMLNewChild(doc, node, s, ns, counter) \
	do { \
		if (counter == 0) { \
			(ns) = xmlNewNs(NULL, SCADA_EDITOR_NS_URI, SCADA_EDITOR_NS_PREFIX); \
			(node) = xmlNewNode((ns), BAD_CAST (s)); \
			xmlNewNs((node), SCADA_EDITOR_NS_URI, SCADA_EDITOR_NS_PREFIX); \
			xmlDocSetRootElement((document), (node)); \
		} \
		else { \
			(node) = xmlNewChild((node), (ns), BAD_CAST (s), NULL); \
		} \
		xmlNewProp((node), BAD_CAST "id", BAD_CAST "1"); \
		++counter; \
	} while (0)

xmlXPathObjectPtr loadDataFromXpathNS(xmlChar* xpath, xmlDocPtr document, bool handle_new_file)
{
	xmlXPathObjectPtr result = NULL;
	xmlChar *enhanced_xpath = process_variable(xpath);
	xmlChar *full_xpath = enhance_xpath(enhanced_xpath, SCADA_EDITOR_NS_PREFIX);

	/* handle new file: construct a simple, minimal XML tree from the given
	   xpath, containing always only one children  */
	if (handle_new_file)
	{
		assert(xpath != NULL);
		char *s  = (char *)xpath;  /* start */
		char *ss = (char *)xpath;  /* end   */
		xmlNs *namespace = NULL;
		int counter = 0;
		xmlNode *tmp = NULL;

		if (*s != '\0')
		{
			++s;   /* remove leading / */
			++ss;  /* remove leading / */

			for (;;)
			{
				if (*ss == '\0')
				{
					/* we are not at the very beginning (+1 removes leading /) */
					if (ss != (char *)xpath +1 && ss != s)
						myXMLNewChild(document, tmp, s, namespace, counter);

					break;
				}

				if (*ss == '/' || *ss == '[')
				{
					/* overcome empty elements */
					if (ss != s)
					{
						char c = *ss;
						*ss = '\0';
						myXMLNewChild(document, tmp, s, namespace, counter);
						*ss = c;
					}

					++ss;
					s = ss;
					continue;
				}

				if (*ss == ']')
				{
					++ss;
					s = ss;
					continue;
				}

				++ss;
			}
		}
	}

	xmlXPathContextPtr context = xmlXPathNewContext(document);

	if (context == NULL)
	{
		fprintf(stderr, "Error in xmlXPathNewContext.\n");
		return NULL;
	}

	if (xmlXPathRegisterNs(context, SCADA_EDITOR_NS_PREFIX, SCADA_EDITOR_NS_URI))
	{
		fprintf(stderr, "Error when registering namespace.\n");
		result = NULL;
	}
	else
	{
		result = xmlXPathEvalExpression(full_xpath, context);

		/* no result detected */
		if (result != NULL && xmlXPathNodeSetIsEmpty(result->nodesetval))
		{
			xmlXPathFreeObject(result);
			result = NULL;
		}
	}

	xmlFree(full_xpath);
	xmlXPathFreeContext(context);

	return result;
}


xmlChar * get_variable_value(xmlChar * var_name) {
	t_variable_list *act = first;

	while (act != NULL)
	{
		if (act->name == NULL) continue;

		if (xmlStrEqual(act->name, var_name)) return act->value;

		act = act->next;
	}

	return NULL;
}


xmlChar *process_variable(xmlChar *xpath)
{
	const xmlChar *var_start = NULL;
	const xmlChar *var_end = NULL;
	xmlChar *enhanced_xpath = xpath;
	xmlChar dolar = '$';
	xmlChar *var_name = NULL;
	xmlChar *var_value = NULL;

	var_start = xmlStrchr(xpath, dolar);

	if (var_start != NULL)
		enhanced_xpath = xmlCharStrndup((const char*)xpath, var_start - xpath);

	while (var_start != NULL)
	{
		var_end = xmlStrchr(var_start + 1, ']');

		if (var_end == NULL)
		{
			fprintf(stderr, "ERROR while replacing xml variable in xpath.\n");
			var_start = NULL;
		}
		else
		{
			var_name = xmlCharStrndup((const char *) var_start + 1,
					var_end - var_start - 1);
			var_value = get_variable_value(var_name);

			if (var_value != NULL)
			{
				enhanced_xpath = xmlStrcat(enhanced_xpath, var_value);
			}
			else
			{
				fprintf(stderr, "ERROR xls:variable \"%s\" not found.\n", var_name);
				xmlFree(var_name);
				return xmlStrdup(xpath);
			}

			xmlFree(var_name);
			var_start = xmlStrchr(var_end + 1, dolar);

			if (var_start == NULL)
				enhanced_xpath = xmlStrcat(enhanced_xpath, var_end);
			else
				enhanced_xpath = xmlStrncat(enhanced_xpath, var_end,
						var_start - var_end);
		}
	}

	return enhanced_xpath;
}


/* /abc/xyz -> /disam:abc/disam:xyz */
xmlChar *enhance_xpath(const xmlChar *xpath, const xmlChar * namespace)
{
	xmlChar *ens;
	xmlChar *sep;
	xmlChar *fullpath;

	int f2 = 1;

	ens = xmlCharStrdup("/");
	ens = xmlStrncat(ens, namespace, xmlStrlen(namespace));
	ens = xmlStrncat(ens, BAD_CAST ":", 1);

	fullpath = xmlStrdup(ens);

	sep = (xmlChar *)xmlStrchr(xpath, (xmlChar)'/');

	while (sep != NULL) {
		f2 = sep - xpath + 1;
		sep = (xmlChar *)xmlStrchr(sep + 1, (xmlChar)'/');

		if (sep != NULL) {
			fullpath = xmlStrncat(fullpath, xpath + f2, sep - xpath - f2);
			fullpath = xmlStrncat(fullpath, ens, xmlStrlen(ens));
		} else {
			fullpath = xmlStrncat(fullpath, xpath + f2, xmlStrlen(xpath + f2));
		}
	}

	xmlFree(ens);

	return fullpath;
}

/* vim: set noexpandtab: */
