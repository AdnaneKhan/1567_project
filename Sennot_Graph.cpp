#include <iostream>
#include "Sennot_Graph.h"

void Sennot_Graph::initialize_paths() {
    for (int i = 0; i < NODE_COUNT; i++) {
        // set initial steps
        (this->graph[i]) = new Node(nodes[i]);
        step_lists.at(i).push_back(this->graph[i]);
    }
}

void Sennot_Graph::initialize_graph() {
    depth = 0;
    num_paths = NODE_COUNT;

    // For each each in graph, add connection from node0 to node1 in one direction, then add edge from node1 to node0 in the opposite
    // direction
    for (int i = 0; i < 12; i++) {
        (this->graph.at(edges[i][0] - 65))->add_neighbor(graph.at(edges[i][1] - CHAR_TO_POSITION), edges[i][2], edges[i][3]);
        (this->graph.at(edges[i][1] - 65))->add_neighbor(graph.at(edges[i][0] - CHAR_TO_POSITION), edges[i][2], (edges[i][3] + 2) % 4);
    }
}


 Sennot_Graph::Sennot_Graph() {
    initialize_paths();
    initialize_graph();

    edge_progress = 0;
    depth = 0;
    num_paths = NODE_COUNT;
}

 Sennot_Graph::~Sennot_Graph() {
    for (int i = 0; i < NODE_COUNT; i++) {
        delete(this->graph[i]);
    }
}

graphInt Sennot_Graph::path_count() {
    return num_paths;
}

/**
*  \param step_count number of steps that were travelled along edge to reach this node (intersection)
*/
cardinalDirection Sennot_Graph::graph_intersect(cardinalDirection next_dir) {

   // cardinalDirection next_dir = next_step_m();
 //   cardinalDirection next_dir = next_step(recent_metrics);
    #ifdef LOGGING
        std::cout << next_dir << " is our next step (cardinal).\n";
    #endif
    for (int i = 0; i < NODE_COUNT; i++) {
        if (step_lists[i].size() > depth) {
            Node *temp = step_lists[i].back();


            // Iterate through all options NOT in the direction we came from
            //for (int j = 0; j < MAX_NEIGHBORS; j++) {
            if (temp->neighbors[next_dir].second != -1) {
                // Check if we have an opening

                // Turn in this direction
                depth++;
                step_lists[i].push_back(temp->neighbors[next_dir].first);

                // Clear st
                    #ifdef LOGGING
                    std::cout << " We are potentially going from " << temp->node_label << " to " << temp->neighbors[next_dir].first->node_label << std::endl;
                    #endif
            }
            //}
        }
    }
    // reset progress
    edge_progress = 0;

    //return convert_dir(next_dir, curr_heading);
    return next_dir;
}

/**
* Prunes possible paths through graph to correspond with how many lights the current path has passed.
*
*
* \param path_cost cost of the edge we are currently travelling upon
*/
graphInt Sennot_Graph::graph_step() {

    int keep_path = 0;

    for (int i = 0; i < NODE_COUNT; i++) {
        if (step_lists[i].size() > depth) {

            Node *temp = step_lists[i].back();

            //printf("We are at %c \n",temp->node_label);

            for (int j = 0; j < MAX_NEIGHBORS; j++) {
                // If at least one of the neighbors of the node represents a valid movement we can keep this traversal
                // in set of possible routes
                if (temp->neighbors[j].second != -1 && temp->neighbors[j].second > edge_cost) {
                    //
                    keep_path = 1;
                }
            }

            // If all paths are less than edge progress, then we remove the base from vector
            //
            if (!keep_path) {
                // step_lists[i].clear();
                //num_paths--;
                #ifdef LOGGING
                std::cout << "We have reached a point where we would have removed steps, but keeping\n";
                #endif
            }
        }
    }

    return 0;
}
