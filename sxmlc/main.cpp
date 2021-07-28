#include "sxmlc.h"

void PrintBlank(int blankCount)
{
	for (int i = 0; i < blankCount; i++)
		putchar(' ');
}
void PrintNode(XMLNode* source, int blankCount)
{
	if (source->n_children > 0)
	{
		PrintBlank(blankCount);

		if (source->n_attributes)
		{
			for(int i = 0; i < source->n_attributes; i++)
				printf("%s %s:%s\n", source->tag, source->attributes[i].name, source->attributes[i].value);
		}
		else
			printf("%s\n", source->tag);

		int childBlankCount = blankCount + 2;
		for (int i = 0; i < source->n_children; i++)
		{
			XMLNode* child = source->children[i];

			if (child)
			{
				if (child->n_children)
				{
					PrintNode(child, childBlankCount);
				}
				else
				{
					PrintBlank(childBlankCount);
					printf("%s", child->tag);
					printf(": %s\n", child->text);
				}
			}
		}
		PrintBlank(blankCount);
		printf("%s end\n", source->tag);
	}
	
}


int main()
{
	XMLDoc doc;
	XMLDoc_init(&doc);
	XMLDoc_parse_file_DOM("./fox.xml", &doc);
	XMLNode* root;
	root = doc.nodes[doc.i_root];

	if (root)
	{
		
		PrintNode(root, 0);
	}

	XMLDoc_free(&doc);

	return 0;
}
