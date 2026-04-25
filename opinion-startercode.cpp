#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(string filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(string filename); // reads file into adjacency list, defined later

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.
int total_ones = 0;  // we keep this so calculating fraction is O(1)


/****************************************************************/

/******** Create adjacency list and vector of opinions */
// simple vector to hold each node's opinion (0 or 1)
std::vector<int> opinions;

// incoming_neighbors[target] stores all nodes that influence target
// this is better than a matrix because it only stores real edges
std::vector<std::vector<int>> incoming_neighbors;

void make_space_for_node(int node)
{
    // if we see a larger id, grow all node based vectors once
    if (node < total_nodes)
        return;

    total_nodes = node + 1;
    opinions.resize(total_nodes, 0);
    incoming_neighbors.resize(total_nodes);
}

double calculate_fraction_of_ones()
{
   // (3) Calculate the fraction of nodes with opinion 1 and return it.
   // we do not loop over all nodes each time now

   if (total_nodes == 0)
        return 0.0;
   
   return (double) total_ones / total_nodes;
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    // (4) Count the number of neighbours with opinion 0 and opinion 1. Return the majority (0 or 1). 
    //If tie, return 0.

    int count_ones = 0;
    int count_zeroes = 0;

    // only loop through the real neighbours of this node
    for (int neighbor : incoming_neighbors[node])
    {
        if (opinions[neighbor] == 1)
            count_ones++;
        else
            count_zeroes++;
    }

    return (count_ones>count_zeroes) ? 1 : 0;
}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{
    // (5) For each node, calculate the majority opinion among its neighbours and update the node's opinion.
    // Return true if any node's opinion changed, false otherwise.
    
    // so even if a single opinion is changed we return true.
    // I can create a copy of the opinions vector and then store the changes there

    bool changed = false;

    vector<int> new_opinions = opinions;
    int new_total_ones = 0;

    for (int i = 0; i<total_nodes; i++)
    {
        new_opinions[i] = get_majority_friend_opinions(i);  //for the ith node we look at the neighbours and get the majority opinion

        if (new_opinions[i] != opinions[i])
            changed = true;

        if (new_opinions[i] == 1)
            new_total_ones++;
    }

    // once we are done with creating the new opinions vector we can replace the old opinion
    opinions = new_opinions;
    total_ones = new_total_ones;
    return changed;
}

int main() {
    // no preallocation; vectors grow on demand

    // Read input files
    read_opinions("opinions.txt"); 
    read_edges("edge_list.txt");
    
    cout << "Total nodes: " << total_nodes << endl;
    
    // Run simulation
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;
    
    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;
    
    /// (6)  //////////////////////////////////////////////
    // (6) Run until consensus or max iterations

    // we have to run this for a mac_iterations 'and' until we have changing opinions 
    while (iteration< max_iterations && opinions_changed)
    {
        double current_fraction = calculate_fraction_of_ones();

        if (current_fraction == 0.0 || current_fraction == 1.0)
            break;

        opinions_changed = update_opinions();

        iteration++;

        //print after every update of opinion
        cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;

    }

    ////////////////////////////////////////////////////////
    // Print final result
    double final_fraction = calculate_fraction_of_ones();
    
    if(final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if(final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;
    
    return 0;
}


/*********** Functions to read files **************************/ 

// Read opinion vector from file.
void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;
    while(file >> id >> opinion)
    {
        make_space_for_node(id);

        // if this node already had opinion 1, remove it before overwriting
        if (opinions[id] == 1)
            total_ones--;

        opinions[id] = opinion;

        if (opinion == 1)
            total_ones++;
    }
    file.close();
}

// Read edges and update total nodes as needed.
void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;
    
    while(file >> source >> target)
    {
        make_space_for_node(source);
        make_space_for_node(target);

        // source influences target, so target stores source as a neighbour
        incoming_neighbors[target].push_back(source);
    }
    file.close();
}

/********************************************************************** */
