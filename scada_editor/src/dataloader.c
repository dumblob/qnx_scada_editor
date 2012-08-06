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

PtTreeItem_t *last_item = NULL;

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
	//loadDataFromXpath("/configuration/simatic-apl-data/symatic-type[@id=02]/simatic-subtype[@id=021]/simatic-item");

}

void parseTree(xmlNodePtr tree) {

	xmlNodePtr tree_child = tree->xmlChildrenNode;

	//void (*funcptr)(xmlNodePtr, xmlNodePtr) = NULL;

	printf("its a tree tag: %s\n", tree->name);

	while (tree_child != NULL) {
		printf("child of tree is: %s\n", tree_child->name);

		if (!xmlStrcmp(tree_child->name, (const xmlChar *) "tree-node")) {
			parseTreeNode(tree_child, tree);
		}

		tree_child = tree_child->next;
	}

}
/*
void (*parseTagPtr(xmlNodePtr next_node))(xmlNodePtr,xmlNodePtr) {

	void (*funcptr)(xmlNodePtr, xmlNodePtr) = NULL;

	if (next_node == NULL) {
		return NULL;
	}

	if ((!xmlStrcmp(next_node->name, (const xmlChar *) "tree-node"))) {
		funcptr = &parseTreeNode;

	} else if ((!xmlStrcmp(next_node->name, (const xmlChar *) "table"))) {
		funcptr = &parseTableNode;

	} else if ((!xmlStrcmp(next_node->name,	(const xmlChar *) "variable"))) {
		funcptr = &parseVarNode;

	}
	return funcptr;

}
*/

void parseTreeNode(xmlNodePtr tree_node, xmlNodePtr parent_node) {

	xmlChar* name = NULL;
	xmlChar* source = NULL;
	xmlNodePtr tree_node_child = NULL;
	//void (*funcptr)(xmlNodePtr, xmlNodePtr) = NULL;

	PtTreeItem_t *item = NULL;
	PtTreeItem_t *tmpitem = NULL;


	name = xmlGetProp(tree_node, (const xmlChar *) "name");
	source = xmlGetProp(tree_node, (const xmlChar *) "source");


	if(source != NULL){
		xmlXPathObjectPtr result = loadDataFromXpath(source);
		xmlNodeSetPtr nodeset = NULL;
		if (result != NULL) {
			nodeset = result->nodesetval;
			int i;
			xmlChar* attr = NULL;
			char have_variable = 0;

			for (i = 0; i < nodeset->nodeNr; i++) {
				attr = xmlGetProp(nodeset->nodeTab[i], name+1);
				item = PtTreeAllocItem(ABW_tree_wgt, (char *) attr, -1, -1);

				printf("loaded tree-mode name is %s\n", attr);

				/*pridava itemy korektne ale v opacnem poradi - opravit
				 * pridat volani pro children
				 * nejak predat hodnotu pro variable*/
				if ((!xmlStrcmp(parent_node->name, (const xmlChar *) "tree-node"))) {
					PtTreeAddFirst(ABW_tree_wgt, item, last_item);
				}else{
					if (last_item != NULL) {
						PtTreeAddAfter(ABW_tree_wgt, item, last_item);
					} else {
						PtTreeAddFirst(ABW_tree_wgt, item, last_item);
					}
				}
				PtTreeExpand(ABW_tree_wgt, last_item, NULL);
				tmpitem = last_item;
				last_item = item;
				have_variable = 0;

				t_variable_list * last_tmp = last;

				tree_node_child = tree_node->xmlChildrenNode;

				while (tree_node_child != NULL) {

					printf("its a: %s\n", tree_node_child->name);

					if ((!xmlStrcmp(tree_node_child->name,(const xmlChar *) "table"))) {
						item->data = createTable(tree_node_child);
					} else if (!xmlStrcmp(tree_node_child->name, (const xmlChar *) "tree-node")) {
						parseTreeNode(tree_node_child, tree_node);
					} else if (!xmlStrcmp(tree_node_child->name,(const xmlChar *) "variable")) {
						have_variable = 1;
						parseVarNode(tree_node_child, attr);
					}

					tree_node_child = tree_node_child->next;

				}
				last_item = tmpitem;

				if(have_variable){
					if(first == last){
						first = NULL;
					}

					free(last->name);
					free(last->value);
					free(last);

					last = last_tmp;
				}
				xmlFree(attr);
			}
		}


	} else {

		tree_node_child = tree_node->xmlChildrenNode;

		item = PtTreeAllocItem(ABW_tree_wgt, (char *) name, -1, -1);


		if ((!xmlStrcmp(parent_node->name, (const xmlChar *) "tree-node"))) {
				PtTreeAddFirst(ABW_tree_wgt, item, last_item);
			}else{
				if (last_item != NULL) {
					PtTreeAddAfter(ABW_tree_wgt, item, last_item);
				} else {
					PtTreeAddFirst(ABW_tree_wgt, item, last_item);
				}
			}

			last_item = item;



			while (tree_node_child != NULL) {

				printf("its a: %s\n", tree_node_child->name);
				/*funcptr = parseTagPtr(tree_node_child);

				 if (funcptr != NULL) {
					 (*funcptr)(tree_node_child, tree_node);
				 }*/

				if ((!xmlStrcmp(tree_node_child->name,(const xmlChar *) "table"))) {
					item->data = createTable(tree_node_child);
				} else if (!xmlStrcmp(tree_node_child->name, (const xmlChar *) "tree-node")) {
					parseTreeNode(tree_node_child, tree_node);
				}

				tree_node_child = tree_node_child->next;

			}

	}




	if (name != NULL) {
		xmlFree(name);
	}

	if (source != NULL) {
		xmlFree(source);
	}

}


void parseVarNode(xmlNodePtr node, xmlChar * variable) {
	//return bo to neni odladene na novy kod - musi mit podporu z fce parseTreeNode
	//bude asi registrovat jen jednu hodnotu z rodicovskeho tagu a pak se sama smaze
	//return;

	xmlChar* name = NULL;
	xmlChar* select = NULL;


	//name = xmlGetProp(node, (const xmlChar *) "name");
	//select = xmlGetProp(node, (const xmlChar *) "select");
	//parent_name = xmlGetProp(parent_node, (const xmlChar *) "name");

	printf("its a variable tag: %s\n", node->name);


	//stary kod, predelat - polozky do menu ted korekte pridava parseTreeNode
	//algoritmus - vyrobit promenou, ulozit hodnotu , tu by mel pak odstranit konec fce parseTreeNode
	//dalsi moznost ze dostane od parseTreeNode cely resultset z xpath a s tim bude pracovat, v tom pripade si sama fce bude ridit tvorbu , hodnotu a mazani aktualni promene
	//promyslet!!!!
	t_variable_list * this = (t_variable_list*) malloc(sizeof(t_variable_list));
	this->name = xmlGetProp(node, (xmlChar*) "name");
	this->value = xmlStrdup(variable);
	this->next = NULL;

	printf("le wild name: %s\n", this->name);

	if (first == NULL) {
		first = this;
		last = first;
	} else {
		last->next = this;
		last = last->next;
	}


	if (name != NULL) {
		xmlFree(name);
	}



	if (select != NULL) {
		xmlFree(select);
	}

}


xmlChar * process_variable(xmlChar *xpath) {

	const xmlChar *var_start = NULL;
	const xmlChar *var_end = NULL;
	xmlChar *enhanced_xpath = xpath;
	xmlChar dolar = '$';
	xmlChar *var_name = NULL;
	xmlChar *var_value = NULL;

	printf("processing xpath : %s\n", xpath);

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
			printf("varname found %s \n", var_name);

			var_value = get_variable_value(var_name);

			if (var_value != NULL) {
				printf("var_value %s \n", var_value);
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
		printf("replaced x path %s \n", enhanced_xpath);
		xmlFree(enhanced_xpath);
	}

}

xmlChar * get_variable_value(xmlChar * var_name) {

	printf("searching value for %s\n", var_name);

	t_variable_list *act = first;
			//printf("le name is %s\n", act->name);

	while (act != NULL) {
		if (act->name == NULL) {
			printf("le name is null\n");
			continue;
		} else {
			printf("le name is %s\n", act->name);
		}

		if (xmlStrEqual(act->name, var_name)) {
			printf("found\n");
			return act->value;
		}

		act = act->next;

	}

	printf("not found\n");
	return NULL;

}






xmlXPathObjectPtr loadDataFromXpath(xmlChar* xpath) {

	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	printf("xpath: %s\n", xpath);

	xmlChar * enhanced_xpath = process_variable(xpath);

	printf("enhanced_xpath: %s\n", enhanced_xpath);

	xmlChar *full_xpath = enhance_xpath(enhanced_xpath, (const xmlChar *) "disam");

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
	} else {
		printf("have results oh yeah\n");
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
