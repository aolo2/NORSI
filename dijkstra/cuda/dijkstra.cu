#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include "check.h"

typedef std::pair<unsigned int, unsigned int> graph_dim;

const float inf = std::numeric_limits<float>::infinity();
const unsigned int start = 1; //415;
const unsigned int end = 5	; //330949;

__global__
void bellman_ford(unsigned int iter, unsigned int *starts, unsigned int *ends,
		float *weights, float *dist, unsigned int n) {

	unsigned int from, to;
	float w;

//	printf("n: %d\t index: %d\t stride: %d\n", n, index, stride);

	for (unsigned int i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += gridDim.x * blockDim.x) {
		from = starts[i];
		to = ends[i];
		w = weights[i];

//		printf("from: %d\t to: %d \t w: %f\n", from, to, w);

		if (dist[from] + w < dist[to]) {
			dist[to] = dist[from] + w;
//			*modif = iter;
		}
	}
}

graph_dim read_file(const std::string &path, unsigned int **starts,
		            unsigned int **ends, float **weights) {

	std::ifstream file(path.c_str());
	std::string line;

	graph_dim size;
	size.first = std::count(std::istreambuf_iterator<char>(file),
				 	 	    std::istreambuf_iterator<char>(), '\n');

	*starts = (unsigned int *) malloc(size.first * sizeof(unsigned int));
	*ends = (unsigned int *) malloc(size.first * sizeof(unsigned int));
	*weights = (float *) malloc(size.first * sizeof(float));

	float weight;
	unsigned int n = 0;
	unsigned int from, to, vertex_num = 0;

	file.close();
	file.open(path.c_str());

	while (file >> from >> to >> weight) {
		vertex_num = std::max(vertex_num, from + 1); // undirected G's are expected
		(*starts)[n] = from;
		(*ends)[n] = to;
		(*weights)[n] = weight;
		++n;
	}

	size.second = vertex_num + 1;

	return size;
}

int main(void) {
	unsigned int *starts = NULL, *ends = NULL, *d_s, *d_e, edge_num, vertex_num;
	float *weights = NULL, *dist, *d_w, *d_dist;

	graph_dim dimensions = read_file("SMALL2_int.sdot", &starts, &ends, &weights);
	edge_num = dimensions.first, vertex_num = dimensions.second;

	dist = (float *) malloc(vertex_num * sizeof(float));

	for (unsigned int i = 0; i < vertex_num; i++) {	dist[i] = inf;}
	dist[start] = 0.0f;

	check(cudaMalloc((void **) &d_s, edge_num * sizeof(unsigned int)));
	check(cudaMalloc((void **) &d_e, edge_num * sizeof(unsigned int)));
	check(cudaMalloc((void **) &d_w, edge_num * sizeof(float)));
	check(cudaMalloc((void **) &d_dist, vertex_num * sizeof(float)));

	check(cudaMemcpy(d_s, starts, edge_num * sizeof(unsigned int), cudaMemcpyHostToDevice));
	check(cudaMemcpy(d_e, ends, edge_num * sizeof(unsigned int), cudaMemcpyHostToDevice));
	check(cudaMemcpy(d_w, weights, edge_num * sizeof(float), cudaMemcpyHostToDevice));
	check(cudaMemcpy(d_dist, dist, vertex_num * sizeof(float), cudaMemcpyHostToDevice));

	int block_size = 512;
	int num_blocks = (edge_num + block_size - 1) / block_size;

	for (unsigned int i = 1; i < edge_num; i++) {
		bellman_ford<<<num_blocks, block_size>>>(i, d_s, d_e, d_w, d_dist, edge_num);
//		check(cudaMemcpy(dist, d_dist, vertex_num * sizeof(float), cudaMemcpyDeviceToHost));
//		for (unsigned int i = 0; i < vertex_num; i++) { std::cout << dist[i] << " "; }
//		std::cout << std::endl;
	}

	check(cudaMemcpy(dist, d_dist, vertex_num * sizeof(float), cudaMemcpyDeviceToHost));

	std::cout << dist[end] << std::endl;

	free(starts);
	free(ends);
	free(weights);
	free(dist);

	check(cudaFree(d_s));
	check(cudaFree(d_e));
	check(cudaFree(d_w));
	check(cudaFree(d_dist));

	return 0;
}
