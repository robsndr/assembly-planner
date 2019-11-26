import xml.etree.ElementTree as ET


class AMFTree:
    """ Class representing the XML-Based AMF-Tree.
    """

    def __init__(self, filename):
        tree = ET.parse(filename)
        root_element = tree.getroot()
        print(root_element.attrib)

        # self.top_constellation = root_element.findall('constellation')
        self.objects = {object_.attrib["id"]: object_ for object_ in root_element.findall('object')}
        self.instances = list()

        for _instance in root_element.iter('instance'):
            current_id = _instance.attrib["objectid"]

            if current_id in self.objects.keys():
                temp = Object(self.objects[current_id],
                              float(_instance.find("deltax").text.replace(',', '.')),
                              float(_instance.find("deltay").text.replace(',', '.')),
                              float(_instance.find("deltaz").text.replace(',', '.')))
                self.instances.append(temp)


class Object:
    """ Class implementing a vertex required to represent a geometry in AMF.
    """

    def __init__(self, object_xml_element, dx=0, dy=0, dz=0):
        self.id = int(object_xml_element.attrib["id"])
        self.mesh = Mesh(object_xml_element.find('mesh'))
        self.colors = None  # colors not supported yet.
        self.delta_x_ = dx
        self.delta_y_ = dy
        self.delta_z_ = dz

    @property
    def delta_x(self):
        return self.delta_x_

    @delta_x.setter
    def delta_x(self, value):
        self.delta_x_ = value

    @property
    def delta_y(self):
        return self.delta_y_

    @delta_y.setter
    def delta_y(self, value):
        self.delta_y_ = value

    @property
    def delta_z(self):
        return self.delta_z_

    @delta_z.setter
    def delta_z(self, value):
        self.delta_z_ = value


class Vertex:
    """ Class implementing a vertex required to represent a geometry in AMF.
    """

    def __init__(self, vertex_xml_element):
        for coordinates in vertex_xml_element.findall('coordinates'):
            self.x = float(coordinates.find('x').text)
            self.y = float(coordinates.find('y').text)
            self.z = float(coordinates.find('z').text)

    def __str__(self):
        return "Vertex: " + "X=" + self.x + "  Y=" + self.y + "  Z=" + self.z

    __repr__ = __str__


class Triangle:
    """ Class implementing a vertex required to represent a geometry in AMF.
    """

    def __init__(self, triangle_xml_element):
        self.v1_id = int(triangle_xml_element.find('v1').text)
        self.v2_id = int(triangle_xml_element.find('v2').text)
        self.v3_id = int(triangle_xml_element.find('v3').text)

    def __str__(self):
        return "Triangle:  " + "v1_id=" + self.v1_id + "  v2_id=" + self.v2_id + "  v3_id=" + self.v3_id

    __repr__ = __str__

    def vertices(self):
        yield self.v1_id
        yield self.v2_id
        yield self.v3_id


class Mesh:
    """ Class implementing a vertex required to represent a geometry in AMF.
    """

    def __init__(self, mesh_xml_element):
        self.vertices = [Vertex(vertex) for vertex in mesh_xml_element.findall('vertex')]
        self.volume = [Triangle(triangle) for triangle in mesh_xml_element.findall('triangle')]

# class Constellation:
#     """ Class implementing a vertex required to represent a geometry in AMF.
#     """    

#     def __init__(self, object_xml_element):
#         self.id = int(object_xml_element.attrib["id"])
#         self.instance_ids = [ instance_.attrib["id"] for instance_ in object_xml_element.findall('instance') ]
