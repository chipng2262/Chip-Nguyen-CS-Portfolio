# Author: Chip Nguyen
# Date: 11/11/2022
# Purpose: Lab 4 - Vertex class

from cs1lib import *

class Vertex:
    def __init__(self, name, x, y):
        self.name = name
        self.x = int(x)
        self.y = int(y)
        self.adjacency_list = []

    def __str__(self):
        # Initiate the string
        adjacent_string = ""
        index = 0

        while index in range(len(self.adjacency_list)):
            # If the vertex is not last in the adjacency list, print its name with ", " at the end
            if index < len(self.adjacency_list) - 1:
                adjacent_string = adjacent_string + str(self.adjacency_list[index].name) + ", "
            # If the vertex is not last in the adjacency list, print its name without ", " at the end
            else:
                adjacent_string = adjacent_string + str(self.adjacency_list[index].name)

            index = index + 1

        return self.name + "; Location: " +  str(self.x) + ", " + str(self.y) + "; Adjacent vertices: " + adjacent_string

    # Method to determine if the mouse in on a vertex. mx, my represent location of the mouse
    def mouse_on_vertex(self, mx, my):
        if self.x - 9 <= mx <= self.x + 9 and self.y - 9 <=my <= self.y + 9:
            return True
        else:
            return False

    # Method to draw the vertex
    def draw_vertex(self, r, g, b):
        disable_stroke()
        set_fill_color(r, g, b)
        draw_circle(self.x, self.y, 9)

    # Method to draw the edges
    def draw_edge(self, vertex, r, g, b):
        enable_stroke()
        set_stroke_width(5)
        set_stroke_color(r, g, b)
        draw_line(self.x, self.y, vertex.x, vertex.y)

    # Method to draw the edges connecting adjacent vertices
    def draw_adjacent(self, r, g, b):
        for vertex in self.adjacency_list:
            self.draw_edge(vertex, r, g, b)