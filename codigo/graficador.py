import networkx as nx
import numpy as np
import csv
from numpy import loadtxt
import matplotlib.pyplot as plt
import pylab

def graficarGrafo(nombre):
    G = nx.Graph()
    path = '../../codigo/grafos/' + nombre + ".csv"
    f = plt.figure(nombre)
    with open(path) as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            n = row[3]
            #print(n)
            G.add_edge(row[0], row[1],cluster=row[2], weight=n)
    pos = nx.spring_layout(G)

    #print(list(G.edges()))
    edge_weights = {(u,v):d['weight'] for u,v,d in G.edges(data=True)}
    #nx.draw(G, pos = pos, with_labels=True)
    # nodes
    #nx.draw_networkx_nodes(G, pos, node_size=1)
    #edge
    edges = G.edges()
    clusters = [G[u][v]['cluster'] for u,v in edges]
    colors = []
    for c in clusters:
        if (int(c) == -1):
            colors+=['g']
        else:
            colors=['g']
    ejes = [(u, v) for (u, v,d) in G.edges(data=True)]
    #colors = range(20)
    nx.draw(G, pos,edges=ejes, node_color='r', edge_color=colors,
        width=1, edge_cmap=plt.cm.Blues, with_labels=True)

    #nx.draw_networkx_edges(G,pos,edges=ejes,width=1, alpha=0.0)# edge_color='g', style='dashed')
    nx.draw_networkx_edge_labels(G,pos,font_size=10,edge_labels=edge_weights)

    #nx.draw(G, pos = pos, with_labels=True)
    #nx.draw_networkx_edge_labels(G, pos)
    #nx.draw(G, with_labels=True)
    #plt.draw()

def graficarNodos(nombre):
    G = nx.Graph()
    path = '../../codigo/grafos/' + nombre + ".csv"
    f = plt.figure(nombre)
    lista = []
    fixed_positions = {}
    with open(path) as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            lista = lista + row
            fixed_positions[row[0]] = (float(row[1]),float(row[2]))
            G.add_node(row[0], pos=(row[1],row[2]))
        #G.add_nodes_from(lista)
    fixed_nodes = fixed_positions.keys()
    print(fixed_positions)
    pos = nx.spring_layout(G,pos=fixed_positions, fixed = fixed_nodes)
    nx.draw_networkx(G,pos)
    #pos = nx.get_node_attributes(G,'pos')
    #pos = nx.spring_layout(G)
    #nx.draw(G,pos,with_labels=True)
    #nx.draw_networkx_nodes(G,pos=pos,with_labels='True')
    #nx.draw(G, pos=pos, with_labels=True, node_size=35)


def main():
    graficarGrafo('grafo')
    graficarGrafo('agm')
    graficarNodos("nodos")
    plt.show()

main()
