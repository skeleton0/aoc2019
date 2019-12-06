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
	strcpy(root.label, "COM");
	
	Node_vec nodes_to_explore = build_Node_vec();
	append_Node_vec(&nodes_to_explore, root); 

	int orbits = 0;
	int node_index = 0;
	while (node_index < nodes_to_explore.size)
	{
		Node current_node = nodes_to_explore.data[node_index];

		//find satellites
		for (int i = 0; i < orbit_data.size; ++i)
		{
			if (strcmp(orbit_data.data[i].host, current_node.label) == 0)
			{
				Node new_node = {malloc(4), current_node.depth + 1};
				strcpy(new_node.label, orbit_data.data[i].satellite);
				append_Node_vec(&nodes_to_explore, new_node);
			}	
		}

		orbits += current_node.depth;
		++node_index;
	}

	printf("Total amount of orbits = %d\n", orbits);

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
