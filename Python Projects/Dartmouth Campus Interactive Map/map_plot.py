# Author: Chip Nguyen
# Date: 11/11/2022
# Purpose: Lab 4 - Map Plot Function

from bfs import bread_first_search
from load_graph import *
from cs1lib import *

WINDOW_WIDTH = 1012
WINDOW_HEIGHT = 811

dartmouth_map = load_image("dartmouth_map.png")

dict_vertices = load_graph("dartmouth_graph.txt") # Create a dictionary of vertices
start_vertex = None
end_vertex = None

def plot_map():
    global start_vertex, end_vertex

    draw_image(dartmouth_map, 0, 0)  # Draw the map

    # Draw all the vertices and edges in blue
    for vertex in dict_vertices:
        dict_vertices[vertex].draw_vertex(0, 0, 1)
        dict_vertices[vertex].draw_adjacent(0, 0, 1)

    # If mouse is pressed on a vertex, assign that vertex to start_vertex
    if is_mouse_pressed():
        for vertex in dict_vertices:
            if dict_vertices[vertex].mouse_on_vertex(mouse_x(), mouse_y()):
                start_vertex = dict_vertices[vertex]

    # If mouse is on a vertex but not pressed, assign that vertex to end_vertex
    else:
        for vertex in dict_vertices:
            if dict_vertices[vertex].mouse_on_vertex(mouse_x(), mouse_y()):
                end_vertex = dict_vertices[vertex]

    # If both start and end vertices have been selected, draws the path
    if start_vertex != None and end_vertex != None:  # checks if there is a start and goal vertex

        # Create a list of vertices between start and end using breadth first search
        path = bread_first_search(start_vertex, end_vertex)

        # Draw all the vertices in the list and the egdes connecting them in red
        for i in range(len(path)-1):
            path[i].draw_vertex(1, 0, 0)
            path[i].draw_edge(path[i+1], 1, 0, 0)

        # Draw the last vertex
        path[len(path)-1].draw_vertex(1, 0, 0)

start_graphics(plot_map, width = WINDOW_WIDTH, height = WINDOW_HEIGHT)