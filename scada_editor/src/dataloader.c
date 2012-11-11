/*
 *  Created on: 4.5.2012
 *      Author: Casey
 */

#include "dataloader.h"
#include "datasaver.h"
#include "xml_func.h"
#include "assert.h"
#include "global_vars.h"

extern struct scada_ed_global_vars_s scada_ed_global_vars;

/** one of these 2 args must be NULL (but not both)
 * uses scada_ed_global_vars */
int parseFile(char *filename, char *viewname)
{
	xmlDocPtr data = NULL;
	xmlDocPtr view = NULL;

	xmlInitParser();
	LIBXML_TEST_VERSION
printf("filename \"%s\"\n", filename);//FIXME debug
	if (filename == NULL)
	{
		assert(viewname != NULL);

		if ((view = xmlParseFile(viewname)) == NULL) {
			fprintf(stderr, "CFG View not parsed successfully.\n");
			return -1;
		}

		data = xmlNewDoc(BAD_CAST "1.0");
	}
	else
	{
		assert(viewname == NULL);

		/* construct the whole shell command */
		char *dst_postfix = ".xml";
		size_t script_len = strlen(scada_ed_global_vars.arg_conversion_script);
		size_t filename_len = strlen(filename);
		size_t dst_len = filename_len + strlen(dst_postfix);

		char *dst = (char *)malloc(sizeof(char) * (dst_len +1));
		if (dst == NULL) PtExit(EXIT_FAILURE);

		char *command = (char *)malloc(sizeof(char) * (
					/* "arg_conversion_script" "filename" "dst"\0 */
					1 + script_len + 3 + filename_len + 3 + dst_len + 2
					));
		if (command == NULL) PtExit(EXIT_FAILURE);

		/* dst construction */
		snprintf(dst, dst_len +1, "%s%s", filename, dst_postfix);

		/* final command construction */
		snprintf(command, 1 + script_len + 3 + filename_len + 3 + dst_len + 2,
				"\"%s\" \"%s\" \"%s\"", scada_ed_global_vars.arg_conversion_script,
				filename, dst);

		switch (WEXITSTATUS(system(command)))
		{
			/* filename was already xml */
			case 0:
				if ((data = xmlParseFile(filename)) == NULL) {
					fprintf(stderr, "Data file \"%s\" not parsed successfully.\n", filename);
					free(dst);
					free(command);
					return -1;
				}
				break;
			/* conversion success */
			case 1:
				if ((data = xmlParseFile(dst)) == NULL) {
					fprintf(stderr, "Data file \"%s\" not parsed successfully.\n", dst);
					free(dst);
					free(command);
					return -1;
				}
				break;
			default:
				fprintf(stderr, "Error occured during conversion script execution.\n");
				free(dst);
				free(command);
				return -1;
		}

		free(command);
		free(dst);

		//FIXME parse view found in parsed data file (if (! found) return -1;)
		char *dirname_end;
		char *s = getCfgviewNameFromData(data);

		if (s == NULL) return -1;

		if ((dirname_end = strrchr(filename, '/')) == NULL)
		{
			view = xmlParseFile(s);

			if ((view = xmlParseFile(s)) == NULL) return -1;
		}
		else
		{
			viewname = (char *)malloc(sizeof(char) *
					/* /some/path + / + s + \0 */
					(dirname_end - filename +1 + strlen(s) +1));
			if (viewname == NULL) PtExit(EXIT_FAILURE);

			/* dirname with end-slash from filename */
			*(dirname_end +1) = '\0';
			sprintf(viewname, "%s%s", filename, s);

			if ((view = xmlParseFile(viewname)) == NULL)
			{
				free(viewname);
				return -1;
			};

			free(viewname);
		}
	}

	loadViewAndData(view, data);

	if (data != NULL) xmlFreeDoc(data);
	if (view != NULL) xmlFreeDoc(view);

	xmlCleanupParser();
	return 0;
}


char *getCfgviewNameFromData(xmlDocPtr data)
{
	//FIXME
	data = data;

	//FIXME je to tady alokovane? => mam to potom uvolnit?
	return "cfgview.xml";
}


void loadViewAndData(xmlDocPtr view, const xmlDocPtr data)
{
	//FIXME kontrolovat major verzi (==) a minor verzi (minor_found <= hardcoded)
	//      dale kontrolovat, aby matchovaly verze cfgview.xml a tohoto datoveho
	//      + respektovat global_vars.viewpath pri exportu!!!

	xmlNodePtr viewnode = xmlDocGetRootElement(view);

	if (viewnode == NULL) {
		fprintf(stderr, "Empty document given.\n");
		return;
	}

	if (xmlStrcmp(viewnode->name, BAD_CAST "config-view")) {
		fprintf(stderr, "Document of wrong type (root node != config-view).\n");
		return;
	}

	viewnode = viewnode->xmlChildrenNode;
	viewnode = viewnode->next;  /* skip text node */

	if (xmlStrcmp(viewnode->name, BAD_CAST "tree")) {
		fprintf(stderr, "Document of wrong type (tree node not found).\n");
		return;
	}

	xmlNodePtr tree_child = viewnode->xmlChildrenNode;
	PtTreeItem_t *last_item = NULL;
	//FIXME doplnit, urcite to tak bude spravne
	/* (re)init list of variables (name + value) */
	assert(scada_ed_global_vars.l_head == NULL);
	scada_ed_global_vars.l_head = NULL;
	t_variable_list *l_end = NULL;

	while (tree_child != NULL)
	{
		/* there is an "text" element, which we don't need */
		if (! xmlStrcmp(tree_child->name, (const xmlChar *)"tree-node"))
			parseTreeNode(tree_child, viewnode, data, &last_item,
					&scada_ed_global_vars.l_head, &l_end);

		tree_child = tree_child->next;
	}
}


void parseTreeNode(xmlNodePtr tree_node, xmlNodePtr parent_node,
		const xmlDocPtr data, PtTreeItem_t **last_item,
		t_variable_list **l_head, t_variable_list **l_end)
{
	xmlChar* name = NULL;
	xmlChar* source = NULL;
	xmlNodePtr tree_node_child = NULL;

	PtTreeItem_t *item = NULL;

	name   = xmlGetProp(tree_node, BAD_CAST "name");
	source = xmlGetProp(tree_node, BAD_CAST "source");

	if (*l_head == NULL)
		fprintf(stderr, "GLOBAL LIST IS EMPTY !!!\n");//FIXME debug

	/* we are not nested (source attr does not exist) */
	if (source == NULL)
	{  /* {{{ */
		item = PtTreeAllocItem(ABW_tree_wgt, (char *)name, -1, -1);

		if ((!xmlStrcmp(parent_node->name, (const xmlChar *) "tree-node")))
		{
			PtTreeAddFirst(ABW_tree_wgt, item, *last_item);
		}
		else
		{
			if (*last_item == NULL)
				PtTreeAddFirst(ABW_tree_wgt, item, NULL);
			else
				PtTreeAddAfter(ABW_tree_wgt, item, *last_item);
		}

		*last_item = item;
		tree_node_child = tree_node->xmlChildrenNode;

		while (tree_node_child != NULL)
		{
			if (!xmlStrcmp(tree_node_child->name, BAD_CAST "table"))
			{
				item->data = createTable(tree_node_child, *l_head, data);
			}
			else if (!xmlStrcmp(tree_node_child->name, BAD_CAST "tree-node"))
			{
				parseTreeNode(tree_node_child, tree_node, data, last_item,
						l_head, l_end);
				item->data = newTableData(NULL,
						xmlGetProp(tree_node_child, BAD_CAST "source"), *l_head);
			}

			tree_node_child = tree_node_child->next;
		}
	} /* }}} */
	/* we are nested */
	else
	{  /* {{{ */
		xmlXPathObjectPtr result = loadDataFromXpathNS(source, data,
				(scada_ed_global_vars.filepath == NULL) ? true : false, *l_head);
		xmlNodeSetPtr nodeset = NULL;

		/* non-empty items */
		if (result != NULL)
		{
			nodeset = result->nodesetval;
			int i;
			xmlChar* attr = NULL;
			char have_variable = 0;

			/* FIXME order in this array is unspecified! */
			for (i = 0; i < nodeset->nodeNr; i++) {
				attr = xmlGetProp(nodeset->nodeTab[i], name +1);
				assert(attr != NULL);
				item = PtTreeAllocItem(ABW_tree_wgt, (char *)attr, -1, -1);

				if (! xmlStrcmp(parent_node->name, (const xmlChar *)"tree-node"))
				{
					PtGenTreeItem_t *gen = ((PtGenTreeItem_t *)(*last_item))->son;

					/* add as son (before any existing son, if any) */
					if (gen == NULL)
					{
						PtTreeAddFirst(ABW_tree_wgt, item, *last_item);
					}
					/* find last brother item of sublist */
					else
					{
						while (gen->brother != NULL) gen = gen->brother;

						PtTreeAddAfter(ABW_tree_wgt, item, (PtTreeItem_t *)gen);
					}
				}
				else
				{
					/* tree widget is empty, we found first item */
					if (*last_item == NULL)
						/* add to the root of the given tree widget */
						PtTreeAddFirst(ABW_tree_wgt, item, NULL);
					else
						/* add as brother to last_item */
						PtTreeAddAfter(ABW_tree_wgt, item, *last_item);
				}

				PtTreeExpand(ABW_tree_wgt, *last_item, NULL);
				PtTreeItem_t *tmpitem = *last_item;
				/* temporary reset for subtree(s) building */
				*last_item = item;
				have_variable = 0;

				t_variable_list *l_end_tmp = *l_end;
				tree_node_child = tree_node->xmlChildrenNode;

				while (tree_node_child != NULL)
				{
					if (! xmlStrcmp(tree_node_child->name, BAD_CAST "table"))
					{
						item->data = createTable(tree_node_child, *l_head, data);
					}
					else if (! xmlStrcmp(tree_node_child->name, BAD_CAST "tree-node"))
					{
						parseTreeNode(tree_node_child, tree_node, data, last_item,
								l_head, l_end);
						item->data = newTableData(NULL, xmlGetProp(tree_node_child,
									BAD_CAST "source"), *l_head);
					}
					else if (! xmlStrcmp(tree_node_child->name, BAD_CAST "variable"))
					{
						have_variable = 1;
						/* {{{ list_add_to_end() */
						t_variable_list *this = (t_variable_list *)malloc(
								sizeof(t_variable_list));

						if (this == NULL) PtExit(EXIT_FAILURE);

						this->name = xmlGetProp(tree_node_child, BAD_CAST "name");
						this->value = xmlStrdup(attr);
						this->next = NULL;

						if (*l_head == NULL)
						{
							*l_head = this;
							*l_end = this;
						}
						else
						{
							(*l_end)->next = this;
							*l_end = this;
						}
						/* }}} */
					}

					tree_node_child = tree_node_child->next;
				}

				*last_item = tmpitem;

				if (have_variable) {
					/* nothing new added */
					if (*l_head == *l_end) *l_head = NULL;

					free((*l_end)->name);
					free((*l_end)->value);
					free(*l_end);
					*l_end = l_end_tmp;
				}

				xmlFree(attr);
			}
		}
	}  /* }}} */

	if (name != NULL) xmlFree(name);
	if (source != NULL) xmlFree(source);
}


int setTypeAndContentOfCell(PtWidget_t *tbl, int col, int row,
		const char *attr, t_xml_attr_type info_type)
{
	int c = 0;
	PtWidgetClassRef_t *class;
	PtArg_t args[3];

	switch (info_type)
	{
		case SCADA_ED_XML_ATTR_TYPE_NUMBER:
			PtSetArg(&args[c++], Pt_ARG_NUMERIC_VALUE,
					strtol((attr == NULL) ? "0" : (const char *)attr, NULL, 10), 0);
			class = PtNumericInteger;
			break;

		case SCADA_ED_XML_ATTR_TYPE_CHAR:
			PtSetArg(&args[c++], Pt_ARG_NUMERIC_VALUE,
					strtol((attr == NULL) ? "0" : (const char *)attr, NULL, 16), 0);
			PtSetArg(&args[c++], Pt_ARG_NUMERIC_PREFIX, "0x", 0);
			PtSetArg(&args[c++], Pt_ARG_NUMERIC_FLAGS, Pt_TRUE, Pt_NUMERIC_HEXADECIMAL);
			class = PtNumericInteger;
			break;

		case SCADA_ED_XML_ATTR_TYPE_BOOL:
			PtSetArg(&args[c++], Pt_ARG_FLAGS,
					(attr == NULL || *attr == '\0' || *attr == '0') ? Pt_FALSE : Pt_TRUE, Pt_SET);
			class = PtToggleButton;
			break;

		/* SCADA_ED_XML_ATTR_TYPE_STRING */
		default:
			PtSetArg(&args[c++], Pt_ARG_TEXT_STRING, attr, 0);
			class = PtText;
	}

	return tblExeOnCellArea(tbl, col, row, col, row, class, c, args);
}


int setHeaderCell(PtWidget_t *tbl, int x, int y, PtWidgetClassRef_t *class,
		const char *label, void *ptr)
{
	PtArg_t args[6];
	int c = 0;

	PtSetArg(&args[c++], Pt_ARG_TEXT_STRING, label, 0);
	PtSetArg(&args[c++], Pt_ARG_POINTER, ptr, 0);
	PtSetArg(&args[c++], Pt_ARG_FLAGS, Pt_FALSE, Pt_GETS_FOCUS);
	PtSetArg(&args[c++], Pt_ARG_SECONDARY_H_ALIGN, Pt_LEFT, 0);
	PtSetArg(&args[c++], Pt_ARG_LABEL_FLAGS, Pt_TRUE, Pt_SHOW_BALLOON);
	PtSetArg(&args[c++], Pt_ARG_BALLOON_POSITION, Pt_BALLOON_BOTTOM, 0);

	return tblExeOnCellArea(tbl, x, y, x, y, class, c, args);
}


t_table_data *createTable(xmlNodePtr node, t_variable_list *l_head,
		const xmlDocPtr data)
{
	PtWidget_t *tbl = NULL;
	PhPoint_t tblPos;
	PhDim_t tblDim;
	int columns_count = 0;
	int rows_count = 0;
	xmlChar *source = xmlGetProp(node, BAD_CAST "source");

	xmlNodeSetPtr nodeset = NULL;
	xmlXPathObjectPtr result;

	result = loadDataFromXpathNS(source, data, false, l_head);

	if (result == NULL) {
		if (scada_ed_global_vars.filepath != NULL)
			fprintf(stderr, "WARNING: Empty tree item found in XML node \"%s\".\n"
					"source|%s\n", node->name, (char *)source); //FIXME debug
	}
	else {
		nodeset = result->nodesetval;
		rows_count = nodeset->nodeNr;
	}

	xmlNodePtr column = node->xmlChildrenNode;

	while (column != NULL) {
		if (!xmlStrcmp(column->name, BAD_CAST "column")) columns_count++;

		column = column->next;
	}

	column = node->xmlChildrenNode;

	tblPos.x = 0;
	tblPos.y = 0;
	tblDim.w = ABW_table_pane->area.size.w;
	tblDim.h = ABW_table_pane->area.size.h;

	tbl = tblInit(ABW_table_pane, tblPos, tblDim);
	tblSetSize(tbl, columns_count, rows_count + 1);

	xmlChar *attr = NULL;
	xmlChar *tmp_s = NULL;
	xmlChar *label;

	t_xml_info *info = NULL;
	int cell = 0;

	while (column != NULL) {
		if ((!xmlStrcmp(column->name, BAD_CAST "column")))
		{
			if ((info = (t_xml_info *)malloc(sizeof(t_xml_info))) == NULL)
				PtExit(EXIT_FAILURE);

			info->source = xmlGetProp(column, BAD_CAST "source");
			label        = xmlGetProp(column, BAD_CAST "label");
			tmp_s        = xmlGetProp(column, BAD_CAST "type");

			/* cfgview.xml <... type="bool/number/char/..." ...> */
			if      (strcmp((const char *)tmp_s, "number") == 0)
				info->type = SCADA_ED_XML_ATTR_TYPE_NUMBER;
			else if (strcmp((const char *)tmp_s, "char"  ) == 0)
				info->type = SCADA_ED_XML_ATTR_TYPE_CHAR;
			else if (strcmp((const char *)tmp_s, "bool"  ) == 0)
				info->type = SCADA_ED_XML_ATTR_TYPE_BOOL;
			/* "string" */
			else
			{
				if (strcmp((const char *)tmp_s, "string") != 0)
					fprintf(stderr, "ERROR: Unknown type \"%s\" found in cfgview.xml!",
							tmp_s);

				info->type = SCADA_ED_XML_ATTR_TYPE_STRING;
			}

			setHeaderCell(tbl, cell, 0, PtButton, (char *)label, info);
printf("HOWK 33\n");//FIXME debug

			if (result != NULL)
			{
				int i;
				for (i = 0; i < nodeset->nodeNr; i++) {
printf("HOWK 77\n");//FIXME debug
					if ((attr = xmlGetProp(nodeset->nodeTab[i], info->source +1)) == NULL)
					{
						fprintf(stderr, "ERROR: Missing attribute \"%s\" in %s.\n",
								info->source +1, nodeset->nodeTab[i]->name);
					}

					setTypeAndContentOfCell(tbl, cell, i +1, (const char *)attr, info->type);
					xmlFree(attr);
				}
			}

			cell++;
			xmlFree(label);
			xmlFree(tmp_s);
		}

		column = column->next;
	}

	xmlXPathFreeObject(result);

	return newTableData(tbl, source, l_head);
}


t_table_data *newTableData(PtWidget_t *tbl, xmlChar *xpath,
		t_variable_list *l_head)
{
	t_table_data *data = (t_table_data *)malloc(sizeof(t_table_data));

	if (data == NULL) PtExit(EXIT_FAILURE);

	data->table = tbl;
	data->xpath = xpath;
	data->enhanced_xpath = process_variable(xpath, l_head);

	return data;
}

/* vim: set noexpandtab: */
