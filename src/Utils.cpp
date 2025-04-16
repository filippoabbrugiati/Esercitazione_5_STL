#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


namespace PolygonalLibrary
{	
	bool ImportMesh(PolygonalMesh& mesh)
	{	

	if(!ImportCell0Ds(mesh))
		return false;
	
	if(!ImportCell1Ds(mesh))
		return false;
	
	if(!ImportCell2Ds(mesh))
		return false;
	
	return true;
	}

	////////////////////////////////////////////////////////////////////////////////
	bool ImportCell0Ds(PolygonalMesh& mesh)
	{
		ifstream file("./Cell0Ds.csv");
		
		if(file.fail())
			return false;
		
		list<string> listLines;
		string line;
		while (getline(file, line))
			listLines.push_back(line);
		file.close();
		
		listLines.pop_front();
		
		mesh.NumCell0Ds = listLines.size();

		if (mesh.NumCell0Ds == 0)
		{
			cerr << "There is no cell 0D" << endl;
			return false;
		}
		
		mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
		mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);
		mesh.Cell0DsMarker.reserve(mesh.NumCell0Ds);
		
		for (const string& line : listLines)
		{
			istringstream converter(line);
			
			unsigned int id;
			unsigned int marker;
			Vector2d coord;
			char tmp;
			
			converter >> id >> tmp >> marker >> tmp >> mesh.Cell0DsCoordinates(0, id)
			>> tmp >> mesh.Cell0DsCoordinates(1, id);
			
			mesh.Cell0DsId.push_back(id);
			mesh.Cell0DsMarker.push_back(marker);

		}
		
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////
	bool ImportCell1Ds(PolygonalMesh& mesh)
	{
		ifstream file("./Cell1Ds.csv");

		if(file.fail())
			return false;

		list<string> listLines;
		string line;
		while (getline(file, line))
			listLines.push_back(line);

		file.close();

		listLines.pop_front();

		mesh.NumCell1Ds = listLines.size();

		if (mesh.NumCell1Ds == 0)
		{
			cerr << "There is no cell 1D" << endl;
			return false;
		}

		mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
		mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);
		mesh.Cell1DsMarker.reserve(mesh.NumCell1Ds);


		for (const string& line : listLines)
		{
			istringstream converter(line);

			unsigned int id;
			unsigned int marker;
			Vector2i vertices;
			char tmp;

			converter >>  id >> tmp >> marker >> tmp >> mesh.Cell1DsExtrema(0, id) >>
			tmp >> mesh.Cell1DsExtrema(1, id);
			mesh.Cell1DsId.push_back(id);   
			mesh.Cell1DsMarker.push_back(marker);

		}

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////
	bool ImportCell2Ds(PolygonalMesh& mesh)
	{
		ifstream file;
		file.open("./Cell2Ds.csv");

		if(file.fail())
			return false;

		list<string> listLines;
		string line;
		while (getline(file, line))
			listLines.push_back(line);

		file.close();

		listLines.pop_front();

		mesh.NumCell2Ds = listLines.size();
		
		if (mesh.NumCell2Ds == 0)
		{
			cerr << "There is no cell 2D" << endl;
			return false;
		}
		
		mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
		mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
		mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);
		mesh.Cell2DsMarker.reserve(mesh.NumCell2Ds);

		
		for (const string& line : listLines)
		{
			istringstream converter(line);
			
			unsigned int id;
			unsigned int marker;
			unsigned int numVertices;
			unsigned int numEdges;
			char tmp;
			
			converter >> id >> tmp >> marker >> tmp >> numVertices >> tmp;
			
			vector<unsigned int> vertices(numVertices);
			
			for (unsigned int i = 0; i < numVertices; i++)
			{
				converter >> vertices[i];
				if (i < numVertices - 1)
					converter >> tmp;
			}
			
			converter >> tmp >> numEdges >> tmp;
			
			vector<unsigned int> edges(numEdges);
			for (unsigned int i = 0; i < numEdges; i++)
			{
				converter >> edges[i];
				if (i < numEdges - 1)
					converter >> tmp;
			}
			
			mesh.Cell2DsId.push_back(id);
			mesh.Cell2DsVertices.push_back(vertices);
			mesh.Cell2DsEdges.push_back(edges);
			mesh.Cell2DsMarker.push_back(marker);

		}
		return true;
	}
}

