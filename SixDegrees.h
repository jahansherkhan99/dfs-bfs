/**
 ** SixDegrees.h
 **
 ** Project Two: Six Degrees of Collaboration
 **
 ** Purpose:
 **   Public Interface of SixDegrees.cpp
 ** Jahansher Khan, April 30, 2021
 **/



#ifndef _SIXDEGREES_H_
#define _SIXDEGREES_H_

#include "CollabGraph.h"
#include "Artist.h"
#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

class SixDegrees {
    public:
        SixDegrees();
        void run (istream &infile, istream &command, ostream &output);
    private:
        CollabGraph graph;
        void commands(ostream &output, istream &command);
        void populate_graph(istream &infile);
        void populate_graph_helper(vector<Artist>&artists);
        bool in_graph_dfs_bfs(Artist artist1, Artist artist2,
                              ostream &output);
        bool in_graph_not(Artist artist1, Artist artist2, 
                          vector<Artist>&excluded, ostream &output);
        bool bfs(Artist artist1, Artist artist2);
        void call_bfs(string temp, ostream &output, istream &command);
        bool dfs(Artist artist1, Artist artist2);
        void call_dfs(string temp, ostream &output, istream &command);
        void dfs_assign_path(stack<Artist>&dfs_path);
        bool not_command(Artist artist1, Artist artist2, 
                         vector<Artist>&excluded);
        void call_not(string temp, ostream &output, istream &command);
        void report_path(Artist artist1, Artist artist2, ostream &output);
        void testing(ostream &output, istream &command, Artist &artist1, 
                     Artist &artist2);
        void testing_dfs(ostream &output, istream &command, 
                         Artist &artist1, Artist &artist2);
        void testing_not(ostream &output, istream &command, Artist &artist1, 
                         Artist &artist2,vector<Artist>excluded);
};

#endif
