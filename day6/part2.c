#include <stdio.h>
#include <bigbrainvector.h>
#include <string.h>

typedef struct Orbit
{
	char* host;
	char* satellite;
} Orbit;

typedef struct Node
{
	char* label;
	int depth;
} Node;

generate_vector(Orbit);
generate_vector(Node);

int main()
{
	Orbit_vec orbit_data = build_Orbit_vec();

	char host[4];
	char satellite[4];
	while (scanf("%3s)%3s\n", host, satellite) == 2)
	{
		Orbit orbit = {malloc(4), malloc(4)};

		strcpy(orbit.host, host);
		strcpy(orbit.satellite, satellite);

		append_Orbit_vec(&orbit_data, orbit);
	}

	Node root = {malloc(4), 0};
	strcpy(root.label, "X1T");
	
	Node_vec nodes_to_explore = build_Node_vec();
	append_Node_vec(&nodes_to_explore, root); 

	int orbit_transfers = 0;
	int node_index = 0;
	while (node_index < nodes_to_explore.size && !orbit_transfers)
	{
		Node current_node = nodes_to_explore.data[node_index];

		//find satellites
		for (int i = 0; i < orbit_data.size; ++i)
		{
			Node new_node = {NULL, 0};
			if (strcmp(orbit_data.data[i].host, current_node.label) == 0)
			{		
				new_node = (Node){malloc(4), current_node.depth + 1};
				strcpy(new_node.label, orbit_data.data[i].satellite);

			}	
			else if (strcmp(orbit_data.data[i].satellite, current_node.label) == 0)
			{
				new_node = (Node){malloc(4), current_node.depth + 1};
				strcpy(new_node.label, orbit_data.data[i].host);
			}

			if (new_node.depth)
			{
				int already_explored = 0;
				for (int j = 0; j < node_index; ++j)
				{
					if (strcmp(nodes_to_explore.data[j].label, new_node.label) == 0)
					{
						already_explored = 1;
						break;
					}
				}
				
				if (already_explored)
					free(new_node.label);
				else
				{
					append_Node_vec(&nodes_to_explore, new_node);
					if (strcmp(new_node.label, "186") == 0)
					{
						orbit_transfers = new_node.depth;
						break;
					}
				}
			}
		}

		++node_index;
	}

	printf("Depth from current host to santa's host = %d\n", orbit_transfers);

	//free orbit data
	for (int i = 0; i < orbit_data.size; ++i)
	{
		free(orbit_data.data[i].host);
		free(orbit_data.data[i].satellite);
	}

	free(orbit_data.data);

	//free nodes
	for (int i = 0; i < nodes_to_explore.size; ++i)
	{
		free(nodes_to_explore.data[i].label);
	}

	free(nodes_to_explore.data);

	return 0;
}
