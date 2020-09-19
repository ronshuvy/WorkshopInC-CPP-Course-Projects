import subprocess
import sys
import json
import uuid
import urllib.parse
import webbrowser


def install(package):
    subprocess.check_call([sys.executable, "-m", "pip", "install", package])


try:
    import pydot
except ModuleNotFoundError:
    print("module not found, installing")
    install('pydot')
    import pydot


def get_tree_json(filename):
    with open(filename) as json_file:
        data = json.load(json_file)
        return data


def tree_to_graph(tree, graph):
    if tree is None:
        label = ''
        leaf = pydot.Node(str(uuid.uuid4()), label=label, shape="box", style='filled', fillcolor="black",  width=.1, height=.1)
        graph.add_node(leaf)
        return leaf

    label = tree["data"]
    color = 'red' if (tree["color"] == 'r') else 'black'
    root = pydot.Node(id(tree), label=label, style='filled', fontcolor='white', shape="circle", fillcolor=color)
    graph.add_node(root)

    neg_node = tree_to_graph(tree["left"], graph)
    graph.add_edge(pydot.Edge(root, neg_node))
    pos_node = tree_to_graph(tree["right"], graph)
    graph.add_edge(pydot.Edge(root, pos_node))

    return root


def draw_tree(tree):
    graph = pydot.Dot(graph_type='graph')
    tree_to_graph(tree, graph)
    url = "https://dreampuf.github.io/GraphvizOnline/#" + urllib.parse.quote(graph.to_string())
    webbrowser.open_new(url)
    print(url)


if __name__ == '__main__':
    if len(sys.argv) == 2:
        tree = get_tree_json(sys.argv[1])
        draw_tree(tree)
    else:
        print("usage error")
