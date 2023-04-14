# Author: Chip Nguyen
# Date: 11/15/2022
# Purpose: Breadth First Search

from collections import deque
from vertex import *

def bread_first_search(start_vertex, end_vertex):

    # Create a queue and add start_vertex to it
    queue = deque()
    queue.append(start_vertex)

    # Create a dictionary of backpointers
    dict_backpointers = {}

    while len(queue) > 0:
        current_vertex = queue.popleft()  # Sets the current vertex to the one that has been on the queue the longest

        # For vertices that are adjacent to the current vertex, if they are not in the backpointers dictionary,
        # add them to the queue and set the backpointer of the adjacent vertex to the current vertex
        for vertex in current_vertex.adjacency_list:
            if vertex not in dict_backpointers:
                queue.append(vertex)
                dict_backpointers[vertex] = current_vertex

            # Return a path list if the end vertex has been reached
            if vertex == end_vertex:
                path = [end_vertex]

                # Backchaining to create a list of vertices that connect start vertex and end vertex
                while start_vertex != end_vertex:
                    end_vertex = dict_backpointers[end_vertex]
                    path.append(end_vertex)

                # Return the list
                return path