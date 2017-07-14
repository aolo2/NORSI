#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include "check.h"

__global__
void bellman_ford(unsigned int iter, unsigned int *starts, unsigned int *ends,
		float *weights, float *dist, unsigned int n) {

	unsigned int index = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int stride = gridDim.x * blockDim.x;
	unsigned int from, to;
	float w;

//	printf("n: %d\t index: %d\t stride: %d\n", n, index, stride);

	for (unsigned int i = index; i < n; i += stride) {
		w = weights[i];
		from = starts[i];
		to = ends[i];

//		printf("from: %d\t to: %d \t w: %f\n", from, to, w);

		if (dist[from] + w < dist[to]) {
			dist[to] = dist[from] + w;
//			*modif = iter;
		}
	}
}

int main(void) {
	unsigned int modif = 0;
//	*modif = 0;

	std::ifstream r("LEN_int.sdot");
	std::string line;

	const unsigned int edge_num = std::count(std::istreambuf_iterator<char>(r),
			std::istreambuf_iterator<char>(), '\n');

	r.close();
	r.open("LEN_int.sdot");

	const unsigned int start = 0; //415;
	const unsigned int end = 8; //330949;

	unsigned int from, to, vertex_num;
	float weight;

	unsigned int *starts, *ends;
	float *weights, *dist;

	starts = (unsigned int *) malloc(edge_num * sizeof(unsigned int));
	ends = (unsigned int *) malloc(edge_num * sizeof(unsigned int));
	weights = (float *) malloc(edge_num * sizeof(float));

	unsigned int n = 0;
	while (r >> from >> to >> weight) {
		vertex_num = std::max(vertex_num, from + 1); // WRONG FOR DIGRAPGS!!
		starts[n] = from;
		ends[n] = to;
		weights[n] = weight;
		++n;
	}

	dist = (float *) malloc(vertex_num * sizeof(float)); // vertices nums start from 1
	float inf = std::numeric_limits<float>::infinity();

	dist[start] = 0.0f;
	for (unsigned int i = 1; i < vertex_num; i++) {
		dist[i] = inf;
	}

	/*for (unsigned int i = 0; i < edge_num; i++) {
		std::cout << std::left << std::setw(2) << starts[i] << " "
				<< std::setw(2) << ends[i] << " " << weights[i] << std::endl;
	}*/

	unsigned int *d_s, *d_e;
	float *d_w, *d_dist;

	check(cudaMalloc((void **) &d_s, edge_num * sizeof(unsigned int)));
	check(cudaMalloc((void **) &d_e, edge_num * sizeof(unsigned int)));
	check(cudaMalloc((void **) &d_w, edge_num * sizeof(float)));
	check(cudaMalloc((void **) &d_dist, vertex_num * sizeof(float)));

	check(cudaMemcpy(d_s, starts, edge_num * sizeof(unsigned int), cudaMemcpyHostToDevice));
	check(cudaMemcpy(d_e, ends, edge_num * sizeof(unsigned int), cudaMemcpyHostToDevice));
	check(cudaMemcpy(d_w, weights, edge_num * sizeof(float), cudaMemcpyHostToDevice));
	check(cudaMemcpy(d_dist, dist, vertex_num * sizeof(float), cudaMemcpyHostToDevice));

	int block_size = 32;
	int num_blocks = (edge_num + block_size - 1) / block_size;

	for (unsigned int i = 1; i < n; i++) {
		bellman_ford<<<num_blocks, block_size>>>(i, d_s, d_e, d_w, d_dist, edge_num);

//		if (modif == i - 1) {
//			break;
//		}
	}

	check(cudaMemcpy(dist, d_dist, vertex_num * sizeof(float), cudaMemcpyDeviceToHost));

	for (unsigned int i = 0; i < vertex_num; i++) {
		std::cout << dist[i] << " ";
	}
	std::cout << std::endl;

	check(cudaFree(d_s));
	check(cudaFree(d_e));
	check(cudaFree(d_w));
	check(cudaFree(d_dist));

	return 0;
}
