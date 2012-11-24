/*
 * xjerab13
 * 2012-10-28 19:46:57 CET
 */

#include "xml_func.h"
#include "assert.h"

#define myXMLNewChild(doc, node, s, ns, counter) \
	do { \
		if (counter == 0) { \
			(ns) = xmlNewNs(NULL, BAD_CAST SCADA_ED_NS_URI, \
					BAD_CAST SCADA_ED_NS_PREFIX); \
			(node) = xmlNewNode((ns), BAD_CAST (s)); \
			xmlNewNs((node), BAD_CAST SCADA_ED_NS_URI, \
					BAD_CAST SCADA_ED_NS_PREFIX); \
			xmlDocSetRootElement((document), (node)); \
		} \
		else { \
			(node) = xmlNewChild((node), (ns), BAD_CAST (s), NULL); \
		} \
		xmlNewProp((node), BAD_CAST "id", BAD_CAST "1"); \
		++counter; \
	} while (0)


xmlXPathObjectPtr loadDataFromXpathNS(xmlChar* xpath, xmlDocPtr document,
		bool handle_new_file, t_variable_list *l_head)
{
	xmlXPathObjectPtr result = NULL;
	xmlChar *enhanced_xpath = process_variable(xpath, l_head);
	xmlChar *full_xpath = enhance_xpath(enhanced_xpath,
			BAD_CAST SCADA_ED_NS_PREFIX);

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

	/* avoid usage of SCADA_ED_NS_PREFIX and SCADA_ED_NS_URI because
	   of problems with non-existing items in half-filled PtTree in GUI */
	xmlNodePtr x = xmlDocGetRootElement(document);
	assert(x->ns != NULL);

	if (xmlXPathRegisterNs(context, x->ns->prefix, x->ns->href))
	{
		fprintf(stderr, "Error while registering namespace.\n");
		result = NULL;
	}
	else
	{
		result = xmlXPathEvalExpression(full_xpath, context);

		/* no result detected */
		if (result != NULL && xmlXPathNodeSetIsEmpty(result->nodesetval))
		{
			fprintf(stderr, "WARN: No results for XPath %s\n", full_xpath);
#ifndef NDEBUG
			//xmlDocFormatDump(stderr, document, 1);
			//htmlDocDump(stderr, document);
			//xmlDebugDumpDocument(stderr, document);
#endif
			xmlXPathFreeObject(result);
			result = NULL;
		}
	}

	xmlFree(full_xpath);
	xmlXPathFreeContext(context);

	return result;
}


xmlChar *process_variable(xmlChar *xpath, t_variable_list *l_head)
{
	const xmlChar *var_start = NULL;
	const xmlChar *var_end = NULL;
	xmlChar *enhanced_xpath = xpath;
	xmlChar dollar = '$';
	xmlChar *var_name = NULL;
	xmlChar *var_value = NULL;

	var_start = xmlStrchr(xpath, dollar);

	if (var_start != NULL)
		enhanced_xpath = xmlCharStrndup((const char*)xpath, var_start - xpath);
	else
		enhanced_xpath = xmlCharStrdup((const char*)xpath);

	while (var_start != NULL)
	{
		var_end = xmlStrchr(var_start +1, ']');

		if (var_end == NULL)
		{
			fprintf(stderr, "ERROR while replacing xml variable in xpath.\n");
			var_start = NULL;
		}
		else
		{
			var_name = xmlCharStrndup((const char *) var_start +1,
					var_end - var_start -1);
			var_value = NULL;
			t_variable_list *act = l_head;

			while (act != NULL)
			{
				if (act->name == NULL) continue;

				if (xmlStrEqual(act->name, var_name))
				{
					var_value = act->value;
					break;
				}

				act = act->next;
			}

			if (var_value != NULL)
			{
				enhanced_xpath = xmlStrcat(enhanced_xpath, var_value);
			}
			else
			{
				fprintf(stderr, "ERROR xls:variable \"%s\" not found.\n", var_name);
				xmlFree(var_name);
				return enhanced_xpath;
			}

			xmlFree(var_name);
			var_start = xmlStrchr(var_end +1, dollar);

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
xmlChar *enhance_xpath(const xmlChar *xpath, const xmlChar *namespace)
{
	xmlChar *ens;  /* enhanced namespace */
	xmlChar *sep;  /* separator          */
	xmlChar *fullpath;

	/* byte count from start of xpath to the current slash inclusive */
	int f2 = 1;

	ens = xmlCharStrdup("/");
	ens = xmlStrncat(ens, namespace, xmlStrlen(namespace));
	ens = xmlStrncat(ens, BAD_CAST ":", 1);

	fullpath = xmlStrdup(ens);

	sep = (xmlChar *)xmlStrchr(xpath, (xmlChar)'/');

	while (sep != NULL) {
		f2 = sep - xpath +1;
		sep = (xmlChar *)xmlStrchr(sep +1, (xmlChar)'/');
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
