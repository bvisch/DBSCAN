#ifndef __CLUSTERING__
#define __CLUSTERING__

#include <vector>
#include <map>
#include <list>

namespace clustering {

	template <typename T>
	struct node {
		T val;
		bool visited;
		bool clustered;
		node() : visited(false), clustered(false) {}
	};

	template <typename T>
	using cluster = std::vector<T>;

	template <typename T>
	using adj_list = std::list<node<T>*>;

	template <typename T>
	using graph = std::map<node<T>*, adj_list<T>>;

	template <typename T, typename DIST_T>
	std::vector<cluster<T>> DBSCAN(T* const& dataset, size_t const dataset_size, DIST_T const eps, size_t const min_pts, DIST_T(*distance_function)(T const& lhs, T const& rhs)) {
		
		std::vector<node<T>> node_list(dataset_size);
		for (auto i = 0; i < dataset_size; ++i) {
			node_list[i].val = dataset[i];
		}

		graph<T> g;
		for (auto i = 0; i < node_list.size(); ++i) {
			for (auto j = 0; j < i; ++j) {
				if (distance_function(node_list[i].val, node_list[j].val) < eps) {
					g[&node_list[i]].push_back(&node_list[j]);
					g[&node_list[j]].push_back(&node_list[i]);
				}
			}
		}

		std::vector<cluster<T>> clusters;
		cluster<T> c;
		for (node<T>& n : node_list) {
			if (n.visited) continue;
			n.visited = true;
			adj_list<T> neighbour_pts = g[&n];
			if (neighbour_pts.size() >= min_pts) {
				expandCluster(n, neighbour_pts, c, g, min_pts);
				clusters.push_back(c);
				c = cluster<T>();
			}
		}

		return clusters;
	}

	template <typename T>
	void expandCluster(node<T>& point, adj_list<T>& neighbourhood, cluster<T>& c, graph<T>& g, unsigned const& min_pts) {
		c.push_back(point.val);
		point.clustered = true;
		for (node<T>*& n : neighbourhood) {
			if (!n->visited) {
				n->visited = true;
				adj_list<T> next_neighbourhood = g[n];
				if (next_neighbourhood.size() >= min_pts) neighbourhood.splice(neighbourhood.end(), next_neighbourhood);
			}
			if (!n->clustered) {
				c.push_back(n->val);
				n->clustered = true;
			}
		}
	}
}

#endif