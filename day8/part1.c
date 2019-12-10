#include <stdio.h>
#include <bigbrainvector.h>

typedef struct LayerStats
{
	int zeros;
	int ones;
	int twos;
} LayerStats;

generate_vector(LayerStats);

int main()
{
	LayerStats_vec layer_stats = build_LayerStats_vec();

	LayerStats current_layer = {0, 0, 0};
	int pixel = 0;
	char c = getchar();

	while (c != '\n')
	{
		switch (c)
		{
			case '0':
				++current_layer.zeros;
				break;
			case '1':
				++current_layer.ones;
				break;
			case '2':
				++current_layer.twos;
				break;
		}

		++pixel;
		c = getchar();

		if (pixel == 150)
		{
			append_LayerStats_vec(&layer_stats, current_layer);
			current_layer = (LayerStats){0, 0, 0};
			pixel = 0;
		}
	}

	LayerStats fewest_zeros = layer_stats.data[0];
	for (int i = 1; i < layer_stats.size; ++i)
	{
		if (layer_stats.data[i].zeros < fewest_zeros.zeros)
			fewest_zeros = layer_stats.data[i];
	}

	printf("Checksum: %d\n", fewest_zeros.ones * fewest_zeros.twos);

	return 0;
}
