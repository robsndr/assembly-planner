import xml.etree.ElementTree as ET

class AMFTree:
    """ Class representing the XML-Based AMF-Tree.
    """

    def __init__(self, filename):
        tree = ET.parse(filename)
        root = tree.getroot()
        print(root.attrib)

class Object:
    """ Class implementing a vertex required to represent a geometry in AMF.
    """        


class Vertex:
    """ Class implementing a vertex required to represent a geometry in AMF.
    """

    def __init__(self, xml_element):
        for coordniates in xml_element.findall('coordinates'):
            self.x = coordniates.find('x').text
            self.y = coordniates.find('y').text
            self.z = coordniates.find('z').text

    def __str__(self):
        return "Vertex: " + "X=" + self.x + "  Y=" + self.y + "  Z=" + self.z 

    __repr__ = __str__


class Triangle:
    """ Class implementing a vertex required to represent a geometry in AMF.
    """

    def __init__(self, triangle_xml):
        self.v1 = triangle_xml.find('v1').text
        self.v2 = triangle_xml.find('v2').text
        self.v3 = triangle_xml.find('v3').text

    def __str__(self):
        return "Triangle:  " + "v1_id=" + self.v1 + "  v2_id=" + self.v2 + "  v3_id=" + self.v3 

    __repr__ = __str__

   def vertices():
       yield self.v1
       yield self.v2
       yield self.v3


class Mesh:
    """ Class implementing a vertex required to represent a geometry in AMF.
    """

    def __init__(self, xml_element):
        self.vertices = [Vertex(vertex) for vertex in xml_element.findall('vertex')]
        self.volume   = [Triangle(triangle) for triangle in xml_element.findall('triangle')]    
