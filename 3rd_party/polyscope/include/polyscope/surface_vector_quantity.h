// Copyright 2017-2019, Nicholas Sharp and the Polyscope contributors. http://polyscope.run.
#pragma once

#include "polyscope/affine_remapper.h"
#include "polyscope/ribbon_artist.h"
#include "polyscope/surface_mesh.h"
#include "polyscope/surface_mesh_enums.h"

namespace polyscope {

// ==== Common base class

// Represents a general vector field associated with a surface mesh, including
// R3 fields in the ambient space and R2 fields embedded in the surface
class SurfaceVectorQuantity : public SurfaceMeshQuantity {
public:
  SurfaceVectorQuantity(std::string name, SurfaceMesh& mesh_, MeshElement definedOn_,
                        VectorType vectorType_ = VectorType::STANDARD);


  virtual void draw() override;
  virtual void buildCustomUI() override;

  // Allow children to append to the UI
  virtual void drawSubUI();

  // === Members

  // Note: these vectors are not the raw vectors passed in by the user, but have been rescaled such that the longest has
  // length 1 (unless type is VectorType::Ambient)
  const VectorType vectorType;
  std::vector<glm::vec3> vectorRoots;
  std::vector<glm::vec3> vectors;

  void writeToFile(std::string filename = "");

  // === Option accessors

  //  The vectors will be scaled such that the longest vector is this long
  SurfaceVectorQuantity* setVectorLengthScale(double newLength, bool isRelative = true);
  double getVectorLengthScale();

  // The radius of the vectors
  SurfaceVectorQuantity* setVectorRadius(double val, bool isRelative = true);
  double getVectorRadius();

  // The color of the vectors
  SurfaceVectorQuantity* setVectorColor(glm::vec3 color);
  glm::vec3 getVectorColor();

  // Enable the ribbon visualization
  SurfaceVectorQuantity* setRibbonEnabled(bool newVal);
  bool isRibbonEnabled();

protected:
  // === Visualization options
  PersistentValue<ScaledValue<float>> vectorLengthMult;
  PersistentValue<ScaledValue<float>> vectorRadius;
  PersistentValue<glm::vec3> vectorColor;

  // The map that takes values to [0,1] for drawing
  AffineRemapper<glm::vec3> mapper;

  MeshElement definedOn;

  // A ribbon viz that is appropriate for some fields
  std::unique_ptr<RibbonArtist> ribbonArtist;
  PersistentValue<bool> ribbonEnabled;

  // GL things
  void prepareProgram();
  std::unique_ptr<gl::GLProgram> program;

  // Set up the mapper for vectors
  void prepareVectorMapper();
};


// ==== R3 vectors at vertices

class SurfaceVertexVectorQuantity : public SurfaceVectorQuantity {
public:
  SurfaceVertexVectorQuantity(std::string name, std::vector<glm::vec3> vectors_, SurfaceMesh& mesh_,
                              VectorType vectorType_ = VectorType::STANDARD);

  std::vector<glm::vec3> vectorField;

  virtual std::string niceName() override;
  virtual void buildVertexInfoGUI(size_t vInd) override;
};


// ==== R3 vectors at faces

class SurfaceFaceVectorQuantity : public SurfaceVectorQuantity {
public:
  SurfaceFaceVectorQuantity(std::string name, std::vector<glm::vec3> vectors_, SurfaceMesh& mesh_,
                            VectorType vectorType_ = VectorType::STANDARD);

  std::vector<glm::vec3> vectorField;

  virtual std::string niceName() override;
  virtual void buildFaceInfoGUI(size_t fInd) override;
};


// ==== Intrinsic vectors at faces

class SurfaceFaceIntrinsicVectorQuantity : public SurfaceVectorQuantity {
public:
  SurfaceFaceIntrinsicVectorQuantity(std::string name, std::vector<glm::vec2> vectors_, SurfaceMesh& mesh_,
                                     int nSym = 1, VectorType vectorType_ = VectorType::STANDARD);

  int nSym;
  std::vector<glm::vec2> vectorField;

  virtual void draw() override;

  void drawSubUI() override;

  virtual std::string niceName() override;
  void buildFaceInfoGUI(size_t fInd) override;
};


// ==== Intrinsic vectors at vertices

class SurfaceVertexIntrinsicVectorQuantity : public SurfaceVectorQuantity {
public:
  SurfaceVertexIntrinsicVectorQuantity(std::string name, std::vector<glm::vec2> vectors_, SurfaceMesh& mesh_,
                                       int nSym = 1, VectorType vectorType_ = VectorType::STANDARD);

  int nSym;
  std::vector<glm::vec2> vectorField;

  virtual void draw() override;

  void drawSubUI() override;

  virtual std::string niceName() override;
  void buildVertexInfoGUI(size_t vInd) override;
};


// ==== Intrinsic one form on edges

class SurfaceOneFormIntrinsicVectorQuantity : public SurfaceVectorQuantity {
public:
  SurfaceOneFormIntrinsicVectorQuantity(std::string name, std::vector<double> oneForm_, std::vector<char> orientations_,
                                        SurfaceMesh& mesh_);

  std::vector<double> oneForm;
  std::vector<glm::vec2> mappedVectorField;

  virtual void draw() override;

  void drawSubUI() override;

  virtual std::string niceName() override;
  void buildEdgeInfoGUI(size_t eInd) override;
  void buildFaceInfoGUI(size_t fInd) override;
};

} // namespace polyscope
