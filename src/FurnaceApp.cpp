/* 
 *
 * Author: Victor Sand (victor.sand@gmail.com)
 * FurnaceApp: Application that uses Furnace to produce a VDF file
 * 
 */

#include <Furnace.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

int main() {

  // Read config (very simple implementation) 
  std::string sourceFolder = "";
  std::string outFilename = "";
  unsigned int xDim = 0, yDim = 0, zDim = 0;
  std::string modelName = "";
  osp::Furnace::ModelType modelType = osp::Furnace::NO_MODEL;
  std::string gridName = "";
  osp::Furnace::GridType gridType = osp::Furnace::NO_GRID;
  
  std::string config = "config/furnaceConfig.txt";
  std::ifstream in;
  in.open(config.c_str(), std::ifstream::in);
  if (!in.is_open()) {
    std::cerr << "Could not open config file: " << config << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(in, line)) {
    // Ignore empty lines and comments ('#')
    if (!line.empty() && line.at(0) != '#') {
      // Read variable name
      std::stringstream ss;
      ss.str(line);
      std::string var;
      ss >> var;
      // Read value
      if (var == "source_folder") {
        ss >> sourceFolder;
      } else if (var == "out_filename") {
        ss >> outFilename;
      } else if (var == "grid_type") {
        ss >> gridName;
        if (gridName == "cartesian") {
          gridType = osp::Furnace::CARTESIAN;
        } else if (gridName == "spherical") {
          gridType = osp::Furnace::SPHERICAL;
        } else {
          std::cerr << "Grid type " << gridName << " unknown" << std::endl;
          exit(1);
        }
      } else if (var == "dimensions") {
        ss >> xDim;
        ss >> yDim;
        ss >> zDim;
      } else if (var == "model_name") {
        ss >> modelName;
        if (modelName == "ENLIL") {
          modelType = osp::Furnace::ENLIL;
        } else {
          std::cerr << "Model name " << modelName << " unknown" << std::endl;
          exit(1);
        }
      } else {
        std::cerr << "Variable " << var << " unknown" << std::endl;
        exit(1);
      }
    }
  } // while getline

  // Print chosen variables 
  std::cout << "Running Furnace with the following settings: " << std::endl;
  std::cout << "Source folder: " << sourceFolder << std::endl;
  std::cout << "Out filename: " << outFilename << std::endl;
  std::cout << "Dimensions: " << xDim << " " << yDim << " " << zDim<<std::endl;
  std::cout << "Model: " << modelName << std::endl;
  std::cout << "Grid: " << gridName << std::endl;

  // Create a Furnace instance 
  osp::Furnace *furnace = osp::Furnace::New();
  
  if (!furnace) {
    std::cerr << "Failed to create Furnace instance" << std::endl;
    exit(1);
  }

  // Set variables from config

  // Setting the model also initialized the reader
  if (!furnace->SetModelType(modelType)) {
    std::cerr << "Failed to set model type" << std::endl;
    exit(1);
  }
  furnace->SetGridType(gridType);
  furnace->SetDimensions(xDim, yDim, zDim);
  furnace->SetSourceFolder(sourceFolder);
  furnace->SetOutFilename(outFilename);

  // Read folder
  if (!furnace->ReadFolder()) {
    std::cerr << "Failed to read folder: " << sourceFolder << std::endl;
    exit(1);
  }

  // Write to output
  if (!furnace->Write()) {
    std::cerr << "Failed to write to output: " << outFilename << std::endl;
    exit(1);
  }

  // Clean up and exit
  delete furnace;
  exit(0);

}
