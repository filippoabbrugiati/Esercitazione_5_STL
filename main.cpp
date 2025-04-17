#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include <cmath>


using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;


void PrintMeshInfo(const PolygonalMesh& mesh)
{
    cout << "=== CELL 0D ===" << endl;
    for (unsigned int i = 0; i < mesh.NumCell0Ds; ++i)
    {
        cout << "Id: " << mesh.Cell0DsId[i]
             << ", Marker: " << mesh.Cell0DsMarker[i]
             << ", Coordinates: (" << mesh.Cell0DsCoordinates(0, i)
             << ", " << mesh.Cell0DsCoordinates(1, i) << ")" << endl;
    }

    cout << "\n=== CELL 1D ===" << endl;
    for (unsigned int i = 0; i < mesh.NumCell1Ds; ++i)
    {
        cout << "Id: " << mesh.Cell1DsId[i]
             << ", Marker: " << mesh.Cell1DsMarker[i]
             << ", Origin: " << mesh.Cell1DsExtrema(0, i)
             << ", End: " << mesh.Cell1DsExtrema(1, i) << endl;
    }

    cout << "\n=== CELL 2D ===" << endl;
    for (unsigned int i = 0; i < mesh.NumCell2Ds; ++i)
    {
        cout << "Id: " << mesh.Cell2DsId[i]
             << ", Marker: " << mesh.Cell2DsMarker[i]
             << "\nVertices: ";
        for (auto v : mesh.Cell2DsVertices[i])
            cout << v << " ";
        cout << "\nEdges: ";
        for (auto e : mesh.Cell2DsEdges[i])
            cout << e << " ";
        cout << "\n" << endl;
    }
}


// calcolo lunghezza segmento tra due punti
double edgeLength( double x0, double y0, double x1, double y1)
{
	 return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

// calcolo area di un poligono, utilzzando la formula di Gauss 
double polygonArea(const vector<unsigned int>& vertices, const MatrixXd coords)
{
    double area = 0.0;
    size_t n = vertices.size();
    for (size_t i = 0; i < n; i++)
    {
        unsigned int curr = vertices[i];
        unsigned int next = vertices[(i + 1) % n];
        double x0 = coords(0, curr);
        double y0 = coords(1, curr);
        double x1 = coords(0, next);
        double y1 = coords(1, next);
        area += (x0 * y1 - x1 * y0);
    }
    return fabs(area * 0.5);
}


int main()
{
	PolygonalMesh mesh;
	

	if(!ImportMesh(mesh))
	{
		cerr << "File not found" << endl;
		return 1;
	}
	

	// test 1: memorizzazione corretta marker
	bool marker0Ok = true;
	//cout << "mesh.Cell0DsMarker.size() = "<< mesh.Cell0DsMarker.size() << "mesh.NumCell0Ds = "<< mesh.NumCell0Ds << endl;
	if (mesh.Cell0DsMarker.size() != mesh.NumCell0Ds)
	{
		cerr << "I marker in Cell0D non sono stati letti correttamente" << endl;
		marker0Ok = false;
	}
	//cout << "mesh.Cell1DsMarker.size() = "<< mesh.Cell1DsMarker.size() << "mesh.NumCell1Ds = "<< mesh.NumCell1Ds << endl;
	bool marker1Ok = true;
	if (mesh.Cell1DsMarker.size() != mesh.NumCell1Ds)
	{
		cerr << "I marker in Cell1D non sono stati letti correttamente" << endl;
		marker1Ok = false;
	}
	//cout << "mesh.Cell2DsMarker.size() = "<< mesh.Cell2DsMarker.size() << "mesh.NumCell2Ds = "<< mesh.NumCell2Ds << endl;
	bool marker2Ok = true;
	if (mesh.Cell2DsMarker.size() != mesh.NumCell2Ds)
	{
		cerr << "I marker in Cell2D non sono stati letti correttamente" << endl;
		marker2Ok = false;
	}
	
	if (marker0Ok & marker1Ok & marker2Ok)
		cout << "Test 1 superato: i marker sono stati letti corretamente" << endl;
	else
		cout << "Test 1 fallito" << endl;
	
	// test 2: lunghezza spigoli
	bool edgesOk = true;
	for (unsigned int i = 0; i < mesh.NumCell1Ds; i++)
	{
		unsigned int origin = mesh.Cell1DsExtrema(0, i);
		unsigned int end = mesh.Cell1DsExtrema(1,i);
		double x0 = mesh.Cell0DsCoordinates(0, origin);
        double y0 = mesh.Cell0DsCoordinates(1, origin);
        double x1 = mesh.Cell0DsCoordinates(0, end);
        double y1 = mesh.Cell0DsCoordinates(1, end);
        double length = edgeLength(x0, y0, x1, y1);
		
		if (length < 1.0e-16)
		{
			cout << "Il nodo " << i << "ha lunghezza nulla" << endl;
			edgesOk = false;
		}
	}
	if (edgesOk)
        cout << "Test 2 superato: nessun lato ha lunghezza nulla (o inferiore alla precisione di macchina)" << endl;
	else 
		cout << "Test 2 fallito" << endl;
	
	// test 3: area poligoni
	bool areaOk = true;
	for (unsigned int i = 0; i < mesh.NumCell2Ds; i++)
    {
        const vector<unsigned int>& polyVertices = mesh.Cell2DsVertices[i];
        double area = polygonArea(polyVertices, mesh.Cell0DsCoordinates);
        if (area < 1.0e-16*sqrt(3)/2 ) //area triangolo equilatero di lato 1.0e-16
        {
            cout << "Il poligono " << i << "ha area nulla" << endl;
            areaOk = false;
        }
    }
    if (areaOk)
        cout << "Test 3 superato: nessun poligono ha area nulla (o inferiore alla precisione di macchina)" << endl;
	else 
		cout << "Test 3 fallito" << endl;
	
	cout << "Visualizzazione dati : " << endl;
	PrintMeshInfo(mesh);

    return 0;
}
