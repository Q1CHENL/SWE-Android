
#include "VTKSWE1DWriter.hpp"
#include <iostream>

Writers::VTKSWE1DWriter::VTKSWE1DWriter(const std::string& basename, const RealType cellSize):
  basename_(basename),
  cellSize_(cellSize),
  timeStep_(0) {

  // Initialize VTP stream
  std::ostringstream vtpFileName;
  vtpFileName << basename_ << ".vtp";

  vtpFile_ = new std::ofstream(vtpFileName.str().c_str());

  // Write VTP header
  *vtpFile_
    << "<?xml version=\"1.0\"?>" << std::endl
    << R"(<VTKFile type="Collection" version="0.1">)" << std::endl
    << "<Collection>" << std::endl;
}

Writers::VTKSWE1DWriter::~VTKSWE1DWriter() {
  // Close VTP file
  *vtpFile_ << "</Collection>" << std::endl << "</VTKFile>" << std::endl;
  delete vtpFile_;
}

void Writers::VTKSWE1DWriter::write(const RealType time, const RealType* h, const RealType* hu, unsigned int size) {
  // Generate VTK file name
  std::string fileName = generateFileName();

  // Add current time to VTP collection
  *vtpFile_
    << "<DataSet timestep=\"" << time << R"(0" group="" part="0" file=")" << fileName << "\"/> " << std::endl;

  // Write VTK file
  std::ofstream vtkFile(fileName);
  // assert(vtkFile.good());

  // VTL XML header
  vtkFile
    << "<?xml version=\"1.0\"?>" << std::endl
    << "<VTKFile type=\"RectilinearGrid\">" << std::endl
    << "<RectilinearGrid WholeExtent=\"0 " << size << " 0 0 0 0\">" << std::endl
    << "<Piece Extent=\"0 " << size << " 0 0 0 0\">" << std::endl;

  vtkFile << "<Coordinates>" << std::endl << R"(<DataArray type="Float32" format="ascii">)" << std::endl;

  // Grid points
  for (unsigned int i = 0; i < size + 1; i++) {
    vtkFile << cellSize_ * i << "" << std::endl;
  }

  vtkFile << "</DataArray>" << std::endl;

  vtkFile
    << R"(<DataArray type="Float32" format="ascii">)" << std::endl
    << "0" << std::endl
    << "</DataArray>" << std::endl;

  vtkFile
    << R"(<DataArray type="Float32" format="ascii">)" << std::endl
    << "0" << std::endl
    << "</DataArray>" << std::endl;

  vtkFile << "</Coordinates>" << std::endl;

  vtkFile << "<CellData>" << std::endl;

  // Water surface height
  vtkFile << R"(<DataArray Name="h" type="Float32" format="ascii">)" << std::endl;
  for (unsigned int i = 1; i < size + 1; i++) {
    vtkFile << h[i] << std::endl;
  }
  vtkFile << "</DataArray>" << std::endl;

  // Momentum
  vtkFile << R"(<DataArray Name="hu" type="Float32" format="ascii">)" << std::endl;
  for (unsigned int i = 1; i < size + 1; i++) {
    vtkFile << hu[i] << std::endl;
  }
  vtkFile << "</DataArray>" << std::endl;

  vtkFile << "</CellData>" << std::endl << "</Piece>" << std::endl;

  vtkFile << "</RectilinearGrid>" << std::endl << "</VTKFile>" << std::endl;

  // Increment time step
  timeStep_++;
}

std::string Writers::VTKSWE1DWriter::generateFileName() {
  std::ostringstream name;
  name << basename_ << '_' << timeStep_ << ".vtr";

  return name.str();
}
