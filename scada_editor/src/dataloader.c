/*
 * DataLoader.cpp
 *
 *  Created on: 4.5.2012
 *      Author: Casey
 */

#include "dataloader.h"

t_variable_list *first = NULL;
t_variable_list *last = NULL;

xmlDocPtr data;
xmlDocPtr view;

int parseFile(char *filename, char *viewname) {
	init();

	xmlInitParser();
	LIBXML_TEST_VERSION

	if (filename) {
		data = xmlParseFile(filename);
	} else {
		data = xmlNewDoc(BAD_CAST "1.0");
	}

	view = xmlParseFile(viewname);

	if (data == NULL) {
		fprintf(stderr, "Data file not parsed successfully. \n");
		return -1;
	}

	if (view == NULL) {
		fprintf(stderr, "CFG View not parsed successfully. \n");
		return -1;
	}
	fprintf(stderr, "1\n");
	loadViewAndData();

	destroy();
	xmlCleanupParser();
	return 0;
}

void loadViewAndData() {

	xmlNodePtr viewnode;

	viewnode = xmlDocGetRootElement(view);
	if (viewnode == NULL) {
		fprintf(stderr, "empty document\n");
		return;
	}

	if (xmlStrcmp(viewnode->name, (const xmlChar *) "config-view")) {
		fprintf(stderr, "document of the wrong type, root node != config-view");
		return;
	}

	viewnode = viewnode->xmlChildrenNode;
	viewnode = viewnode->next; //text node skip

	if (xmlStrcmp(viewnode->name, (const xmlChar *) "tree")) {
		fprintf(stderr, "document of the wrong type, tree node not found");
		return;
	}

	parseTree(viewnode);

}

void parseTree(xmlNodePtr tree) {

	xmlChar *name;
	PtTreeItem_t *last_item = NULL;
	PtTreeItem_t *item;
	xmlNodePtr tree_child = tree->xmlChildrenNode;

	while (tree_child != NULL) {
		if ((!xmlStrcmp(tree_child->name, (const xmlChar *) "tree-node"))) {

			name = xmlGetProp(tree_child, (const xmlChar *) "name");

			item = PtTreeAllocItem(ABW_tree_wgt, (char *) name, -1, -1);

			if (last_item != NULL) {
				PtTreeAddAfter(ABW_tree_wgt, item, last_item);
			} else {
				PtTreeAddFirst(ABW_tree_wgt, item, last_item);

			}

			parseTreeNode(item, tree_child);
			last_item = item;
			xmlFree(name);

		}
		tree_child = tree_child->next;

	}

}

void parseTreeNode(PtTreeItem_t *parent_item, xmlNodePtr tree_node) {
	xmlChar* name = NULL;
	xmlChar* source = NULL;
	xmlNodePtr tree_node_child = tree_node->xmlChildrenNode;

	name = xmlGetProp(tree_node, (const xmlChar *) "name");
	source = xmlGetProp(tree_node, (const xmlChar *) "source");


	while (tree_node_child != NULL) {

		printf("its a: %s\n", tree_node_child->name);

		if ((!xmlStrcmp(tree_node_child->name, (const xmlChar *) "table"))) {
			parent_item->data = createTable(tree_node_child);

		} else if (!xmlStrcmp(tree_node_child->name,
				(const xmlChar *) "tree-node")) {
			printf("its tree-node tag: %s\n", tree_node_child->name);
			parseTreeNode(parent_item, tree_node_child);
			parent_item->data = newTableData(NULL, tree_node_child);//FIXME dumblob

		} else if (!xmlStrcmp(tree_node_child->name,
				(const xmlChar *) "variable")) {

			printf("its a variable tag: %s\n", tree_node_child->name);

			xmlXPathObjectPtr result = loadDataFromXpath(source);
			xmlNodeSetPtr nodeset = NULL;

			if (result != NULL) {
				nodeset = result->nodesetval;
				PtTreeItem_t *last_item = parent_item;
				PtTreeItem_t *item;
				int i;
				xmlChar* attr = NULL;




				t_variable_list * this = (t_variable_list*) malloc(sizeof(t_variable_list));
				this->name = xmlGetProp(tree_node_child, (xmlChar*) "name");

				if(first == NULL){
					first = this;
					last = first;
				}else{
					last->next = this;
					last = last->next;
				}




				for (i = 0; i < nodeset->nodeNr; i++) {

					attr = xmlGetProp(nodeset->nodeTab[i],(xmlChar*) "id");

					this->value = attr;


					item = PtTreeAllocItem(ABW_tree_wgt, (char *) attr, -1, -1);

					if (last_item != parent_item) {
						PtTreeAddAfter(ABW_tree_wgt, item, last_item);
					} else {
						PtTreeAddFirst(ABW_tree_wgt, item, last_item);

					}

					last_item = item;
					xmlFree(this->value);


				}
				xmlFree(this->name);
				free(this);
			}

			break;

		} else {
			printf("its some tag: %s\n", tree_node_child->name);

		}
		tree_node_child = tree_node_child->next;

	}
	if (name != NULL) {
		xmlFree(name);
	}

}

void process_variable(xmlChar *xpath) {

	xmlChar *sep = NULL;

	sep = (xmlChar *)xmlStrchr(xpath, (xmlChar) '$');

	while(sep != NULL){



		sep = (xmlChar *)xmlStrchr(xpath, (xmlChar) '$');
	}


}


xmlChar *replace_str(xmlChar *xpath, xmlChar *variable, xmlChar *value)
{
  return NULL;
}




xmlXPathObjectPtr loadDataFromXpath(xmlChar* xpath) {

	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	printf("xpath: %s\n", xpath);

	xmlChar *full_xpath = enhance_xpath(xpath, (const xmlChar *) "disam");

	printf("full xpath: %s\n", full_xpath);

	context = xmlXPathNewContext(data);

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
		printf("No result detected\n");
		result = NULL;
	}

	xmlFree(full_xpath);
	xmlXPathFreeContext(context);

	return result;
}




t_table_data * createTable(xmlNodePtr node) {
	xmlChar* source = xmlGetProp(node, (const xmlChar *) "source");

	printf("its table source: %s .\n", source);
	xmlNodePtr column = node->xmlChildrenNode;

	PtWidget_t *tbl = NULL;
	PhPoint_t tblPos;
	PhDim_t tblDim;
	int columns_count = 0;
	int rows_count = 0;

	xmlNodeSetPtr nodeset = NULL;
	xmlXPathObjectPtr result;

	result = loadDataFromXpath(source);

	if (result != NULL) {
		nodeset = result->nodesetval;
		rows_count = nodeset->nodeNr;

	}

	while (column != NULL) {
		if ((!xmlStrcmp(column->name, (const xmlChar *) "column"))) {
			columns_count++;
		}
		column = column->next;
	}
	printf("its table columns and rows: %d %d\n", columns_count, rows_count);
	column = node->xmlChildrenNode;
	printf("all ok 0\n");
	tblPos.x = 000;
	tblPos.y = 000;
	tblDim.w = ABW_table_pane->area.size.w;
	tblDim.h = ABW_table_pane->area.size.h;

	tbl = tblInit(ABW_table_pane, tblPos, tblDim);
	tblSetSize(tbl, columns_count, rows_count + 1);

	int cell = 0;

	PtArg_t args[2];
	xmlChar* attr = NULL;
	xmlChar* tmp_s = NULL;
	xmlChar *label;
	t_xml_info *info = NULL;
	int i;
	while (column != NULL) {
		if ((!xmlStrcmp(column->name, (const xmlChar *) "column"))) {
			if ((info = (t_xml_info *)malloc(sizeof(t_xml_info))) == NULL)
				PtExit(EXIT_FAILURE);

			label        = xmlGetProp(column, (const xmlChar *) "label");
			info->source = xmlGetProp(column, (const xmlChar *) "source");
			tmp_s        = xmlGetProp(column, (const xmlChar *) "type");

			/* cfgview.xml <... type="bool" ...> */
			if      (strcmp((const char*)tmp_s, "number") == 0)
				info->type = SCADA_EDITOR_XML_ATTR_TYPE_NUMBER;
			else if (strcmp((const char*)tmp_s, "string") == 0)
				info->type = SCADA_EDITOR_XML_ATTR_TYPE_STRING;
			else if (strcmp((const char*)tmp_s, "char"  ) == 0)
				info->type = SCADA_EDITOR_XML_ATTR_TYPE_CHAR;
			else if (strcmp((const char*)tmp_s, "bool"  ) == 0)
				info->type = SCADA_EDITOR_XML_ATTR_TYPE_BOOL;
			else
			{
				fprintf(stderr, "ERROR: Unknown type=\"%s\" found in cfgview.xml!", tmp_s);
				/* recovery */
				info->type = SCADA_EDITOR_XML_ATTR_TYPE_STRING;
			}

			PtSetArg(&args[0], Pt_ARG_TEXT_STRING, label, 0);
			PtSetArg(&args[1], Pt_ARG_POINTER, info, 0); //FIXME dumblob
			tblExeOnCellArea(tbl, cell, 0, cell, 0, PtButton, 2, args);

			if (result != NULL) {
				for (i = 0; i < nodeset->nodeNr; i++) {
					//printf("keyword: %s\n", nodeset->nodeTab[i]->name);
					//printf("cs: %s\n", info->source +1);
					attr = xmlGetProp(nodeset->nodeTab[i], info->source +1);
					//printf("attr: %s\n", attr);
					PtSetArg(&args[0], Pt_ARG_TEXT_STRING, attr, 0);
					tblExeOnCellArea(tbl, cell, i + 1, cell, i + 1, PtText, 1, args);

					xmlFree(attr);
				}
			}

			cell++;
			xmlFree(label);
			xmlFree(tmp_s); //FIXME dumblob

		}
		column = column->next;
	}

	t_table_data *tbdt = (struct s_table_data *) malloc(sizeof(t_table_data));
	tbdt->table = tbl;
	tbdt->xpath = source;

	//xmlFree(source);
	xmlXPathFreeObject(result);

	return tbdt;

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

void init() {
	data = NULL;
	view = NULL;

}

void destroy() {
	if (data != NULL) {
		xmlFreeDoc(data);
	}
	if (view != NULL) {
		xmlFreeDoc(view);
	}
}

//FIXME dumblob
t_table_data *newTableData(PtWidget_t *tbl, xmlNodePtr node)
{
	t_table_data *data = (t_table_data *)malloc(sizeof(t_table_data));

	if (data == NULL) PtExit(EXIT_FAILURE);

	data->table = tbl;
	data->xpath = xmlGetProp(node, (const xmlChar *)"source");

	return data;
}

/* vim: set noexpandtab: */
