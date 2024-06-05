import clang.cindex
import os

from typing import List, Dict

scripts_folder = os.path.dirname(os.path.abspath(__file__))

file_contents: str = ''


def load_content(path):
    global file_contents
    with open(path, 'r') as file:
        file_contents = file.read()


def get_string(input: clang.cindex.SourceRange) -> str:
    return file_contents[input.start.offset : input.end.offset]


class FunctionParam:
    def __init__(self, name: str, proto: str):
        if proto.endswith('='):
            # We currently can't retrieve the default value, the definition of the arg just ends in '='
            # Instead we remove this and rely on the py::arg(...) = ... to set a default value
            proto = proto[:-2]
        self.proto = proto
        self.name = name


class ConnectionMethod:
    def __init__(self, name: str, params: List[FunctionParam], is_void: bool):
        self.name = name
        self.params = params
        self.is_void = is_void


def traverse(class_name, node, methods_dict):
    if node.kind == clang.cindex.CursorKind.STRUCT_DECL or node.kind == clang.cindex.CursorKind.CLASS_DECL:
        if node.spelling != class_name:
            return
        for child in node.get_children():
            traverse(class_name, child, methods_dict)
    elif node.kind == clang.cindex.CursorKind.CXX_METHOD:
        name = node.spelling
        return_type = node.type.get_result().spelling
        is_void = return_type == "void"
        params = [FunctionParam(x.spelling, get_string(x.extent)) for x in node.get_arguments()]

        methods_dict[name] = ConnectionMethod(name, params, is_void)
    else:
        for child in node.get_children():
            traverse(class_name, child, methods_dict)


def get_methods(class_name: str) -> Dict[str, ConnectionMethod]:
    CLASSES = {
        'DuckDBPyConnection': os.path.join(
            scripts_folder, '..', 'src', 'include', 'duckdb_python', 'pyconnection', 'pyconnection.hpp'
        ),
        'DuckDBPyRelation': os.path.join(scripts_folder, '..', 'src', 'include', 'duckdb_python', 'pyrelation.hpp'),
    }
    # Create a dictionary to store method names and prototypes
    methods_dict = {}

    path = CLASSES[class_name]
    load_content(path)

    index = clang.cindex.Index.create()
    tu = index.parse(path, args=['-std=c++11'])
    traverse(class_name, tu.cursor, methods_dict)

    return methods_dict


if __name__ == '__main__':
    print("This module should not called directly, please use `make generate-files` instead")
    exit(1)
