#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <utility>

#define _WIN32_WINNT 0x0500

struct Vertex {
	int x;
	int y;
};

struct Edge {
	int src;
	int dst;
	double weight;
};

struct Graph {
	int num_vertices;
	int num_edges;
	
	struct Edge *edge;
	struct Vertex *vertices;
};

struct Subset {
	int parent;
	int rank;
};

struct Vertex* create_vertex(int x, int y) {
	struct Vertex *vertex = (struct Vertex*)(malloc(sizeof(struct Vertex)));
	vertex->x = x;
	vertex->y = y;
	
	return vertex;
}

struct Graph* create_graph(int num_vertices, int num_edges) {
	struct Graph *graph = (struct Graph*)(malloc(sizeof(struct Graph)));
	graph->num_edges = num_edges;
	graph->num_vertices = num_vertices;
	
	graph->vertices = (struct Vertex*)(malloc(num_vertices * sizeof(struct Vertex)));
	graph->edge = (struct Edge*)(malloc(num_edges * sizeof(struct Edge)));
	
	return graph;
}

int find_parent(struct Subset subsets[], int i) {
	if (subsets[i].parent != i) {
		subsets[i].parent = find_parent(subsets, subsets[i].parent);
	}
	
	return subsets[i].parent;
}

void con_graph(struct Subset subsets[], int x, int y) {
	int x_root = find_parent(subsets, x);
	int y_root = find_parent(subsets, y);
	
	if (subsets[x_root].rank < subsets[y_root].rank) {
		subsets[x_root].parent = y_root;
	}
	else if (subsets[x_root].rank > subsets[y_root].rank) {
		subsets[y_root].parent = x_root;
	}
	else {
		subsets[y_root].parent = x_root;
		subsets[x_root].rank++;
	}
}

int compare_weights(const void *a, const void *b) {
	struct Edge *a1 = (struct Edge*)a;
	struct Edge *b1 = (struct Edge*)b;
	
	return (a1->weight > b1->weight) ? 1 : ((a1->weight < b1->weight) ? -1 : 0);
}

struct Subset* kruskal(struct Graph *graph, int n_cluster, int *cluster_result) {
	int num_vertices = graph->num_vertices;
	struct Edge result[num_vertices];
	
	int e = 0;
	int i = 0;
	
	qsort(graph->edge, graph->num_edges, sizeof(graph->edge[0]), compare_weights);
	
	struct Subset *subsets = (struct Subset*)(malloc(num_vertices * sizeof(struct Subset)));	
	
	for (int v = 0; v < num_vertices; v++) {
		subsets[v].parent = v;
		subsets[v].rank = 0;
	}
	
	while (e < num_vertices - n_cluster && i < graph->num_edges) {
		struct Edge next_edge = graph->edge[i++];
		
		int x = find_parent(subsets, next_edge.src);
		int y = find_parent(subsets, next_edge.dst);
		
		if (x != y) {
			result[e++] = next_edge;
			con_graph(subsets, x, y);
		}
	}
	
	for (int v = 0; v < num_vertices; v++) {
		subsets[v].parent = find_parent(subsets, subsets[v].parent);
	}

	for (int a = 0; a < num_vertices; a++) {
		cluster_result[a] = subsets[a].parent;
	}

	return subsets;
}

struct Subset* kruskal(struct Graph *graph, double cluster_threshold, int *cluster_result) {
	int num_vertices = graph->num_vertices;
	struct Edge result[num_vertices];
	
	int e = 0;
	int i = 0;
	
	qsort(graph->edge, graph->num_edges, sizeof(graph->edge[0]), compare_weights);
	
	struct Subset *subsets = (struct Subset*)(malloc(num_vertices * sizeof(struct Subset)));	
	
	for (int v = 0; v < num_vertices; v++) {
		subsets[v].parent = v;
		subsets[v].rank = 0;
	}
	
	while (e < num_vertices - 1 && i < graph->num_edges) {
		struct Edge next_edge = graph->edge[i++];
		if(next_edge.weight > cluster_threshold) {
			continue;
		}
		
		int x = find_parent(subsets, next_edge.src);
		int y = find_parent(subsets, next_edge.dst);
		
		if (x != y) {
			result[e++] = next_edge;
			con_graph(subsets, x, y);
		}
	}
	
	for (int v = 0; v < num_vertices; v++) {
		subsets[v].parent = find_parent(subsets, subsets[v].parent);
	}

	for (int a = 0; a < num_vertices; a++) {
		cluster_result[a] = subsets[a].parent;
	}

	return subsets;
}

float distance(int py, int px, int jy, int jx) {
	return sqrtf((float)((px-jx)*(px-jx)+(py-jy)*(py-jy)));
}

int combination(int N, int K) {
	if (K > N) return 0;
    int r = 1;
    int d;
    for (d = 1; d <= K; d++) {
       r *= N--;
       r /= d;
    }
   	return r;
}

void initialize_graph() {
	int graph_width = 100;
	int graph_height = 30;
	
	int counter = 0;
	
	for (int a = 0; a < graph_height; a++) {
		for (int b = 0; b < graph_width; b++) {
			if (a == graph_height - 1 && b == 1) {
				printf("0");
			}
			else if (a == graph_height - 1 && (b - 3)%10 == 0 && (b-3) > 0) {
				if (b - 3 > 9) printf("\b");
				printf("%d", b - 3);
			}
			else if (b == 1) {
				int h_index = a + 27 - counter;
				if (h_index % 5 == 0 && h_index > 0) {
					if (h_index > 9) printf("\b");
					printf("%d", h_index);
				}
				else {
					printf(" ");
				}
				counter+=2;
			}
			else if (a == graph_height-3 && b == 3) {
				printf("+");
			}
			else if (a == graph_height-3 && b > 2) {
				if ((b - 3)%10 == 0) printf("|");
				else printf("-");
			}
			else if (b == 3 && a < graph_height - 2) {
				int h_index = a + 27 - (counter - 2);
				if (h_index % 5 == 0 && h_index > 0) {
					printf("-");
				}
				else {
					printf("|");
				}
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

void move_cursor(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void draw_dots(int x, int y, int color_code){
	int translated_x = x + 3;
	int translated_y = y + (27 - (y * 2));
	move_cursor(translated_x, translated_y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_code);
	printf("%c", 260);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void clear_graph() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	for (int a = 0; a < 27; a++) {
		move_cursor(4, a);
		for (int b = 0; b < 96; b++) {
			printf(" ");
		}
	}
}

void draw_centroid(int x, int y, int color_code){
	int translated_x = x + 3;
	int translated_y = y + (27 - (y * 2));
	move_cursor(translated_x, translated_y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_code);
	printf("%c", 233);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

//constraint 0 = n-cluster
//constraint 1 = treshold
void print_menu(int active_menu, int data_count, int cluster_result, int constraint, double constraint_value, int show_centroids) {
	int h_position = 105;
	int v_position = 0;
	
	move_cursor(h_position, v_position++);
	//1st row
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	printf("        [F]OLICE [S]TATE        ");
	
	move_cursor(h_position, v_position++);
	printf("           DEPARTMENT           ");
	v_position++;
	move_cursor(h_position, v_position++);
	
	//data count
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("%-16s", "DATA COUNT:");
	printf("%16d", data_count);
	move_cursor(h_position, v_position++);
	
	//cluster result
	printf("%-16s", "CLUSTER RESULT:");
	printf("%16d", cluster_result);
	v_position++;
	move_cursor(h_position, v_position++);
	
	if (active_menu == 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	//constraint
	printf("%-16s", "CONSTRAINT:");
	printf("%16s", constraint == 0 ? "N-CLUSTER" : "THRESHOLD");
	move_cursor(h_position, v_position++);
	if (active_menu == 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
	if (active_menu == 2) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	//cluster count
	printf("%-16s", constraint == 0 ? "CLUSTER COUNT:" : "THRESHOLD:");
	if (constraint == 1 ) printf("%16.2lf", constraint_value);
	else printf("%16d", int(constraint_value));
	move_cursor(h_position, v_position++);
	if (active_menu == 2) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
	if (active_menu == 3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	//centroids
	printf("%-16s", "SHOW CENTROIDS:");
	printf("%16s", show_centroids == 0 ? "[ ]" : "[X]");
	v_position++;
	move_cursor(h_position, v_position++);
	if (active_menu == 3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
	if (active_menu == 4) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	//save result
	printf("%-32s", "SAVE RESULT");
	move_cursor(h_position, v_position++);
	if (active_menu == 4) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
	if (active_menu == 5) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	//reset view
	printf("%-32s", "RESET VIEW");
	move_cursor(h_position, v_position++);
	if (active_menu == 5) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
	move_cursor(0, 31);
}

void print_legends() {
	int y_position = 31;
	move_cursor(0, y_position);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	char menu[256];
	sprintf(menu, "   [ESC]EXIT   [%c%c]SELECT MENU   [%c%c]CHANGE VALUE   [ENTER]OK", 286, 287, 273, 272);
	printf("%-100s" , menu); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

int cluster(struct Graph *graph, int *cluster_result, std::pair<int, int> *centroids) {
	
	Vertex *vertices = graph->vertices;
	int num_vertices = graph->num_vertices;
	
	int cluster_mapped[num_vertices] = {0};
	int total_x[num_vertices] = {0};
	int total_y[num_vertices] = {0};
	
	int cluster_size[15] = {0};
	
	int cluster_count = 1;
	
	for (int a = 0; a < num_vertices; a++) {
		if (cluster_mapped[cluster_result[a]] == 0) {
			cluster_mapped[cluster_result[a]] = cluster_count;
			cluster_count++;
		}
		
		total_x[cluster_mapped[cluster_result[a]]] += vertices[a].x;
		total_y[cluster_mapped[cluster_result[a]]] += vertices[a].y;
		
		cluster_size[cluster_mapped[cluster_result[a]]]++;
		
		cluster_result[a] = cluster_mapped[cluster_result[a]];
	}
	
	for (int a = 1; a < cluster_count; a++) {
		centroids[a].first = total_x[a] / cluster_size[a];
		centroids[a].second = total_y[a] / cluster_size[a];
	}
	
	int change = 1;
	while (change != 0) {
		change = 0;
		
		for (int a = 1; a < cluster_count; a++) {
			centroids[a].first = total_x[a] / cluster_size[a];
			centroids[a].second = total_y[a] / cluster_size[a];
			
		}
		
		for (int a = 0; a < num_vertices; a++) {
			int cluster = cluster_result[a];
			double dist_own_centroid = distance(centroids[cluster].first, centroids[cluster].second,
												vertices[a].x, vertices[a].y);
			
			for (int b = 1; b < cluster_count; b++) {
				if (b == cluster) continue;
				double dist_to_centroid = distance(centroids[b].first, centroids[b].second,
												vertices[a].x, vertices[a].y);
												
				if (dist_own_centroid > dist_to_centroid) {
					cluster_result[a] = b;
					dist_own_centroid = dist_to_centroid;
					total_x[cluster] -= vertices[a].x;
					total_y[cluster] -= vertices[a].y;
					
					total_x[b] += vertices[a].x;
					total_y[b] += vertices[a].y;
					
					cluster_size[cluster]--;
					cluster_size[b]++;
					
					cluster = b;
					
					change++;
				}
			}
			
		}
	}
	
	return cluster_count;
}

void draw_graph(int num_vertices, int cluster_count, struct Graph *graph, int *cluster_result, std::pair<int, int> *centroids, int show_centroid) {
	clear_graph();
	for (int a = 0; a < num_vertices; a++) {
		draw_dots(graph->vertices[a].x, graph->vertices[a].y, cluster_result[a]);
	}
	
	if (show_centroid) {
		for (int a = 1; a < cluster_count; a++) {	
			draw_centroid(centroids[a].first, centroids[a].second, a);
		}
	}
}

int main() {
	HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); 
    MoveWindow(console, 0, 0, 1280, 700, TRUE);
    
    int end_coord_x = 0;
    int end_coord_y = 32;
    
	int coords[300][4];
	
	FILE *f;
	
	f = fopen("coords.csv", "r");
	
	if (f == NULL) {
		exit(1);
	}
	
	char line[256];
	int num_vertices = 0;
	
	while (fgets(line, sizeof(line), f)) {
		char *token;
		
		token = strtok(line, ",");
		coords[num_vertices][0] = atoi(token);
		token = strtok(NULL, ",");
		coords[num_vertices][2] = atoi(token);
		token = strtok(NULL, ",");
		coords[num_vertices][1] = atoi(token);
		
		num_vertices++;
	}
	
	fclose(f);
	
	int max_edges = combination(num_vertices, 2);
	
	struct Graph *graph = create_graph(num_vertices, max_edges);
	
	for (int a = 0; a < num_vertices; a++){
		graph->vertices[a].x = coords[a][1];
		graph->vertices[a].y = coords[a][2];
	}
	
	
	int count = 0;
	double total_dist = 0;
	
	for (int a = 0; a < num_vertices; a++) {
		Vertex vertex = graph->vertices[a];
		for (int b = a + 1; b < num_vertices; b++) {
			Vertex vertex_to = graph->vertices[b];
					
			graph->edge[count].src = a;
			graph->edge[count].dst = b;
			double dist = distance(vertex.x, vertex.y, vertex_to.x, vertex_to.y);
			graph->edge[count].weight = dist;
			
			total_dist += dist;
			
			count++;
		}
	}
	
	int cluster_result[num_vertices] = {0};
	std::pair<int, int> centroids[15];
	
	int cluster_count = 1;

	initialize_graph();
	clear_graph();
	
	int selected_menu = 1;
	int n_cluster = 4;
	double threshold = 5.0;
	int constraint = 0;
	int show_centroid = 0;
	
	print_legends();
	print_menu(selected_menu, num_vertices, cluster_count-1, constraint, n_cluster, show_centroid);
	
 	move_cursor(end_coord_x, end_coord_y);
 	
 	int in_menu = 1;
	while (in_menu) {
		int k = getch();
		
		switch (k) {
			case 72:
				if (selected_menu == 1) continue;
				selected_menu--;
				break;
			case 80:
				if (selected_menu == 5) continue;
				selected_menu++;
				break;
			case 75:
				if (selected_menu == 1) {
					constraint = constraint == 1 ? 0 : 1;
				}
				else if (selected_menu == 2) {
					if (constraint == 0) {
						if (n_cluster == 2) continue;
						n_cluster--;
					}
					else {
						if (threshold == 4.5) continue;
						threshold -= 0.5;
					}
				}
				break;
			case 77:
				if (selected_menu == 1) {
					constraint = constraint == 1 ? 0 : 1;
				}
				else if (selected_menu == 2) {
					if (constraint == 0) {
						if (n_cluster == 10) continue;
						n_cluster++;
					}
					else {
						if (threshold == 10.5) continue;
						threshold += 0.5;
					}
				}
				break;
			case 13:
				if (selected_menu == 3) {
					show_centroid = show_centroid == 1 ? 0 : 1;
				}
				else if (selected_menu == 4) {
					FILE *f;
					
					f = fopen("result.csv", "w");
					if (f == NULL) {
						continue;
					}
					
					for (int a = 0; a < num_vertices; a++) {
						fprintf(f, "%d,%d,%d,%d\n", coords[a][0], coords[a][1], coords[a][2], cluster_result[a]);
					}
					
					fclose(f);
				}
				else if (selected_menu == 5) {
					clear_graph();
					if (constraint == 1) kruskal(graph, threshold, cluster_result);
					else kruskal(graph, n_cluster, cluster_result);
					
					cluster_count = cluster(graph, cluster_result, centroids);
					
					draw_graph(num_vertices, cluster_count, graph, cluster_result, centroids, show_centroid);
				}
				break;
			case 27:
				in_menu = 0;
				break;
		}
		
		print_menu(selected_menu, num_vertices, cluster_count-1, constraint, constraint == 0 ? n_cluster : threshold, show_centroid);
	}
    return 0;
}

