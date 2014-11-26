
#include "Sennot_Graph.h"

#include <iostream>

#define LOGGING


 Sennot_Graph::~Sennot_Graph() {
    for (int i = 0; i < NODE_COUNT; i++) {
        delete(this->graph[i]);
    }
}

nodeLabel Sennot_Graph::get_last_node(int path_length) {
    nodeLabel ret;

    if (num_paths == 1) {
        for (std::list<Node*> list : step_lists) {
            if (list.size() == path_length) {

                #ifdef LOGGING
                    std::cout << "Returning " << ret << " from get last node. \n";
                #endif
                ret = list.back()->node_id;

                break;
            }
        }
    } else {
        #ifdef LOGGING
        std::cout << "There was no step list that matched the current depth";
        #endif
        ret = -1;
    }

    return ret;
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
        return_node = this->graph.at(index);
    } else {
        // Problem
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


/**
*  \param step_count number of steps that were travelled along edge to reach this node (intersection)
*/
cardinalDirection Sennot_Graph::intersection_action(cardinalDirection next_dir) {
    bool increment = false;

    #ifdef LOGGING
        std::cout << next_dir << " is our next step (cardinal).\n";
    #endif
    for (int i = 0; i < NODE_COUNT; i++) {
        if (step_lists[i].size() > depth) {
            Node *temp = step_lists[i].back();


            if (temp->neighbors[next_dir].second != -1) {
                // Check if we have an opening

                // Turn in this direction
                increment = true;
                step_lists[i].push_back(temp->neighbors[next_dir].first);

                    #ifdef LOGGING
                    std::cout << " We are potentially going from " << temp->node_id << " to " << temp->neighbors[next_dir].first->node_id << std::endl;
                    #endif


            } else {
                // Reduce the number of paths
                if (num_paths > 1) {
                    num_paths--;
                } else {
                    #ifdef LOGGING
                      std::cout << " We are in a state where we would be reducing paths below one.\n";
                    #endif
                }

            }
        }
    }
    // reset progress
    edge_progress = 0;

    // If a turn was made then depth is increased correspondingly
    if (increment) {
        depth++;
    }

    return next_dir;
}

/**
* Prunes possible paths through graph to correspond with how many lights the current path has passed.
*
*
* \param path_cost cost of the edge we are currently travelling upon
*/
graphInt Sennot_Graph::edge_step() {

    int keep_path = 0;
    edge_progress++;

    for (int i = 0; i < NODE_COUNT; i++) {
        if (step_lists[i].size() > depth) {

            Node *temp = step_lists[i].back();

            #ifdef LOGGING
            std::cout << "We are checking " << temp->node_id << " curr prog:" << edge_progress;
            #endif

            for (int j = 0; j < MAX_NEIGHBORS; j++) {
                // If at least one of the neighbors of the node represents a valid movement we can keep this traversal
                // in set of possible routes
                if (temp->neighbors[j].second != -1 && temp->neighbors[j].second >= edge_progress) {
                    #ifdef LOGGING
                    std::cout << " with " <<temp->neighbors[j].first->node_id<< ",Cost:" << temp->neighbors[j].second;
                    #endif
                    //
                    keep_path = 1;
                }

            }
            #ifdef LOGGING
                std::cout << std::endl;
            #endif

            if (!keep_path) {

                num_paths--;
                #ifdef LOGGING
                std::cout << "We have reached a point where we would have removed steps, but keeping.\n";
                #endif
                keep_path = 0;
            }
        }
    }

    return 0;
}

void Sennot_Graph::initialize_paths() {
    for (int i = 0; i < NODE_COUNT; i++) {
        // set initial steps
        (graph[i]) = new Node(nodes[i]);
        step_lists.at(i).push_back(graph[i]);
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

std::list<nodeLabel> Sennot_Graph::find_path(nodeLabel start, nodeLabel finish) {
    Node * source = graph[start - CHAR_TO_POSITION];
    Node * dest = graph[finish - CHAR_TO_POSITION];

    return find_path(source, dest);
}

std::list<nodeLabel> Sennot_Graph::find_path(Node *start, Node *finish) {
    std::list<nodeLabel> to_return;

    // Used to mark nodes as visited
    // 0 -> clear
    // 1 -> visited
    char node_mark[NODE_COUNT] = {0};
    int dist[NODE_COUNT] = {0};
    char prev[NODE_COUNT] = {0};

    std::vector<Node *> bfs_queue;

    dist[start->node_id - CHAR_TO_POSITION] = 0;
    prev[start->node_id - CHAR_TO_POSITION] = -1;

    for (Node * n : this->graph) {
        if (n->node_id != start->node_id) {
            dist[n->node_id - CHAR_TO_POSITION] = 9999;
            prev[n->node_id - CHAR_TO_POSITION] = -1;
        }

        bfs_queue.push_back(n);
    }

        while (!bfs_queue.empty()) {
            int alt_cost;

            int min_c = dist[bfs_queue.front()->node_id - CHAR_TO_POSITION];
            int iter;
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

               #ifdef DEBUG
                    std::cout << "We found the destination node!\n";
                #endif
            // Found it
                break;

            } else {
               #ifdef DEBUG
                std::cout << "We are checking " << current->node_label << std::endl;

                #endif

                for (int i = 0; i < MAX_NEIGHBORS; i++) {
                    if (current->neighbors[i].second != -1) {
                        char label_check = current->neighbors[i].first->node_id;
                        alt_cost = dist[current->node_id - CHAR_TO_POSITION] + current->neighbors[i].second;

                        #ifdef DEBUG
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

    char iter = finish->node_id;

    while (prev[iter - CHAR_TO_POSITION] != -1) {
        to_return.push_front(iter);
        iter = prev[iter - CHAR_TO_POSITION];
    }
    to_return.push_front(start->node_id);


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
