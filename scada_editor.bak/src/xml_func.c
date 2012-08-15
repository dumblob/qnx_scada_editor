#include "xml_func.h"

t_variable_list *first = NULL;
t_variable_list *last = NULL;

xmlXPathObjectPtr loadDataFromXpathNS(xmlChar* xpath, xmlDocPtr document) {

	xmlXPathContextPtr context;
	xmlXPathObjectPtr result = NULL;
	xmlChar * enhanced_xpath = process_variable(xpath);
	xmlChar *full_xpath = enhance_xpath(enhanced_xpath, (const xmlChar *) "disam");

	context = xmlXPathNewContext(document);
	if (context == NULL) {
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}


	if (xmlXPathRegisterNs(context, (const xmlChar *) "disam",
				(const xmlChar *) "http://www.disam.cz/Xmlns/Scada/Configuration")
				!= 0) {
			printf("error when namespace registering");
			return NULL;
	};


	result = xmlXPathEvalExpression(full_xpath, context);

	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		//printf("No result detected\n"); //FIXME
		result = NULL;
	//} else {
	//	printf("have results oh yeah\n"); //FIXME
	}

	xmlFree(full_xpath);
	xmlXPathFreeContext(context);

	return result;

}

xmlChar * get_variable_value(xmlChar * var_name) {
	t_variable_list *act = first;
		while (act != NULL) {
		if (act->name == NULL) {
			continue;
		}

		if (xmlStrEqual(act->name, var_name)) {
			return act->value;
		}

		act = act->next;

	}
	return NULL;
}


xmlChar * process_variable(xmlChar *xpath) {

	const xmlChar *var_start = NULL;
	const xmlChar *var_end = NULL;
	xmlChar *enhanced_xpath = xpath;
	xmlChar dolar = '$';
	xmlChar *var_name = NULL;
	xmlChar *var_value = NULL;

	//printf("processing xpath : %s\n", xpath);  //FIXME

	var_start = xmlStrchr(xpath, dolar);

	if (var_start != NULL) {

		enhanced_xpath = xmlCharStrndup((const char*) xpath, var_start	- xpath);
	}

	while (var_start != NULL) {
		var_end = xmlStrchr(var_start + 1, ']');

		if (var_end == NULL) {
			printf("error when xml variable replacing in xpath \n");
			var_start = NULL;
		} else {

			var_name = xmlCharStrndup((const char *) var_start + 1, var_end - var_start - 1);
			//printf("varname found %s \n", var_name); //FIXME

			var_value = get_variable_value(var_name);

			if (var_value != NULL) {
				//printf("var_value %s \n", var_value); //FIXME
				enhanced_xpath = xmlStrcat(enhanced_xpath, var_value);

			} else {
				printf("error - xls:variable %s not found. \n",
						var_name);
				return xpath;
			}

			var_start = xmlStrchr(var_end + 1, dolar);
			xmlFree(var_name);

			if (var_start == NULL) {
				enhanced_xpath = xmlStrcat(enhanced_xpath, var_end);
			} else {
				enhanced_xpath = xmlStrncat(enhanced_xpath, var_end, var_start - var_end);
			}
		}

	}

	return enhanced_xpath;

	if (enhanced_xpath != NULL) {
		//printf("replaced x path %s \n", enhanced_xpath); //FIXME
		xmlFree(enhanced_xpath);
	}

}

xmlChar* enhance_xpath(const xmlChar *xpath, const xmlChar * namespace) {
	xmlChar *ens;
	xmlChar *sep;
	xmlChar *fullpath;

	int f2 = 1;

	ens = xmlCharStrdup("/");
	ens = xmlStrncat(ens, namespace, xmlStrlen(namespace));
	ens = xmlStrncat(ens, (const xmlChar *) ":", 1);

	fullpath = xmlCharStrdup((const char *) ens);

	sep = (xmlChar *)xmlStrchr(xpath, (xmlChar) '/');
	while (sep != NULL) {
		f2 = sep - xpath + 1;
		sep = (xmlChar *)xmlStrchr(sep + 1, (xmlChar) '/');
		if (sep != NULL) {
			//printf("/// %d //\n", sep - xpath - f2 - 1);
			fullpath = xmlStrncat(fullpath, xpath + f2, sep - xpath - f2);
			fullpath = xmlStrncat(fullpath, ens, xmlStrlen(ens));
		} else {
			fullpath = xmlStrncat(fullpath, xpath + f2, xmlStrlen(xpath + f2));

		}
		//printf("/// %s //\n", fullpath);
	}

	//printf("/// %s //\n", fullpath);
	xmlFree(ens);

	return fullpath;
}
