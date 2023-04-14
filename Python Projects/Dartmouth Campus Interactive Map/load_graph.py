# Author: Chip Nguyen
# Date: 11/11/2022
# Purpose: Load Graph Function

from vertex import Vertex

def load_graph(file):
    in_file1 = open(file, "r")
    # Initiate the dictionary
    vertex_dict = {}

    # Add vertices to the dictionary
    for line in in_file1:
        # Put the main information pieces, currently separated by ";" in a list
        information_piece = line.split(";")
        # Get the name of the vertex from the list
        name = information_piece[0]

        # Parse and get the coordinates
        coordinate = information_piece[2].split(",")
        x = coordinate[0].strip()
        y = coordinate[1].strip()

        # Create vertex objects and add them to the dictionary of vertices
        vertex_object = Vertex(name, x, y)
        vertex_dict[name] = vertex_object

    in_file1.close()
    in_file2 = open(file, "r")

    # Add adjacency lists into the dictionary
    for line in in_file2:
        # Put the main information pieces, currently separated by ";" in a list
        information_piece = line.split(";")
        # Get the name of the vertex from the list
        name = information_piece[0]

        # Parse and create a list of adjacent vertices
        adjacent_vertices_list = information_piece[1].split(",")

        # Add adjacent vertices of a current vertex to the dictionary
        for vertex in adjacent_vertices_list:
            current_vertex = vertex_dict[name]
            current_vertex.adjacency_list.append(vertex_dict[vertex.strip()])

    return vertex_dict
    in_file2.close()

print(load_graph("dartmouth_graph.txt"))


