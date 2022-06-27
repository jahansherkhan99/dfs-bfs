/**
 ** SixDegrees.cpp
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 **   Finds a path using either breadth first search,
 **   depth first search or not commands. If no path
 **   is found then the correct error should be posted
 ** Jahansher Khan, April 30 2021
 **/

#include "SixDegrees.h"

SixDegrees::SixDegrees() {

}
/* Run
 *    Purpose: Runs a simulation of SixDegrees
 * Parameters: The first parameter infile represents
 *             the artists' datafile. The second paramter command
 *             represents the command stream. The program can either
 *             take command inputs from cin or from a filestream.
 *             Finally we have an output file. The user can specify,
 *             as in the case with the command stream whether to print to cout
 *             or to an output file.  
 *    Returns: None.
 */
void SixDegrees::run(istream &infile, istream &command, ostream &output) {
    populate_graph(infile);
    commands(output, command);
}
/* populate_graph
 *    Purpose: Inserts artists and songs into a vector based 
 *    on the datafile and calls a helper function to insert into graph.
 *    Parameters: istream &infile datafile containing all the necessary data
 *    to create vertices and edges for artists. 
 *    Returns: None.
 */

void SixDegrees::populate_graph(istream &infile) {
    string artist_name = "";
    string song_name = "";
    vector<Artist>artists;
    int i = 0;
    getline(infile, artist_name);
    while (!infile.eof()) {
        artists.push_back(artist_name);
        while (song_name != "*") {
            getline(infile, song_name);
            if (song_name != "*") {
                artists[i].add_song(song_name);
            }
        }
        song_name.clear();
        i++;
        getline(infile, artist_name);
    }
    if (artists.empty()) return;
    populate_graph_helper(artists);
}
/* populate_graph_helper
 *    Purpose: Inserts artists and songs into the graph based 
 *    based on the data stored in the vector passed to the function.
 *    Parameters: A vector containing all the artists and the respective songs
 *    they wrote. 
 *    Returns: None.
 */
void SixDegrees::populate_graph_helper(vector<Artist>&artists) {
    //inserts vertices
    for (size_t i = 0; i < artists.size(); i++) {
        graph.insert_vertex(artists[i]);
    }
     /*for loop to insert edges, which are songs that artists may have
    collaborated on */
    for (size_t i = 0; i < artists.size(); i++) {
        for (size_t j = 0; j < artists.size(); j++) {
            //ensures collaboration exists and the artists are not the same
            if (artists[i].get_collaboration(artists[j]) != "" and 
                artists[i] != artists[j]) {
                string collab = artists[i].get_collaboration(artists[j]);
                //ensures no edge already exists
                if (graph.get_edge(artists[i], artists[j]) == "") {
                    graph.insert_edge(artists[i], artists[j], collab);
                }
            }
        }
    }
}
/* commands
 *    Purpose: Takes in commands from cin or commands file, and calls the
 *    respective functions
 *    Parameters: An output stream where the results are written to and a
 *    command stream that can take commands from cin or a filestream
 *    Returns: None.
 */

void SixDegrees::commands(ostream &output, istream &command) {
    string temp = "";
    //takes inputs until eof or end of input stream
    while(getline(command, temp)) {
        //calls the bfs function
        if (temp == "bfs") {
            call_bfs(temp, output, command);
        }
        //calls the dfs function
        else if (temp == "dfs") {
            call_dfs(temp, output, command);
        }
        //calls the not function
        else if (temp == "not") {
            call_not(temp, output, command);
        }
        //quits program
        else if (temp == "quit") {
            return;
        }
        //ensures command is valid
        else {
            output << temp << " is not a command. Please try again." << endl;
        }
    }
}
/* in_graph_dfs_bfs
 *    Purpose: Ensures the two artists passed to the function exists as
 *    as vertices.
 *    Parameters: Artist1, Artist2 and an output stream. 
 *    Returns: A boolean value determing. True means both were in the 
 *    graph, false means at least one of the artists was not in the graph.
 */
bool SixDegrees::in_graph_dfs_bfs(Artist artist1, Artist artist2, 
                                  ostream &output) {
    vector<Artist>in_graph;
    //inserts the artists into a vector
    in_graph.push_back(artist1);
    in_graph.push_back(artist2);
    /*loops through the vector to see if any of the elements are not in
    in the graph */
    for (size_t i = 0; i < in_graph.size(); i++) {
        if (not graph.is_vertex(in_graph[i])) {
            output << "\"" << in_graph[i] << "\"" << 
            " was not found in the dataset :(" << endl;
        }
    }
    //if either is not in the graph a false is returned
    if (not graph.is_vertex(artist1) or not graph.is_vertex(artist2)) {
        return false;
    }
    //otherwise return true
    else return true;
}

/*    call_bfs
 *    Purpose: Creates the artists based on the user inputs and calls
 *    several functions to run the bfs command. This function acts as a
 *    liaison to other functions, essentially. 
 *    Parameters: a variable with the name of the artist, the output stream,
 *                and a command input stream. 
 *    Returns: N/A.
 */
void SixDegrees::call_bfs(string temp, ostream &output, istream &command) {
    getline(command, temp);
    //creates artist1
    Artist artist1(temp);
    getline(command, temp);
    //creates artist2
    Artist artist2(temp);
    //if either is not in the graph end the function
    if (not in_graph_dfs_bfs(artist1, artist2, output)) {
        return;
    }
    //if both artists are in graph calls bfs function
    if (bfs(artist1, artist2)) {
        //report the path if bfs returns true
        report_path(artist1, artist2, output);
    }
    //if bfs returns false then output the following message
    else {
        output << "A path does not exist between " << "\"" << 
        artist1 << "\"" << " and " << "\"" << artist2 << "\"" << "." <<endl;
    }
    //resets graph and unmarks all vertices
    graph.clear_metadata();
}
/*    bfs
 *    Purpose: Runs a breadth first search on the graph to find a path. 
 *    Parameters: Two artists, artist1 and artist2. Artist1 is the source,
 *    artist2 is the destination 
 *    Returns: A boolean. True is returned if a path is found, false if not.
 */
bool SixDegrees::bfs(Artist artist1, Artist artist2) {
    if (artist1 == artist2) {
        return false;
    }
    //relevant if not command is inputted
    if (graph.is_marked(artist1) or graph.is_marked(artist2)) {
        return false;
    }
    queue<Artist>q;
    graph.mark_vertex(artist1);
    q.push(artist1);
    while (not q.empty()) {
        vector<Artist>temp = graph.get_vertex_neighbors(q.front());
        Artist storage(q.front());
        q.pop();
        //for loop to push neighbors and set predecessors
        for (size_t i = 0; i < temp.size(); i++) {
            if (not graph.is_marked(temp[i])) {
                q.push(temp[i]);
                graph.mark_vertex(temp[i]);
                graph.set_predecessor(temp[i], storage);
            }
        }
    }
    //if artist 2 is not marked at the end return false
    if (not graph.is_marked(artist2)) {
        return false;
    }
    else return true;
}


/*    call_dfs
 *    Purpose: Creates the artists based on the user inputs and calls
 *    several functions to run the bfs command. This function acts as a
 *    liaison to other functions, essentially. 
 *    Parameters: a variable with the name of the artist, the output stream,
 *                and a command input stream. 
 *    Returns: N/A.
 */

void SixDegrees::call_dfs(string temp, ostream &output, istream &command) {
    getline(command, temp);
    //creates artist1
    Artist artist1(temp);
    getline(command, temp);
    //creates artist2
    Artist artist2(temp);
    //if either artist is not in the graph return and end the function
    if (not in_graph_dfs_bfs(artist1, artist2, output)) {
        return;
    }
    //run the depth first search function
    if (dfs(artist1, artist2)) {
        //if true then report the path
        report_path(artist1, artist2, output);
    }
    //if dfs function returns false output the following
    else {
        output << "A path does not exist between " << "\"" << 
        artist1 << "\"" << " and " << "\"" << artist2 << "\"" << "." <<endl;
    }
    //resets the graph unmarking all vertices 
    graph.clear_metadata();
}
/*    dfs
 *    Purpose: Runs a depth first search on the graph to find a path. 
 *    Parameters: Two artists, artist1 and artist2. Artist1 is the source,
 *    artist2 is the destination 
 *    Returns: A boolean. True is returned if a path is found, false if not.
 */
bool SixDegrees::dfs(Artist artist1, Artist artist2) {
    if (artist1 == artist2) return false;
    stack<Artist>s;
    s.push(artist1);
    graph.mark_vertex(artist1);
    while (not s.empty()) {
        //gets neighbors
        vector<Artist>neighbors = graph.get_vertex_neighbors(s.top());
        vector<Artist>temp;
        //pushes all unmarked neighbors into a temp vector
        for (size_t i = 0; i < neighbors.size(); i++) {
            if (not graph.is_marked(neighbors[i])) {
                temp.push_back(neighbors[i]);
            }
        }
        if (temp.empty() and not s.empty()) s.pop();
        else { //pushes first unmarked neighbor to stack
            s.push(temp[0]);
            graph.mark_vertex(s.top());
        }
        if (s.empty()) break;
        //if destination found break
        if (s.top() == artist2) break;
    }
    //assigns path if the stack has a path
    if (not s.empty()) {
        dfs_assign_path(s);
        return true;
    }
    //there is no path the stack will be empty and will return false
    else return false;
}
/*    dfs_assign_path
 *    Purpose: if a path is found then the path is marked on the graph. 
 *    Parameters: stack containing the path of the depth first search
 *    algorithm
 *    Returns: N/A.
 */
void SixDegrees::dfs_assign_path(stack<Artist>&dfs_path) {
    while (not dfs_path.empty()) {
        Artist to = dfs_path.top();
        dfs_path.pop();
        if (not dfs_path.empty()) {
            graph.set_predecessor(to, dfs_path.top());
        }
    } 
}
/* in_graph_dfs_bfs
 *    Purpose: Ensures all the artists passed to the function exists as
 *    as vertices.
 *    Parameters: Artist1, Artist2, vector of excluded artist 
 *    and an output stream. 
 *    Returns: A boolean value determing. True means all artists were in the 
 *    graph, false means at least one of the artists was not in the graph.
 */
bool SixDegrees::in_graph_not(Artist artist1, Artist artist2, 
                              vector<Artist>&excluded, ostream &output) {
    vector<Artist>in_graph;
    //pushes sorce and destination artists into vector
    in_graph.push_back(artist1);
    in_graph.push_back(artist2);
    //pushes excluded artists into vector
    for (size_t i = 0; i < excluded.size(); i++) {
        in_graph.push_back(excluded[i]);
    }
    //determines if all artists in vector are in graph
    for (size_t i = 0; i < in_graph.size(); i++) {
        if (not graph.is_vertex(in_graph[i])) {
            output << "\"" << in_graph[i] << "\"" << 
            " was not found in the dataset :(" << endl;
        }
    }
    //if the source or destination is nit in graph return false
    if (not graph.is_vertex(artist1) or not graph.is_vertex(artist2)) {
        return false;
    }
    //if any of the excluded artists are not in graph then return false
    for (size_t i = 0; i < excluded.size(); i++) {
        if (not graph.is_vertex(excluded[i])) return false;
    }
    //if all are in graph then return true
    return true;
}
/*    call_not
 *    Purpose: Creates the artists based on the user inputs and calls
 *    several functions to run the not command. This function acts as a
 *    liaison to other functions, essentially. 
 *    Parameters: a variable with the name of the artist, the output stream,
 *                and a command input stream. 
 *    Returns: N/A.
 */
void SixDegrees::call_not(string temp, ostream &output, istream &command) {
    getline(command, temp);
    Artist artist1(temp);
    getline(command, temp);
    Artist artist2(temp);
    vector<Artist>excluded;
    //receive input for excluded artists until "*" character is entered
    while (temp != "*") {
        getline(command, temp);
        if (temp!="*") {
            Artist artist_not(temp);
            excluded.push_back(artist_not);
        }
    }
    //if any of the artists are not in the function then quit
    if (not in_graph_not(artist1, artist2, excluded, output)) return;
    //call the not command funciton
    if (not_command(artist1, artist2, excluded)) {
        //if not command is true then report the path
        report_path(artist1, artist2, output);
    }
    //if no path is found then output the following
    else {
        output << "A path does not exist between " << "\"" << 
        artist1 << "\"" << " and " << "\"" << artist2 << "\"" << "." << endl;
    }
    //resets graph and unmarks all vertices
    graph.clear_metadata();
}
/*    not_command
 *    Purpose: Marks all excluded vertices and runs a breadth first search. 
 *    Parameters: Two artists, artist1 and artist2 and a vector of excluded
 *     artists. Artist1 is the source, artist2 is the destination.
 *    Returns: A boolean. True is returned if a path is found, false if not.
 */
bool SixDegrees::not_command(Artist artist1, Artist artist2, 
                             vector<Artist>&excluded) {
    //mark all excluded vertices
    for (size_t i = 0; i < excluded.size(); i++) {
        graph.mark_vertex(excluded[i]);
    }
    /*calls breadth first search after excluding vertices. If a path is found
    then return true */
    if (bfs(artist1, artist2)) return true;
    //if no path is found then return false
    else return false;
}
/*    report_path
 *    Purpose: Outputs the path if it's found. This function is only called
 *    if a path is found so it does not account for a situation where a path
 *    is not found 
 *    Parameters: Two artists, artist1 and artist2 and the output stream.
 *    Artist1 is the source, artist2 is the destination.
 *    N/A.
 */
void SixDegrees::report_path(Artist artist1, Artist artist2, ostream &output) 
{
    //creates a stack of the path
    stack<Artist>path = graph.report_path(artist1, artist2);
    //outputs the path until the stack is empty
    while (not path.empty()) {
        Artist a = path.top();
        path.pop();
        /*once the path is empty output the following to indicate the path is 
        complete*/
        if (path.empty()) {
            output << "***" << endl;
            return;
        }
        Artist b = path.top();
        string edge = graph.get_edge(a, b);
        output << "\"" << a.get_name() << "\"" << " collaborated with " << 
        "\"" << b.get_name() << "\"" << " in " << "\"" << 
        edge << "\"." << endl;
    }
}