
#include "Sennot_Graph.h"

#include <iostream>

//#define LOGGING


 Sennot_Graph::~Sennot_Graph() {
    for (int i = 0; i < NODE_COUNT; i++) {
        delete(this->graph[i]);
    }

     // Change to recursively delete children
     for (Node * n : this->progression_tree) {
         delete(n);
     }

}

nodeLabel Sennot_Graph::get_last_node(Node * root, int deeper, nodeLabel & parent) {
    if (deeper == 0) {


        return root->node_id;

    } else {
        for (int i = 0 ; i < MAX_NEIGHBORS;i++) {
            if (root->neighbors[i].second != INVALID_NEIGHBOR) {
                nodeLabel temp = get_last_node(root->neighbors[i].first,deeper -1,parent);

                if (temp != INVALID_NEIGHBOR && deeper == 1) {
                    std::cout << "The parent was " << root->node_id << std::endl;
                    parent = root->node_id;
                }
                return temp;
            }
        }

        return INVALID_NEIGHBOR;
    }
}


nodeLabel Sennot_Graph::get_last_node(int deeper,nodeLabel & parent) {
    nodeLabel retC;
    for (Node * root : this->progression_tree) {
        retC =get_last_node(root,deeper,parent);
        if (retC != -1) {
            return retC;
        }
    }

    return -1;
}

graphInt Sennot_Graph::path_count() {
    return num_paths;
}

graphInt Sennot_Graph::get_depth() {
    return depth;
}

Node * Sennot_Graph::get_node(nodeLabel node) {
    Node * return_node = nullptr;

    int index = node - CHAR_TO_POSITION;
    if (index < this->graph.size()) {
        return_node = this->graph[index];
    } else {
        // Problem!!

    }

    return return_node;
}

Sennot_Graph::Sennot_Graph() {
    initialize_paths();
    initialize_graph();

    edge_progress = 0;
    depth = 0;
    num_paths = NODE_COUNT;
}

bool check_valid(Node * curr, std::vector<cardinalDirection> & dirs_open) {

    for (int i = 0; i < MAX_NEIGHBORS; i++) {

        if( ((dirs_open[i] != INVALID_NEIGHBOR) && curr->neighbors[i].second == INVALID_NEIGHBOR ))  {
            return false;
        }
    }

    return true;
}

bool Sennot_Graph::neighbor_logic(int n1, int n2) {
    bool  retb = false;

    if (n1 == 1 && n2 != 1) {
        retb = false;
    } else if( n1 == 3 && n2 == 2) {
        retb = true;
    } else if (n1 == 4 && n2 == 3) {
        retb = true;
    } else if (n1 == n2 ) {
        retb = true;
    }

    return retb;

}

int Sennot_Graph::add_node(Node * root ,int tree_depth, int num_neighbors, int add_cost) {

    int to_ret = 0;
    // If we are at leaf level
    if (tree_depth == 0) {

        Node * ref = get_node(root->node_id);

        for (int i = 0; i < MAX_NEIGHBORS; i ++) {

            if (ref->neighbors[i].second != INVALID_NEIGHBOR && neighbor_logic(ref->neighbors[i].first->num_neighbors() , num_neighbors) && ref->neighbors[i].first->visitor != ref->node_id /* && ref->neighbors[i].second-1 <= add_cost || ref->neighbors[i].second+1 >= add_cost*/) {
               // cardinalDirection rev_dir = (ref->neighbors[i].second + 2) % 4;
             //   if ((ref->neighbors[i].first->neighbors[rev_dir].second != INVALID_NEIGHBOR && (ref->neighbors[i].first->neighbors[rev_dir].first->node_id != ref->node_id))) {
                    // Creating a new node with same ID as the potential neighbor
                    Node * to_add = new Node( ref->neighbors[i].first->node_id );
                    ref->neighbors[i].first->visitor = ref->node_id;

                    std::cout << "The node ID was " << ref->neighbors[i].first->node_id << "\n";
                    if (root->add_neighbor(to_add,add_cost)) {
                        to_ret+= 1;
                    }
              //  } else {
             //       return 0;
              //  }
            }
        }
        return to_ret;

    } else {
        for (int i = 0; i < MAX_NEIGHBORS;i++) {
            if (root->neighbors[i].second != INVALID_NEIGHBOR  ) {

                to_ret += add_node(root->neighbors[i].first,tree_depth-1,num_neighbors,add_cost);
            }
        }


    }

    return to_ret;
}


// S
bool Sennot_Graph::intersection_update( std::vector<handDirection> & dirs_open) {

    int opencount= 0;
    for (handDirection d : dirs_open) {
        if (d != INVALID_NEIGHBOR) {
            opencount++;
        }
    }

    // Clear visitor list
    for (Node * n : graph) {
        n->visitor = -1;
    }

    int added = 0;

    for (Node * n : progression_tree) {
        added += add_node(n, this->depth, opencount,this->edge_progress);
    }

    std::cout << "In updating: " << added << " were added to graph.\n";

    // If we only are able to add 1 we have located ourselves.

    this->num_paths = added;
    // If we are unable to add at all we are fucked.
    this->edge_progress = 0;

    if (added) {
        depth++;
        return true;
    } else {
        return false;
    }
}


/**
* Prunes possible paths through graph to correspond with how many lights the current path has passed.
*
*
* \param path_cost cost of the edge we are currently travelling upon
*/
graphInt Sennot_Graph::edge_step() {

    edge_progress++;

    return 0;
}

void Sennot_Graph::initialize_paths() {
    for (int i = 0; i < NODE_COUNT; i++) {
        // set initial steps
        (graph[i]) = new Node(nodes[i]);
        this->progression_tree[i] = new Node(graph[i]->node_id);
    }
}

void Sennot_Graph::initialize_graph() {
    depth = 0;
    num_paths = NODE_COUNT;

    // For each each in graph, add connection from node0 to node1 in one direction, then add edge from node1 to node0 in the opposite
    // direction
    for (int i = 0; i < EDGE_COUNT; i++) {
        (this->graph.at(edges[i][0] - 65))->add_neighbor(graph.at(edges[i][1] - CHAR_TO_POSITION), edges[i][2], edges[i][3]);
        (this->graph.at(edges[i][1] - 65))->add_neighbor(graph.at(edges[i][0] - CHAR_TO_POSITION), edges[i][2], (edges[i][3] + 2) % 4);
    }
}

std::vector<nodeLabel> Sennot_Graph::find_path(nodeLabel start, nodeLabel finish) {
    Node * source = graph[start - CHAR_TO_POSITION];
    Node * dest = graph[finish - CHAR_TO_POSITION];

    return find_path(source, dest);
}

std::vector<nodeLabel> Sennot_Graph::find_path(Node *start, Node *finish) {
    std::vector<nodeLabel> to_return;

#ifdef LOGGING
    std::cout << "We are searching for a path from " << start->node_id << " to " << finish->node_id << std::endl;
#endif

    // Used to mark nodes as visited
    // 0 -> clear
    // 1 -> visited
    int dist[NODE_COUNT] = {0};
    char prev[NODE_COUNT] = {0};

    std::vector<Node *> bfs_queue;

    dist[start->node_id - CHAR_TO_POSITION] = 0;
    prev[start->node_id - CHAR_TO_POSITION] = INVALID_PREV;

    for (Node * n : this->graph) {
        if (n->node_id != start->node_id) {
            dist[n->node_id - CHAR_TO_POSITION] = 9999;
            prev[n->node_id - CHAR_TO_POSITION] = INVALID_PREV;
        }

        bfs_queue.push_back(n);
    }

        while (!bfs_queue.empty()) {
            int alt_cost;

            int min_c = dist[bfs_queue.front()->node_id - CHAR_TO_POSITION];
            int iter = 0;
            int saved_iter = 0;
            for (iter = 0 ; iter < bfs_queue.size(); iter++) {
                if (dist[bfs_queue[iter]->node_id - CHAR_TO_POSITION] < min_c) {

                    #ifdef LOGGING
                    std::cout << "Updating " << bfs_queue[iter]->node_id << std::endl;
                    #endif
                    min_c = dist[bfs_queue[iter]->node_id - CHAR_TO_POSITION];
                    saved_iter = iter;
                }
            }

            Node *current = bfs_queue[saved_iter];
            bfs_queue.erase(bfs_queue.begin() + saved_iter);

            if (current->node_id == finish->node_id) {

               #ifdef LOGGING
                    std::cout << "We found the destination node!\n";
                #endif
            // Found it
                break;

            } else {
               #ifdef LOGGING
                std::cout << "We are checking " << current->node_id << std::endl;

                #endif

                for (int i = 0; i < MAX_NEIGHBORS; i++) {
                    if (current->neighbors[i].second != INVALID_NEIGHBOR) {
                        char label_check = current->neighbors[i].first->node_id;
                        alt_cost = dist[current->node_id - CHAR_TO_POSITION] + current->neighbors[i].second;

                        #ifdef LOGGING
                        std::cout << "Neighbor " << label_check << " Cost " << alt_cost<< std::endl;
                        #endif
                        if (alt_cost < dist[label_check - CHAR_TO_POSITION]) {
                            dist[label_check- CHAR_TO_POSITION] = alt_cost;
                            #ifdef LOGGING
                            std::cout << "Setting prev " << label_check << " to " << current->node_id << std::endl;
                            #endif
                            prev[label_check - CHAR_TO_POSITION] = current->node_id;
                        }
                    }
                }
            }
        }

    #ifdef LOGGING
        for (int i = 0; i < NODE_COUNT; i++) {
                std::cout << prev[i] << " ";

        }
    std::cout << "\nFinished dumping prev list\n";
    #endif

    char iter = finish->node_id;

    while (prev[iter - CHAR_TO_POSITION] != INVALID_PREV) {
        #ifdef LOGGING
        std::cout << "Adding " << iter << std::endl;
        #endif

        to_return.insert(to_return.begin(),iter);
        iter = prev[iter - CHAR_TO_POSITION];
    }
    to_return.insert(to_return.begin(),start->node_id);


    #ifdef LOGGING
    std::cout << "Cost: " << dist[finish->node_id - CHAR_TO_POSITION] << std::endl;


        std::cout << "Printing path from " << start->node_id << " to " << finish->node_id << std::endl;
        for (graphInt i : to_return) {
            std::cout << this->graph[i - CHAR_TO_POSITION]->node_id << " -> ";
        }
        std::cout << "__END__\n";
    #endif

    return to_return;
}
