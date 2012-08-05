#include "datasaver.h"
#include "dataloader.h"

extern char *filepath;

void save_data() {

	generateXMLfromTree();
	return;

	FILE *fp;

	fp = fopen(filepath, "w");
	if (fp != NULL) {

		/*save data to xml here!*/

		fclose(fp);
	} else {
		fprintf(stderr, "\nFile open error.\n");
	}

}

void generateXMLfromTree() {

	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	xmlAttrPtr newattr;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "disam:configuration");
	xmlDocSetRootElement(doc, root_node);
	xmlNewProp(root_node, BAD_CAST "id", BAD_CAST "kom_map");
	xmlNewProp(root_node, BAD_CAST "version", BAD_CAST "1.0");
	xmlNewProp(root_node, BAD_CAST "xmlns:disam",
			BAD_CAST "http://www.disam.cz/Xmlns/Scada/Configuration");

	PtTreeItem_t *item, **buf;
	PtWidget_t *tbl = NULL;
	t_table_data *tbldt = NULL;

	buf = PtTreeAllItems(ABW_tree_wgt, NULL);
	int i = 0;
	for (i = 0; (item = buf[i]) != NULL; ++i) {
		printf("nulling ");
		if (item->data != NULL) {
			tbldt = item->data;
			tbl = tbldt->table;
			save_table(tbl, tbldt->xpath, root_node);

		}
	}

	free(buf);
	xmlSaveFormatFileEnc("test.xml", doc, "UTF-8", 2);
	xmlFreeDoc(doc);
	xmlCleanupParser();
}

void save_table(PtWidget_t *table, xmlChar *xpath, xmlNodePtr *root_node) {

	xmlChar *nodename = NULL;
	xmlNodePtr *lastnode = root_node;
	xmlNodePtr *table_cfg = NULL;
	int f2 = 1;
	xmlChar *sep;

	xmlDocPtr view = xmlParseFile("cfgview.xml");
	xmlXPathContextPtr context;

	context = xmlXPathNewContext(view);

	if (xmlXPathRegisterNs(
			context,
			(const xmlChar *) "disam",
			(const xmlChar *) "http://www.disam.cz/Xmlns/Scada/ConfigEditor/Layout")
			!= 0) {
		printf("error ns ");

	};

	//xmlChar *tablepath = BAD_CAST "//disam:table[@source=";
	xmlChar *tablepath;
	tablepath = xmlCharStrdup("//disam:table[@source='");

	tablepath = xmlStrcat(tablepath, xpath);
	tablepath = xmlStrcat(tablepath, "']");

	printf("%s \n", tablepath);

	xmlXPathObjectPtr result;
	xmlNodeSetPtr nodeset;

	result = xmlXPathEvalExpression(tablepath, context);

	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		printf("No result\n");
		result = NULL;
	}

	if (result != NULL) {
		nodeset = result->nodesetval;
		printf("Results is: %d\n", nodeset->nodeNr);
		table_cfg = nodeset->nodeTab[0];
	}

	xmlChar* source;

	sep = xmlStrchr(xpath + 1, (xmlChar) '/');
	while (sep != NULL) {
		nodename = xmlCharStrdup("disam:");
		f2 = sep - xpath + 1;
		sep = xmlStrchr(sep + 1, (xmlChar) '/');
		if (sep != NULL) {
			nodename = xmlStrncat(nodename, xpath + f2, sep - xpath - f2);
			lastnode = xmlNewChild(lastnode, NULL, BAD_CAST nodename, NULL);
		} else {
			nodename = xmlStrncat(nodename, xpath + f2, xmlStrlen(xpath + f2));
			lastnode = xmlNewChild(lastnode, NULL, BAD_CAST nodename, NULL);

			xmlNodePtr column = nodeset->nodeTab[0]->xmlChildrenNode;
			int clmn = 0;
			while (column != NULL) {
				if ((!xmlStrcmp(column->name, (const xmlChar *) "column"))) {

					source = xmlGetProp(column, (const xmlChar *) "source");
					char *cell_text;
					tblGetCellResource((PtWidget_t *)table, clmn, 1, Pt_ARG_TEXT_STRING, &cell_text, 0);
					printf("OBSAH cell 0,0: %s\n", cell_text);

					xmlNewProp(lastnode, source+1 , BAD_CAST cell_text);

					xmlFree(source);
					clmn++;
				}
				column = column->next;
			}

		}
		xmlFree(nodename);

	}

}
