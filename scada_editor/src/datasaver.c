/*
 * Casey & Jan Pacner
 * 2012-10-28 18:22:18 CET
 */

#include "datasaver.h"
#include "dataloader.h"
#include "xml_func.h"
#include "global_vars.h"

extern struct scada_ed_global_vars_s scada_ed_global_vars;







// FIXME respektovat global_vars.viewpath pri exportu!!!









/** uses scada_ed_global_vars */
void save_data()
{
  xmlNodePtr root_node = NULL;
  xmlDocPtr save_doc = xmlNewDoc(BAD_CAST "1.0");

  /* FIXME some magic ;), do not touch! */
  xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar *)SCADA_ED_NS_URI,
      (const xmlChar *)SCADA_ED_NS_PREFIX);
  root_node = xmlNewNode(ns, BAD_CAST "configuration");
  ns = xmlNewNs(root_node, (const xmlChar *)SCADA_ED_NS_URI,
      (const xmlChar *)SCADA_ED_NS_PREFIX);

  //FIXME ten namespace je OK?
  xmlDocSetRootElement(save_doc, root_node);
  xmlNewProp(root_node, BAD_CAST "id", BAD_CAST "kom_map");
  xmlNewProp(root_node, BAD_CAST "version", BAD_CAST
      SCADA_ED_COMPAT_MAJOR "." SCADA_ED_COMPAT_MINOR);
  xmlNewProp(root_node, BAD_CAST "config-view", BAD_CAST "cfgview.xml");

  PtGenTreeItem_t *gen = (PtGenTreeItem_t *)PtTreeRootItem(ABW_tree_wgt);
  walkOverTreeBranch(gen, save_doc, ns);

  xmlSaveFormatFileEnc(scada_ed_global_vars.filepath, save_doc, "UTF-8", 2);

  xmlFreeDoc(save_doc);
  xmlCleanupParser();
}


void exportToSrc(char *path)
{
	FILE *fp = fopen(path, "w");

	if (fp != NULL)
	{
		generateSrcFromTree(fp);
		fclose(fp);
	}
	else
	{
		fprintf(stderr, "\nFile open error.\n");
	}
}


void walkOverTreeBranch(PtGenTreeItem_t *gen, xmlDocPtr save_doc, xmlNsPtr ns)
{
	while (gen != NULL)
	{
		/* t_table_data */
		assert(((PtTreeItem_t *)gen)->data != NULL);
		assert(((t_table_data *)((PtTreeItem_t *)gen)->data)->xpath != NULL);

		generateXML(((PtTreeItem_t *)gen)->data, save_doc, ns);

		if (gen->son != NULL) walkOverTreeBranch(gen->son, save_doc, ns);

		gen = gen->brother;
	}
}

/** uses scada_ed_global_vars */
void generateXML(t_table_data *data, xmlDocPtr save_doc, xmlNsPtr ns)
{
	xmlXPathObjectPtr result = NULL;
	xmlNodeSetPtr nodeset;
	xmlNodePtr lastnode = NULL;
	xmlChar *nodename = NULL;
	xmlChar* source;
	int f2 = 1;

	xmlDocPtr view = xmlParseFile(scada_ed_global_vars.viewpath);

	xmlChar *enhanced_xpath = data->enhanced_xpath;
	xmlChar *xpath = data->xpath;

	xmlChar *sep = enhanced_xpath;
	xmlChar *last_exist_path = xmlCharStrdup("/");
	int new_prop = 0;

	while (sep != NULL) {

		f2 = sep - enhanced_xpath + 1;
		sep = (xmlChar *)xmlStrchr(sep + 1, (xmlChar) '/');
		if (sep != NULL) {
			nodename = xmlStrndup(enhanced_xpath + f2, sep - enhanced_xpath - f2);
			last_exist_path = xmlStrcat(last_exist_path,nodename);

			if (!new_prop) {
				result = loadDataFromXpathNS(last_exist_path, save_doc, false,
            scada_ed_global_vars.l_head);
			} else {
				result = NULL;
			}

			if (result != NULL){
				/* node exists => load node to lastnode and skip */
				nodeset = result->nodesetval;
				lastnode = nodeset->nodeTab[0];
        assert(nodeset->nodeNr <= 1);
			} else {
				new_prop = 1;
				lastnode = process_node(lastnode, nodename, ns);
			}

			last_exist_path = xmlStrcat(last_exist_path,(xmlChar *)"/");
		} else {
			if (data->table == NULL) {
				printf("no table data\n");
				continue;
			}

			nodename = xmlStrndup(enhanced_xpath + f2, xmlStrlen(enhanced_xpath + f2));
			//lastnode = process_node(lastnode, nodename, ns);//FIXME

			xmlXPathContextPtr context = xmlXPathNewContext(view);

			if (xmlXPathRegisterNs(
					context,
					BAD_CAST "disam",
					BAD_CAST "http://www.disam.cz/Xmlns/Scada/ConfigEditor/Layout")
					!= 0) {
				fprintf(stderr, "ERROR while registering namespace.");
				continue;
			};

			xmlChar *tablepath;
			tablepath = xmlCharStrdup("//disam:table[@source='");

			tablepath = xmlStrcat(tablepath, xpath);
			tablepath = xmlStrcat(tablepath, (const xmlChar *)"']");

			result = xmlXPathEvalExpression(tablepath, context);

			if (result == NULL){
				printf("no table data \n");
				continue;
			}

			if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
					xmlXPathFreeObject(result);
					continue;
			}

			nodeset = result->nodesetval;

			xmlNodePtr column;
			xmlNodePtr node;
			int clmn;

			int rows = tblLastRow((PtWidget_t *)data->table);
			int r;

			for (r = 1; r <= rows; ++r)
			{
				column = nodeset->nodeTab[0]->xmlChildrenNode;
				clmn = 0;
				node = process_node(lastnode, nodename, ns);

				while (column != NULL)
				{
					if ((!xmlStrcmp(column->name, BAD_CAST "column")))
					{
						source = xmlGetProp(column, BAD_CAST "source");
						char *cell_text;
						tblGetCellResource((PtWidget_t *)data->table, clmn, r,
                Pt_ARG_TEXT_STRING, &cell_text, 0);

						xmlNewProp(node, source+1 , BAD_CAST cell_text);

						xmlFree(source);
						clmn++;
					}

					column = column->next;
				}
			}
		}

		xmlFree(nodename);
	}
}

xmlNodePtr process_node(xmlNodePtr lastnode, xmlChar *nodename, xmlNsPtr ns)
{
	xmlNodePtr tmp;

	if (node_have_attribude(nodename))
	{
		xmlChar *attrname = getAttrNameFrom(nodename);
		xmlChar *attrvalue = getAttrValueFrom(nodename);
		xmlChar *nodename_pure = getPureNodeNameFrom(nodename);
		tmp = xmlNewChild(lastnode, ns, BAD_CAST nodename_pure, NULL);
		xmlNewProp(tmp, BAD_CAST attrname, BAD_CAST attrvalue);
	}else{
		tmp = xmlNewChild(lastnode, ns, BAD_CAST nodename, NULL);
	}

	return tmp;
}


int node_have_attribude(xmlChar *nodename)
{
	return (xmlStrchr(nodename,'@') == NULL) ? 0 : 1;
}

xmlChar* getAttrNameFrom(xmlChar *nodename)
{
	const xmlChar *start = xmlStrchr(nodename,'@');

	return xmlStrndup(start +1, xmlStrchr(nodename,'=') - start -1);
}

xmlChar* getAttrValueFrom(xmlChar *nodename)
{
	const xmlChar *start = xmlStrchr(nodename,'=');

	return xmlStrndup(start +1, xmlStrchr(nodename,']') - start -1);
}

xmlChar* getPureNodeNameFrom(xmlChar *nodename)
{
	return xmlStrndup(nodename, xmlStrchr(nodename,'[') - nodename);
}


/* dashes into underscores */
#define DASH_TO_UNDERSCORE(p) { \
  char *__p = (char *)p; \
  while (*__p != '\0') { \
    if (*__p == '-') *__p = '_'; \
    ++__p; \
  } \
}

void generateSrcFromTree(FILE *f)
{
  PtGenTreeItem_t *gen = (PtGenTreeItem_t *)PtTreeRootItem(ABW_tree_wgt);

  while (gen != NULL)
  {
    /* t_table_data */
    assert(((PtTreeItem_t *)gen)->data != NULL);
    assert(((t_table_data *)((PtTreeItem_t *)gen)->data)->xpath != NULL);

    /* get category name from xpath */
    int lenprefix = strlen("/configuration/");
    int lenxpath = strlen((const char *)(
          (t_table_data *)((PtTreeItem_t *)gen)->data
          )->xpath);
    assert(lenxpath >= lenprefix);
    char *p = (char *)(
        (t_table_data *)((PtTreeItem_t *)gen)->data
        )->xpath + lenprefix;
    char *label = (char *)malloc(sizeof(char) * (lenxpath - lenprefix +1));
    char *pp = label;

    /* substitute _, uppercase others, truncate at / and copy all */
    for (;*p != '\0'; ++p, ++pp)
    {
      if (*p == '-')
        *pp = '_';
      else if (*p == '/')
      {
        *pp = '\0';
        break;
      }
      else
        *pp = (unsigned char)toupper(*p);
    }

    /* category */
    fprintf(f, ":%s\n", label);
    free(label);

    /* attributes */
    saveAttrToSrc(gen, f, 0);

    /* values */
    saveValToSrc(gen, f, 0);

    if (gen->brother != NULL) fputs("\n\n", f);

    gen = gen->brother;
  }
}

#define FPUTS_N(cnt, str, file) \
  do { \
    int __i; \
    for (__i = 1; __i <= (cnt); ++__i) fputs(str, file); \
  } while (0)

void printTableLines(PtWidget_t *tbl, int col, int colm, int row, int rowm,
    FILE *f, char *tbl_label)
{
  char *s;
  long *flags;

  for (; row <= rowm; ++row)
  {
    for (; col <= colm; ++col)
    {
      if (col != 0) fputs(", ", f);

      t_xml_info *info = NULL;
      tblGetCellResource(tbl, col, 0, Pt_ARG_POINTER, &info, 0);

      switch (info->type)
      {
        case SCADA_ED_XML_ATTR_TYPE_BOOL:
          tblGetCellResource(tbl, col, row, Pt_ARG_FLAGS, &flags, 0);
          fprintf(f, "%d", (*flags & Pt_SET) ? 1 : 0);
          break;

        case SCADA_ED_XML_ATTR_TYPE_STRING:
          s = NULL;
          tblGetCellResource(tbl, col, row, Pt_ARG_TEXT_STRING, &s, 0);
          fprintf(f, "\"%s\"", (s == NULL) ? "" : s);
          break;

        /* SCADA_ED_XML_ATTR_TYPE_NUMBER */
        /* SCADA_ED_XML_ATTR_TYPE_CHAR */
        default:
          s = NULL;
          tblGetCellResource(tbl, col, row, Pt_ARG_TEXT_STRING, &s, 0);

          if (s == NULL || *s == '\0') { \
            fprintf(stderr, "ERROR: Unset value in the table \"%s\" on [%d, %d]. Using 0.\n", \
                tbl_label, col, row); \
              s = "0"; \
          }

          fputs(s, f);
      }
    }
  }
}

void saveAttrToSrc(PtGenTreeItem_t *gen, FILE *f, unsigned short depth)
{
  PtWidget_t *tbl = ((t_table_data *)((PtTreeItem_t *)gen)->data)->table;

  /* no further nesting - we are at the floor */
  if (gen->son == NULL)
  {
    /* only for SIMATIC APL DATA */
    if (depth != 0)
    {
      /* header */
      fputs("# ", f);
      FPUTS_N(depth, "  ", f);
      fputs("subtype_id, subtype_items;\n# ", f);
      FPUTS_N(depth, "  ", f);
      fputs("{\n", f);
    }

    /* SIMATIC APL DATA have no subitems => we can not construct anything */
    if (tbl == NULL) return;

    /* print the line with attributes */
    fputs("# ", f);
    FPUTS_N((depth) ? depth +1: depth, "  ", f);
    int col_max = tblLastCol(tbl);
    t_xml_info *info = NULL;

    int col;
    /* first table line contains attributes */
    for (col = 0; col <= col_max; col++)
    {
      if (col != 0) fputs(", ", f);

      tblGetCellResource(tbl, col, 0, Pt_ARG_POINTER, &info, 0);
      DASH_TO_UNDERSCORE(info->source);
      /* without leading @ */
      fputs((const char *)info->source +1, f);

      /* bool */
      if (info->type == SCADA_ED_XML_ATTR_TYPE_BOOL)
        fputs(" (0/1)", f);
    }

    fputs((depth == 0) ? "\n" : ";\n", f);

    /* only for SIMATIC APL DATA */
    if (depth != 0)
     {
      fputs("# ", f);
      FPUTS_N(depth +1, "  ", f);
      fputs("...\n# ", f);
      FPUTS_N(depth +1, "  ", f);

      /* the last table line contains values we are interested in */
      int row_max = tblLastRow(tbl);
      printTableLines(tbl, 0, col_max, row_max, row_max,
            f, ((PtTreeItem_t *)gen)->string);

      /* footer */
      fputs(";\n# ", f);
      FPUTS_N(depth, "  ", f);
      fputs("}\n", f);
    }
  }
  /* nest deeper */
  else
  {
    /* header */
    fputs("# ", f);
    FPUTS_N(depth, "  ", f);
    fputs("id, subtypes_total;\n# ", f);
    FPUTS_N(depth, "  ", f);
    fputs("{\n", f);
    if (depth == 0) assert(gen->son != NULL);
    saveAttrToSrc((depth) ? gen->son : gen->son->son, f, depth +1);

    fputs("# ", f);
    FPUTS_N(depth +1, "  ", f);
    fputs("...\n", f);
    /* FIXME WTF? Why is it twice in the testing kom_map.src? */
    saveAttrToSrc((depth) ? gen->son : gen->son->son, f, depth +1);

    /* footer */
    fputs("# ", f);
    FPUTS_N(depth, "  ", f);
    fputs("}\n", f);
  }
}

int getGenTreeItemCount(PtGenTreeItem_t *gen)
{
  int i = 0;

  while (gen != NULL)
  {
    gen = gen->brother;
    ++i;
  }

  return i;
}

void saveValToSrc(PtGenTreeItem_t *gen, FILE *f, unsigned short depth)
{
  /* no further nesting => print table content */
  if (gen->son == NULL)
  {
    PtWidget_t *tbl = ((t_table_data *)((PtTreeItem_t *)gen)->data)->table;

    /* SIMATIC APL DATA have no subitems => we can not construct anything */
    if (tbl == NULL) return;

    int col_max = tblLastCol(tbl);
    int row_max = tblLastRow(tbl);

    int row;
    /* at 0 index, there are labels, not values */
    for (row = 1; row <= row_max; row++)
    {
      FPUTS_N(depth, "  ", f);
      printTableLines(tbl, 0, col_max, row, row,
            f, ((PtTreeItem_t *)gen)->string);
      fputs((depth == 0) ? "\n" : ";\n", f);
    }
  }
  /* nest deeper => print bounding boxes */
  else
  {
    gen = gen->son;

    /* loop through immediate childs */
    while (gen != NULL)
    {
      /* header */
      FPUTS_N(depth, "  ", f);
      fprintf(f, "%s, %d\n", ((PtTreeItem_t *)gen)->string,
          (gen->son == NULL) ?
          tblLastRow(((t_table_data *)((PtTreeItem_t *)gen)->data)->table) :
          getGenTreeItemCount(gen->son)
          );
      FPUTS_N(depth, "  ", f);
      fputs("{\n", f);

      /* data (another bounding box or table) */
      saveValToSrc(gen, f, depth +1);

      /* footer */
      FPUTS_N(depth, "  ", f);
      fputs("}\n", f);

      gen = gen->brother;
    }
  }
}
